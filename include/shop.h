#ifndef SHOP_H
  #define SHOP_H

  #define SHOP_PRICE_POISON            15
  #define SHOP_PRICE_PARALIZE          20
  
  #define SHOP_PRICE_UPGRADE           20
  
  #define SHOP_STD_GOLD_AMOUNT_REWARD  15
  #define SHOP_GOLD_MULTIPLIYER        1.2
  #define SHOP_MAX_ITEMS 24
  #define SHOP_COLS 4
  #define SHOP_ROWS 3

  #define SHOP_EXIT 1

  typedef struct {
    int iIdItem;
    char szName[64];
    int iPrice;
    int iStock;
    int iTextureId; /* ícone do item, já carregado em outro lugar */
  #ifdef USE_SDL2
    SDL_Rect stRect;
  #endif
  } STRUCT_SHOP_ITEM, *PSTRUCT_SHOP_ITEM;

  typedef struct {
    STRUCT_SHOP_ITEM aItems[SHOP_MAX_ITEMS];
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
    int iButtonBuyX;
    int iButtonBuyY;
    int iButtonBuyW;
    int iButtonBuyH;
    int iButtonBackX;
    int iButtonBackY;
    int iButtonBackW;
    int iButtonBackH;
    int iFontTitle;
    int iFontUi;
  } STRUCT_SHOP, *PSTRUCT_SHOP;

  extern int gbShopOpen;
 
  #ifdef USE_SDL2
    void vSDL_ShopInit(PSTRUCT_SHOP pstShop);
    int iShopDraw(SDL_Renderer *pSDL_Renderer, PSTRUCT_SHOP pstShop, PSTRUCT_PLAYER pstPlayer);
    int iSDL_OpenShop(SDL_Renderer *pSDL_Renderer, PSTRUCT_PLAYER pstPlayer, PSTRUCT_DECK pstDeck);
  #endif
  void vOpenShop(PSTRUCT_DECK pstDeck);
  void vAddPlayerReward(PSTRUCT_PLAYER pstPlayer);
#endif /* _SHOP_H_ */

