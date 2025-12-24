#include <stdio.h>
#include <card_game.h>
#include <debuff.h>
#include <deck.h>
#include <monster.h>
#ifdef USE_SDL2
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_ttf.h>
#endif
#include <player.h>
#include <terminal_utils.h>
#include <trace.h>
#include <input.h>
#include <dialog.h>
#include <battle.h>
#ifdef USE_SDL2
  #include <sdl_api.h>
  #include <event_render.h>
#endif



int bHasAnyPlayableCard(PSTRUCT_DECK pstDeck){
  PSTRUCT_CARD pstCard;
  int iTotalHandCards;
  int ii;

  iTotalHandCards = pstDeck->iHandCount - 1;
  #ifdef USE_SDL2
    if ( gbSDL_Mode )
      iTotalHandCards = pstDeck->iHandCount;
  #endif

  for ( ii = 0 ; ii < iTotalHandCards ; ii++ ){
    pstCard = &pstDeck->astHand[ii];
    if ( pstCard->iCost <= gstPlayer.iEnergy )
      return TRUE;
  }
  return FALSE;
}

int iInitCurrentLevel(int iCurrLevel, PSTRUCT_MONSTER pastMonsters, int *iMonsterCount){
  vFreeDialog();
  vInitMonstersForLevel(pastMonsters, iCurrLevel, iMonsterCount);
  return 0;
}

int iSelectMonsterFromList(int iMonsterCt){
  int iChoice;
  char szLine[1024];
#ifdef USE_SDL2
  if ( gbSDL_Mode ){
    if (giSelectedMonster >= 0 && giSelectedMonster < iMonsterCt) {
      return giSelectedMonster;
    }
    return 0;
  }
#endif
  sprintf(szLine, "\nEscolha um monstro (1..%d). 'q' para sair.", iMonsterCt);
  vPrintLine(szLine, INSERT_NEW_LINE);
  iChoice = iPortableGetchar();
  return (iChoice - 48); /** ASCII char */
}

/**
 * Prints game table (console mode)
 */
void vShowTable(PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCount){
#ifndef USE_SDL2
  vShowPlayer();
  vShowDeck(pstDeck);
  vShowMonsters(pastMonsters, iMonsterCount);
#else
  /* no SDL a mesa é desenhada em sdl_api.c */
  if ( !gbSDL_Mode ){
    vShowPlayer();
    vShowDeck(pstDeck);
    vShowMonsters(pastMonsters, iMonsterCount);
  }
#endif
}

int iHandlePlayerActionByCard(PSTRUCT_CARD pstCard, PSTRUCT_MONSTER pastMonsters, int iMonsterCt){
  int iTarget = 0;
  int ii = 0;
  int bUsed = FALSE;
  int iAliveCt = 0;

  vTraceVarArgsFn("Card Type=%d", pstCard->iType);
  
  if ( (iAliveCt = iAliveMonsterQty(pastMonsters, iMonsterCt)) == 0 ){
    return CARD_NONE;
  }

  switch ( pstCard->iType ){
    case CARD_STRIKE:
    case CARD_FIREBALL:
    case CARD_POISON:
    case CARD_PARALIZE:
    {
      int iRoundsParalized = 0;
      char* pszRoundStr = "rodadas";
      time_t lTime;

      if ( iAliveCt == 1 ){
        iTarget = iGetFirstAliveMonster(pastMonsters, iMonsterCt);
      }
      else if ( pstCard->iTarget != CARD_TARGET_MULTIPLE && iMonsterCt > 1 ){
        /* Single target */
        #ifdef USE_SDL2
          /* SDL2: Selected by click */
          if (gbSDL_Mode) {
            iTarget = giSelectedMonster;
            vTraceVarArgsFn("SDL Target [%d]", iTarget);
          }
          else{
            /* console mode, asks for user input */
            iTarget = iSelectMonsterFromList(iMonsterCt);    
            iTarget--; /* console: adjusts target base to 0 */
          }
        #else
          /* console mode, asks for user input */
          iTarget = iSelectMonsterFromList(iMonsterCt);
          iTarget--; /*  console: adjusts target base to 0 */
        #endif  

        if ( iTarget < 0 )
          iTarget = 0;
        if ( iTarget >= iMonsterCt )
          return CARD_NONE;
      }
      else if ( pstCard->iTarget == CARD_TARGET_MULTIPLE ){
        /** Target all? */
        iTarget = iMonsterCt;
      }
      
      for ( ; ii <= iTarget; ii++ ) {
        char szLine[1024];
        /* If we're not targeting all monsters it means we're doing a single use */
        if ( iTarget < iMonsterCt ) ii = iTarget;
        if ( pstCard->iTarget == CARD_TARGET_MULTIPLE && ii == iTarget && bUsed ) break;
        if ( pastMonsters[ii].iHP <= 0 ) continue;

        if ( pstCard->iType == CARD_PARALIZE ){
          time(&lTime);
          srand(lTime);
          iRoundsParalized = (rand() % 2) + 2;
          pastMonsters[ii].astDebuff[pastMonsters[ii].iDebuffCt].iType   = DEBUFF_TYPE_PARALYZE;
          pastMonsters[ii].astDebuff[pastMonsters[ii].iDebuffCt].iDamage = pstCard->iValue;
          pastMonsters[ii].astDebuff[pastMonsters[ii].iDebuffCt].iRounds = iRoundsParalized;
          pastMonsters[ii].iDebuffCt++;
          snprintf(szLine, sizeof(szLine),
            "Voce aplicou paralisia ao monstro %s por %d %s",
            pastMonsters[ii].szName,
            iRoundsParalized,
            pszRoundStr
          );
          vPrintLine(szLine, INSERT_NEW_LINE);
          bUsed = TRUE;
        }
        else if ( pstCard->iType == CARD_POISON ) {
          pastMonsters[ii].astDebuff[pastMonsters[ii].iDebuffCt].iType   = DEBUFF_TYPE_POISON;
          pastMonsters[ii].astDebuff[pastMonsters[ii].iDebuffCt].iDamage = pstCard->iValue;
          pastMonsters[ii].astDebuff[pastMonsters[ii].iDebuffCt].iRounds = DEBUFF_POISON_CYCS;
          pastMonsters[ii].iDebuffCt++;
          snprintf(szLine, sizeof(szLine),
            "Voce aplicou veneno ao monstro %s por %d %s",
            pastMonsters[ii].szName,
            DEBUFF_POISON_CYCS,
            pszRoundStr
          );
          vPrintLine(szLine, INSERT_NEW_LINE);
          bUsed = TRUE;
        }
        else {
          pastMonsters[ii].iBlock  -= pstCard->iValue;
          if (pastMonsters[ii].iBlock < 0) {
            pastMonsters[ii].iHP   -= (pastMonsters[ii].iBlock)*-1;
            pastMonsters[ii].iBlock = 0;
          }
          else {
            pastMonsters[ii].iHP   -= pstCard->iValue;
          }
          sprintf(szLine, " Causou %d dano a %s", pstCard->iValue, pastMonsters[ii].szName);
          vPrintLine(szLine, INSERT_NEW_LINE);
          #ifdef USE_SDL2
            if (gbSDL_Mode) {
              vEVR_PushMonsterDamage(ii, pstCard->iValue);
            }
          #endif
          bUsed = TRUE;
          if ( iTarget < iMonsterCt ) ii++;
        }
      }
      break;
    }
    case CARD_DEFEND:
    {
      char szLine[128];
      gstPlayer.iBlock += pstCard->iValue;
      sprintf(szLine, "Voce se defendeu. +%d = %d Escudo", pstCard->iValue, gstPlayer.iBlock);
      vPrintLine(szLine, INSERT_NEW_LINE);
      #ifdef USE_SDL2
        if (gbSDL_Mode) {
          vEVR_PushPlayerShield(pstCard->iValue);
        }
      #endif
      bUsed = TRUE;
      break;
    }
    case CARD_HEAL:
    {
      char szLine[128];
      gstPlayer.iHP += pstCard->iValue;
      if ( gstPlayer.iHP >= PLAYER_HP_MAX )
        gstPlayer.iHP = PLAYER_HP_MAX;
        
      sprintf(szLine, "Voce se curou. +%d = %d HP", pstCard->iValue, gstPlayer.iHP);
      vPrintLine(szLine, INSERT_NEW_LINE);
      #ifdef USE_SDL2
        if (gbSDL_Mode) {
          vEVR_PushPlayerHeal(pstCard->iValue);
        }
      #endif
      bUsed = TRUE;
      break;
    }
    default:
      return CARD_NONE;
  }

  if ( !bUsed ) return CARD_NONE; 
  return pstCard->iType;
}

void vPlayCard(int *iCardIndex, PSTRUCT_DECK pstDeck){
  PSTRUCT_CARD pstCard;
  int iWrkCardIx;
  
  /** Console is not zero based we should correct when not SDL2 */
  iWrkCardIx = (*iCardIndex) - 1;
  #ifdef USE_SDL2
    /** Use the exacly selected index for SDL2 */
    if ( gbSDL_Mode )
      iWrkCardIx = (*iCardIndex);
  #endif
 
  if (iWrkCardIx < 0 || iWrkCardIx >= pstDeck->iHandCount)
    return;

  pstCard = &pstDeck->astHand[iWrkCardIx];

  if (gstPlayer.iEnergy <= 0 || pstCard->iCost > gstPlayer.iEnergy ) {
    char szMsg[128];
    sprintf(szMsg, "Energia insuficiente [%d/%d]", pstCard->iCost, gstPlayer.iEnergy);
    vPrintHighlitedLine(szMsg, INSERT_NEW_LINE);
    *iCardIndex = -2;
    return;
  }

  if ( !bStrIsEmpty(pstCard->szName) ){
    char szMsg[128];
    sprintf(szMsg, "Carta Escolhida: %s", pstCard->szName);
    vPrintHighlitedLine(szMsg, INSERT_NEW_LINE);
  }
  *iCardIndex = iWrkCardIx;

}
