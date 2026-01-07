/**
 * event.c
 *
 * Written by Renato Fermi <repiazza@gmail.com>
 *
 * Description: Events of the game
 */

#ifdef USE_SDL2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <sys_interface.h>
#include <card_game.h>
#include <deck.h>
#include <debuff.h>
#include <monster.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <player.h>
#include <battle.h>
#include <sdl_api.h>
#include <trace.h>
#include <event.h>
#include <event_render.h>
#include <terminal_utils.h>
#include <game.h>

int giPendingCard = -1;
int giHoverCard = -1;
int giHoverMonster = -1;


/* ---- helpers locais ---- */
static void vMaybeToggleFullscreen(SDL_Event *pSDL_EVENT_Ev) {
  int bIsAlt;
  if (pSDL_EVENT_Ev->type != SDL_KEYDOWN)
    return;
  bIsAlt = (pSDL_EVENT_Ev->key.keysym.mod & KMOD_ALT) ? TRUE : FALSE;
  if (bIsAlt && pSDL_EVENT_Ev->key.keysym.sym == SDLK_RETURN) {
    vSDL_ToggleFullscreen();
  }
}

int iHandleDialogMouse(SDL_Event *pEv, SDL_Renderer *pSDL_Renderer) {
  int iWinW, iWinH;
  int iDlgY, iDlgH;
  int iMarginSuperior = 2;
  int iMarginInferior = 2;

  SDL_GetRendererOutputSize(pSDL_Renderer, &iWinW, &iWinH);

  iDlgY = 50 + 500 + iMarginSuperior;
  iDlgH = iWinH - iDlgY - iMarginInferior;
  if (iDlgH < 0)
    iDlgH = 0;

  return iSDL_DialogHandleMouse(
    pEv,
    &gstDialogLayout
  );
}



/* ---- API pública ---- */
void vEVENT_Init(void) {
  giPendingCard = -1;
  giHoverCard = -1;
  giHoverMonster = -1;
  vEVR_Init();
}

void vEVENT_Quit(void) {
  giPendingCard = -1;
  giHoverCard = -1;
  giHoverMonster = -1;
  vEVR_Quit();
}


/* handler único com contexto completo */
int iEVENT_HandlePollEv(SDL_Event *pSDL_EVENT_Ev,
                        int iRedrawCurrentAction,
                        SDL_Renderer *pSDL_Renderer,
                        PSTRUCT_DECK pstDeck,
                        PSTRUCT_MONSTER pastMonsters,
                        int iMonsterCt,
                        int *pbRunning) {
  int iRedrawReturnStatus;

  if ( DEBUG_SDL_ALL ) vTraceBegin();

  UNUSED(pSDL_Renderer);

  iRedrawReturnStatus = iRedrawCurrentAction;

  if ( gstGame.iStatus == STATUS_PAUSE ) {
    if ( pSDL_EVENT_Ev->type != SDL_KEYDOWN && pSDL_EVENT_Ev->key.keysym.sym != SDLK_SPACE ) {
      return REDRAW_NONE;
    }
  }

  switch (pSDL_EVENT_Ev->type) {

    case SDL_QUIT: {
      *pbRunning = FALSE;
      break;
    }

    case SDL_KEYDOWN: {
      /* Espaco Pausa o jogo */
      if ( pSDL_EVENT_Ev->key.keysym.sym == SDLK_SPACE ) {
        char szMsg[32] = "";
        memset(szMsg, 0x00, sizeof(szMsg));
        switch ( gstGame.iStatus = (gstGame.iStatus == STATUS_PAUSE ? STATUS_GAMING : STATUS_PAUSE) ) {
          case STATUS_PAUSE: {
            // snprintf(szMsg, sizeof(szMsg), "Jogo pausado");
            gstGame.iLastState = gstGame.iState;
            gstGame.iState = STATE_PAUSE_GAMING;
            break;
          }
          case STATUS_GAMING:
          default: {
            /* TODO: Restore LastStatus function */
            gstGame.iState = gstGame.iLastState;
            gstGame.iLastState = STATE_NONE;
            // snprintf(szMsg, sizeof(szMsg), "Jogo despausado");
            break;
          }
        }
        // vPrintLine(szMsg, NO_NEW_LINE);
        iRedrawReturnStatus = REDRAW_TABLE;
        break;
      }

      if (pSDL_EVENT_Ev->key.keysym.sym == SDLK_ESCAPE) {
        *pbRunning = FALSE;
        break;
      }
      vMaybeToggleFullscreen(pSDL_EVENT_Ev);
      iRedrawReturnStatus |= REDRAW_TABLE; /* troca de fullscreen pede full redraw */
      break;
    }
    case SDL_MOUSEMOTION: {
      int iX;
      int iY;
      int iNewHoverCard;
      int iNewHoverMonster;

      iX = pSDL_EVENT_Ev->motion.x;
      iY = pSDL_EVENT_Ev->motion.y;

      iNewHoverCard = iSDL_HandIndexFromPoint(iX, iY, pstDeck);
      iNewHoverMonster = iSDL_MonsterIndexFromPoint(iX, iY, pastMonsters, iMonsterCt);

      if (iNewHoverCard != giHoverCard || iNewHoverMonster != giHoverMonster) {
        giHoverCard = iNewHoverCard;
        giHoverMonster = iNewHoverMonster;
        iRedrawReturnStatus |= REDRAW_TABLE;
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      if (pSDL_EVENT_Ev->button.button == SDL_BUTTON_LEFT) {
        int iX;
        int iY;
        int iHandIdx;
        int iMonIdx;
        PSTRUCT_CARD pstCard;
        int iAlive;
        int iLastM;
        int iMM;

        iX = pSDL_EVENT_Ev->button.x;
        iY = pSDL_EVENT_Ev->button.y;

        /* 1) diálogo (setas ↑/↓) */
        if ( iRedrawReturnStatus != REDRAW_NONE ) break;

        /* 2) clique nas cartas / seleção de alvo */
        iHandIdx = iSDL_HandIndexFromPoint(iX, iY, pstDeck);
        if (iHandIdx >= 0) {

          if (giPendingCard == iHandIdx) {
            giPendingCard = -1;
            giSelectedMonster = -1;
            iRedrawReturnStatus |= REDRAW_TABLE;
            break;
          }

          if (!bHasAnyPlayableCard(pstDeck)) {
            giPendingCard = -1;
            iRedrawReturnStatus |= REDRAW_TABLE;
            break;
          }

          giPendingCard = iHandIdx;
          pstCard = &pstDeck->astHand[giPendingCard];

          /* alvo: self → joga direto */
          if (pstCard->iTarget == CARD_TARGET_SELF) {
            if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("EVENT: target self [%s]", pstCard->szName);
            vPlayCard(&giPendingCard, pstDeck);
            if ( giPendingCard < 0 )
              return giPendingCard;
              
            pstCard = &pstDeck->astHand[giPendingCard];
            if ( iHandlePlayerActionByCard(pstCard, pastMonsters, iMonsterCt) == CARD_NONE ) 
              return -1;
              
            gstPlayer.iEnergy -= pstCard->iCost;
            vDiscardCard(pstDeck, giPendingCard);
            giPendingCard = -1;
            iRedrawReturnStatus |= (REDRAW_TABLE);
            break;
          }

          /* alvo: múltiplos → joga direto */
          if (pstCard->iTarget == CARD_TARGET_MULTIPLE) {
            if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("EVENT: target multiple [%s]", pstCard->szName);
            vPlayCard(&giPendingCard, pstDeck);
            if ( giPendingCard < 0 )
              return giPendingCard;

            pstCard = &pstDeck->astHand[giPendingCard];
            if ( iHandlePlayerActionByCard(pstCard, pastMonsters, iMonsterCt) == CARD_NONE ) 
              return -1;
              
            gstPlayer.iEnergy -= pstCard->iCost;
            vDiscardCard(pstDeck, giPendingCard);
            
            giPendingCard = -1;
            iRedrawReturnStatus |= (REDRAW_TABLE);
            break;
          }

          /* alvo: único → se só 1 vivo, autodestino; senão aguarda clique no monstro */
          iAlive = 0;
          iLastM = -1;
          for (iMM = 0; iMM < iMonsterCt; iMM++) {
            if (pastMonsters[iMM].iHP > 0) {
              iAlive++;
              iLastM = iMM;
            }
          }

          if (iAlive == 1 && iLastM >= 0) {
            if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("EVENT: auto target [%s] -> monstro %d", pstCard->szName, iLastM);
            vPlayCard(&giPendingCard, pstDeck);
            if ( giPendingCard < 0 )
              return giPendingCard;

            pstCard = &pstDeck->astHand[giPendingCard];
            if ( iHandlePlayerActionByCard(pstCard, pastMonsters, iMonsterCt) == CARD_NONE ) 
              return -1;
              
            gstPlayer.iEnergy -= pstCard->iCost;
            vDiscardCard(pstDeck, giPendingCard);
            giPendingCard = -1;
            iRedrawReturnStatus |= (REDRAW_TABLE);
          }
          /* se iAlive > 1, apenas mantém giPendingCard e aguarda clique no monstro */
          break;
        }

        /* caso não clique na mão: se há carta pendente, tenta monstros */
        if (giPendingCard >= 0) {
          iMonIdx = iSDL_MonsterIndexFromPoint(iX, iY, pastMonsters, iMonsterCt);
          if (iMonIdx >= 0 && pastMonsters[iMonIdx].iHP > 0) {
            giSelectedMonster = iMonIdx;
            vPlayCard(&giPendingCard, pstDeck);
            if ( giPendingCard < 0 )
              return giPendingCard;

            pstCard = &pstDeck->astHand[giPendingCard];
            if ( iHandlePlayerActionByCard(pstCard, pastMonsters, iMonsterCt) == CARD_NONE ) 
              return -1;
              
            gstPlayer.iEnergy -= pstCard->iCost;
            vDiscardCard(pstDeck, giPendingCard);
            giPendingCard = -1;
            iRedrawReturnStatus |= (REDRAW_TABLE);
          }
          if (iMonIdx < 0) {
            giPendingCard = -1;
            giSelectedMonster = -1;
            iRedrawReturnStatus |= REDRAW_TABLE;
          }
        }
      }
      break;
    }

    case SDL_MOUSEWHEEL: {
      /* se quiser, pode marcar só o diálogo (scroll de lista, etc.) */
      // iRedrawReturnStatus |= REDRAW_DIALOG;
      break;
    }

    case SDL_WINDOWEVENT: {
      /* resize, expose, etc. — força re-render completo */
      // iRedrawReturnStatus |= REDRAW_ALL;
      break;
    }

    default: {
      break;
    }
  }

  if ( DEBUG_SDL_ALL ) vTraceVarArgsFn("end iRedrawReturnStatus[%d]", iRedrawReturnStatus);

  return iRedrawReturnStatus;
}

#endif /* USE_SDL2 */
