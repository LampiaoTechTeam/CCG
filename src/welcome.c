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

int bRunning = FALSE;
int iWelcomeExit = 0;

STRUCT_WELCOME_ITEM stWrkWelcomeItem;
STRUCT_WELCOME gstWelcome;
int gbWelcomeOpen = FALSE;

int icbackWelcomeItem(xmlNodePtr pstNode, void* pData __attribute__((unused)));
int icbackWelcome(xmlNodePtr pstNode, void* pData __attribute__((unused)));

STRUCT_XML astWelcomeItemXml[] = {
  { "ITEM", XMLTYPE_PROC    , 0                              , NULL                      , icbackWelcomeItem },
  { "ID"  , XMLTYPE_INT     , 0                              , &stWrkWelcomeItem.iIdItem , NULL          },
  { "NAME", XMLTYPE_STRING  , sizeof(stWrkWelcomeItem.szName), stWrkWelcomeItem.szName   , NULL          },
#ifdef USE_SLD2
  { "H"   , XMLTYPE_INT     , 0                              , &stWrkWelcomeItem.stRect.h, NULL          },
  { "W"   , XMLTYPE_INT     , 0                              , &stWrkWelcomeItem.stRect.w, NULL          },
  { "X"   , XMLTYPE_INT     , 0                              , &stWrkWelcomeItem.stRect.x, NULL          },
  { "Y"   , XMLTYPE_INT     , 0                              , &stWrkWelcomeItem.stRect.y, NULL          },
#endif
  { NULL  , XMLTYPE_NULL    , 0                              , NULL                      , NULL          }
};

STRUCT_XML astWelcomeXml[] = {
  { "WELCOME"   , XMLTYPE_PROC, 0, NULL                  , icbackWelcome },
  { "PANEL_X"   , XMLTYPE_INT , 0, &gstWelcome.iPanelX   , NULL          },
  { "PANEL_Y"   , XMLTYPE_INT , 0, &gstWelcome.iPanelY   , NULL          },
  { "PANEL_H"   , XMLTYPE_INT , 0, &gstWelcome.iPanelH   , NULL          },
  { "PANEL_W"   , XMLTYPE_INT , 0, &gstWelcome.iPanelW   , NULL          },
  { "CELL_H"    , XMLTYPE_INT , 0, &gstWelcome.iCellH    , NULL          },
  { "CELL_W"    , XMLTYPE_INT , 0, &gstWelcome.iCellW    , NULL          },
  { "CELL_PAD"  , XMLTYPE_INT , 0, &gstWelcome.iCellPad  , NULL          },
  { "TITLE_H"   , XMLTYPE_INT , 0, &gstWelcome.iTitleH   , NULL          },
  { "FOOTER_H"  , XMLTYPE_INT , 0, &gstWelcome.iFooterH  , NULL          },
  { "FONT_TITLE", XMLTYPE_INT , 0, &gstWelcome.iFontTitle, NULL          },
  { "FONT_UI"   , XMLTYPE_INT , 0, &gstWelcome.iFontUi   , NULL          },
  { "ITEM_LIST" , XMLTYPE_PROC , 0, NULL                 , icbackWelcome },
  { "ITEM"      , XMLTYPE_PROC , 0, NULL                 , icbackWelcomeItem },
  { NULL        , XMLTYPE_NULL , 0, NULL                 , NULL          }
};

int icbackWelcomeItem(xmlNodePtr pstNode, void* pData __attribute__((unused))) {
  if ( !strcasecmp((char*)pstNode->name, "ITEM") ) {
    memset(&stWrkWelcomeItem, 0x00, sizeof(stWrkWelcomeItem));
    iParseXmlFields(pstNode, astWelcomeItemXml);
    gstWelcome.aItems[gstWelcome.iNumItems] = stWrkWelcomeItem;
    gstWelcome.iNumItems++;
  }
  return 1;
}

int icbackWelcome(xmlNodePtr pstNode, void* pData __attribute__((unused))) {
  if ( !strcasecmp((char*)pstNode->name, "WELCOME") ) {
    memset(&gstWelcome, 0x00, sizeof(gstWelcome));
    iParseXmlFields(pstNode, astWelcomeXml);
  }
  if ( !strcasecmp((char*)pstNode->name, "ITEM_LIST") ) {
    memset(&stWrkWelcomeItem, 0x00, sizeof(stWrkWelcomeItem));
    iParseXmlFields(pstNode, astWelcomeItemXml);
  }
  if ( !strcasecmp((char*)pstNode->name, "ITEM") ) {
    gstWelcome.aItems[gstWelcome.iNumItems] = stWrkWelcomeItem;
    gstWelcome.iNumItems++;
  }
  return 1;
}

int bLoadWelcome(void) {
  return bLoadXmlFromFile("./conf/welcome.xml", astWelcomeXml);
}

#ifdef USE_SDL2
  void vSDL_WelcomeInit(){
    memset(&gstWelcome, 0x00, sizeof(gstWelcome));
    bLoadWelcome();
  }

  /* Desenha o Welcome completo */
  int iWelcomeDraw(SDL_Renderer *pSDL_Renderer){
    SDL_Rect stPanel;
    SDL_Rect stItem;
    SDL_Color stCorBranco = {255, 255, 255, 255};
    int i;
    int iX;
    int iY;

    if ( pSDL_Renderer == NULL ) return -1;

    SDL_RenderClear(pSDL_Renderer);

    /* Painel principal */
    stPanel.x = 80;
    stPanel.y = 60;
    stPanel.w = 640;
    stPanel.h = 460;

    vSDL_DrawRectShadow(pSDL_Renderer, &stPanel, 6, 6, 80);
    SDL_SetRenderDrawColor(pSDL_Renderer, 40, 40, 50, 255);
    SDL_RenderFillRect(pSDL_Renderer, &stPanel);
    SDL_SetRenderDrawColor(pSDL_Renderer, 220, 220, 230, 255);
    SDL_RenderDrawRect(pSDL_Renderer, &stPanel);

    /* Título */
    vSDL_DrawText(pSDL_Renderer, "WELCOME", stPanel.x + 20, stPanel.y + 20, stCorBranco);

    /* Itens da loja */
    iX = stPanel.x + 40;
    iY = stPanel.y + 60;

    for (i = 0; i < gstWelcome.iNumItems; i++) {
      gstWelcome.aItems[i].stRect.x = iX;
      gstWelcome.aItems[i].stRect.y = iY + (i * 60);
      gstWelcome.aItems[i].stRect.w = stPanel.w - 80;
      gstWelcome.aItems[i].stRect.h = 50;
      stItem = gstWelcome.aItems[i].stRect;
      /* Fundo do item */
      if ( i == gstWelcome.iSelected ) SDL_SetRenderDrawColor(pSDL_Renderer, 70, 90, 130, 255);
      else
        SDL_SetRenderDrawColor(pSDL_Renderer, 50, 50, 60, 255);
      SDL_RenderFillRect(pSDL_Renderer, &stItem);

      SDL_SetRenderDrawColor(pSDL_Renderer, 200, 200, 210, 255);
      SDL_RenderDrawRect(pSDL_Renderer, &stItem);

      /* Nome */
      vSDL_DrawText(pSDL_Renderer, gstWelcome.aItems[i].szName, stItem.x + 10, stItem.y + 15, stCorBranco);
    }
    SDL_RenderPresent(pSDL_Renderer);
    return 0;
  }

  int iSDL_OpenWelcome(SDL_Renderer *pSDL_Renderer){
    SDL_Event stEvent;
    memset(&stEvent  , 0x00, sizeof(stEvent  ));
    if ( pSDL_Renderer == NULL ) return -1;
    vSDL_WelcomeInit();
    gbWelcomeOpen = TRUE;
    bRunning = TRUE;
    while (bRunning) {
      /* desenha a tela */
      iWelcomeDraw(pSDL_Renderer);
      while (SDL_PollEvent(&stEvent)) {
        if (stEvent.type == SDL_QUIT)
          return 0;
        if (stEvent.type == SDL_KEYDOWN) {
          SDL_Keycode key = stEvent.key.keysym.sym;
          if (key == SDLK_DOWN) {
            gstWelcome.iSelected++;
            if (gstWelcome.iSelected >= gstWelcome.iNumItems)
              gstWelcome.iSelected = 0;
          }
          else if (key == SDLK_UP) {
            gstWelcome.iSelected--;
            if (gstWelcome.iSelected < 0)
              gstWelcome.iSelected = gstWelcome.iNumItems - 1;
          }
        }
        else if (stEvent.type == SDL_MOUSEBUTTONDOWN) {
          int mx = stEvent.button.x;
          int my = stEvent.button.y;
          int ii = 0;
          /* Detectar clique nos botoes */
          for ( ii = 0; ii < gstWelcome.iNumItems; ii++ ) {
            if ( bAreCoordsInSDL_Rect(&gstWelcome.aItems[ii].stRect, mx, my) ) {
              gstWelcome.iSelected = ii;
              break;
            }
          }

          if ( !strcasecmp(gstWelcome.aItems[gstWelcome.iSelected].szName, "Comecar Novo Jogo") ) {
            iWelcomeExit = 0;
            bRunning = FALSE;
            break;
          }
          else if ( !strcasecmp(gstWelcome.aItems[gstWelcome.iSelected].szName, "Sair") ) {
            iWelcomeExit = WELCOME_EXIT;
            bRunning = FALSE;
            break;
          }
          else {
            continue;
          }
        }
      }
    }

    gbWelcomeOpen = FALSE;

    return iWelcomeExit;
  }
#endif

