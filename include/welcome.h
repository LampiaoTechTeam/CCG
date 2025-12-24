#ifndef WELCOME_H
  #define WELCOME_H
  // #include <card_game.h>
  // #ifdef USE_SDL2
  //   #include <sdl_api.h>
  //   #include <SDL2/SDL.h>
  // #endif

  #define WELCOME_MAX_ITEMS 24
  #define WELCOME_COLS 4
  #define WELCOME_ROWS 3

  #define WELCOME_EXIT 1

  typedef struct {
    int iIdItem;
    char szName[64];
  #ifdef USE_SDL2
    SDL_Rect stRect;
  #endif
  } STRUCT_WELCOME_ITEM, *PSTRUCT_WELCOME_ITEM;

  typedef struct {
    STRUCT_WELCOME_ITEM aItems[WELCOME_MAX_ITEMS];
    int iNumItems;
    int iSelected;     /* índice do item selecionado */
    int iScroll;       /* para paginação (opcional) */
    int iPanelX;
    int iPanelY;
    int iPanelW;
    int iPanelH;
    int iCellW;
    int iCellH;
    int iCellPad;
    int iTitleH;
    int iFooterH;
    int iFontTitle;
    int iFontUi;
  } STRUCT_WELCOME, *PSTRUCT_WELCOME;

  extern int gbWelcomeOpen;

  int bLoadWelcome(void);
  #ifdef USE_SDL2
    void vSDL_WelcomeInit();
    int iWelcomeDraw(SDL_Renderer *pSDL_Renderer);
    int iSDL_OpenWelcome(SDL_Renderer *pSDL_Renderer);
  #endif
#endif /* _WELCOME_H_ */

