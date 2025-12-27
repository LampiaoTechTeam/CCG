#include <stdio.h>
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

int gbWelcomeOpen = FALSE;

#ifdef USE_SDL2
  void vSDL_WelcomeInit(void){
    return;
  }

  /* Desenha o Welcome completo */
  int iWelcomeDraw(SDL_Renderer *pSDL_Renderer){
    SDL_Rect* pstPanel;
    SDL_Rect* pstItem;
    int i;
    int iX;
    int iY;
    SDL_Color stCorBranco = {255, 255, 255, 255};
    PSTRUCT_ELEMENT pstMenu;

    if ( pSDL_Renderer == NULL ) return -1;

    pstMenu = pstSCREEN_GetElementByType(ELM_TYPE_MENU);

    // vSDL_DrawMenu(pSDL_Renderer, pstMenu);
// #if 0
    SDL_RenderClear(pSDL_Renderer);

    /* Painel principal */
    pstPanel = (SDL_Rect*)&pstMenu->stRect;

    vSDL_DrawRectShadow(pSDL_Renderer, pstPanel, 6, 6, 80);
    SDL_SetRenderDrawColor(pSDL_Renderer, 40, 40, 50, 255);
    SDL_RenderFillRect(pSDL_Renderer, pstPanel);
    SDL_SetRenderDrawColor(pSDL_Renderer, 220, 220, 230, 255);
    SDL_RenderDrawRect(pSDL_Renderer, pstPanel);

    /* Título */
    vSDL_DrawText(pSDL_Renderer, pstMenu->szText, pstPanel->x + 20, pstPanel->y + 20, stCorBranco);

    /* Itens do menu */
    iX = pstPanel->x + 40;
    iY = pstPanel->y + 60;

    for ( i = 0; i < pstMenu->iCtItems; i++ ) {
      pstMenu->astItem[i].stRect.x = iX;
      pstMenu->astItem[i].stRect.y = iY + (i * 60);
      pstMenu->astItem[i].stRect.w = pstPanel->w - 80;
      pstMenu->astItem[i].stRect.h = 50;
      pstItem = (SDL_Rect*)&pstMenu->astItem[i].stRect;
      /* Fundo do item */
      if ( i == pstMenu->iSelectedItemIdx ) SDL_SetRenderDrawColor(pSDL_Renderer, 70, 90, 130, 255);
      else
        SDL_SetRenderDrawColor(pSDL_Renderer, 50, 50, 60, 255);
      SDL_RenderFillRect(pSDL_Renderer, pstItem);

      SDL_SetRenderDrawColor(pSDL_Renderer, 200, 200, 210, 255);
      SDL_RenderDrawRect(pSDL_Renderer, pstItem);

      /* Nome */
      vSDL_DrawText(pSDL_Renderer, pstMenu->astItem[i].szText, pstItem->x + 10, pstItem->y + 15, stCorBranco);
    }
    SDL_RenderPresent(pSDL_Renderer);
// #endif
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
              vMessageBox("Nenhum save para carregar!", "Pressione qualquer tecla para continuar");
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

