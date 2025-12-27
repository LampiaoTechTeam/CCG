/**
 * welcome.c
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *
 * Description: Initial screen of the game
 */

#include <stdio.h>
#include <sys_interface.h>
#include <card_game.h>
#include <debuff.h>
#include <deck.h>
#include <trace.h>
#include <terminal_utils.h>
#include <monster.h>
#ifdef USE_SDL2
  #include <SDL2/SDL_rect.h>
#endif
#include <player.h>
#ifdef USE_SDL2
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_ttf.h>
  #include <sdl_api.h>
#endif
#include <input.h>
#include <welcome.h>
#include <xml.h>
#include <screen.h>
#include <game.h>
#include <msg.h>

int gbWelcomeOpen = FALSE;

#ifdef USE_SDL2
  void vSDL_WelcomeInit(void){
    return;
  }

  /* Desenha o Welcome completo */
  int iWelcomeDraw(SDL_Renderer *pSDL_Renderer){
    PSTRUCT_ELEMENT pstMenu;
    if ( pSDL_Renderer == NULL ) return -1;
    pstMenu = pstSCREEN_GetElementByType(ELM_TYPE_MENU);
    vSDL_DrawMenu(pSDL_Renderer, pstMenu);
    return 0;
  }

  int iSDL_OpenWelcome(SDL_Renderer *pSDL_Renderer){
    SDL_Event stEvent;
    PSTRUCT_ELEMENT pstMenu = NULL;
    static int bRunning = FALSE;

    memset(&stEvent  , 0x00, sizeof(stEvent  ));

    iSCREEN_SetLayout(LAYOUT_MAIN_MENU);

    pstMenu = pstSCREEN_GetElementByType(ELM_TYPE_MENU);
    if ( !pstMenu ) {
      if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn("FALHA em pstSCREEN_GetElementByType(ELM_TYPE_MENU)");
      return -1;
    }

    if ( pSDL_Renderer == NULL ) return -1;
    vSDL_WelcomeInit();
    gbWelcomeOpen = TRUE;
    bRunning = TRUE;
    while ( bRunning ) {
      /* desenha a tela */
      iWelcomeDraw(pSDL_Renderer);
      while ( SDL_PollEvent(&stEvent) ) {
        if ( stEvent.type == SDL_QUIT ) {
          return FINISH_PROGRAM;
        }
        if ( stEvent.type == SDL_KEYDOWN ) {
          SDL_Keycode key = stEvent.key.keysym.sym;
          if ( key == SDLK_DOWN ) {
            pstMenu->iSelectedItemIdx++;
            if ( pstMenu->iSelectedItemIdx >= pstMenu->iCtItems ) {
              pstMenu->iSelectedItemIdx = 0;
            }
          }
          else if ( key == SDLK_UP ) {
            pstMenu->iSelectedItemIdx--;
            if (pstMenu->iSelectedItemIdx < 0)
              pstMenu->iSelectedItemIdx = pstMenu->iCtItems - 1;
          }
        }
        else if ( stEvent.type == SDL_MOUSEBUTTONDOWN ) {
          int mx = stEvent.button.x;
          int my = stEvent.button.y;
          int ii = 0;

          /* Detectar clique nos botoes */
          for ( ii = 0; ii < pstMenu->iCtItems; ii++ ) {
            if ( bAreCoordsInSDL_Rect(
                  (SDL_Rect*)&pstMenu->astItem[ii].stRect,
                  mx,
                  my) ) {
              pstMenu->iSelectedItemIdx = ii;
              break;
            }
          }

          if ( !strcasecmp(pstMenu->astItem[pstMenu->iSelectedItemIdx].szText, "Comecar Novo Jogo") ) {
            bRunning = FALSE;
            memset(&gstGame, 0x00, sizeof(gstGame));
            break;
          }
          else if ( !strcasecmp(pstMenu->astItem[pstMenu->iSelectedItemIdx].szText, "Carregar Jogo") ) {
            memset(&gstGame, 0x00, sizeof(gstGame));
            if ( iGameLoad() ) {
              bRunning = FALSE;
              gbLoadGameFromFile = TRUE;
            }
            else {
              vSDL_MessageBox(MSG(MSG_NOT_FOUND_GAME_SAVE), MSG(MSG_PRESS_ANY_KEY_TO_CONTINUE));
            }
            break;
          }

          else if ( !strcasecmp(pstMenu->astItem[pstMenu->iSelectedItemIdx].szText, "Configuracoes") ) {
            // bRunning = FALSE;
            break;
          }
          else if ( !strcasecmp(pstMenu->astItem[pstMenu->iSelectedItemIdx].szText, "Sair") ) {
            gbWelcomeOpen = FALSE;
            bRunning = FALSE;
            return FINISH_PROGRAM;
          }
          else {
            continue;
          }
        }
        else if ( stEvent.type == SDL_MOUSEMOTION ) {
          int mx = stEvent.button.x;
          int my = stEvent.button.y;
          int ii = 0;
          /* Detectar movimento nos botoes */
          for ( ii = 0; ii < pstMenu->iCtItems; ii++ ) {
            if ( bAreCoordsInSDL_Rect(
              (SDL_Rect*)&pstMenu->astItem[ii].stRect,
              mx,
              my) ) {
              pstMenu->iSelectedItemIdx = ii;
              break;
            }
          }
        }
      }
    }

    gbWelcomeOpen = FALSE;

    return 0;
  }
#endif

