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
  #include <screen.h>
#endif
#include <input.h>
#include <shop.h>

int gbShopOpen = FALSE;

#ifdef USE_SDL2
  SDL_Rect gstButtonBuy;
  SDL_Rect gstButtonExit;

  void vSDL_ShopInit(PSTRUCT_SHOP pstShop){
    int ii;
    time_t lTime;
    if (pstShop == NULL)
      return;

    time(&lTime);
    srand(lTime);
    pstShop->iNumItems = 6;
    pstShop->iSelected = 0;
    for (ii = 0; ii < pstShop->iNumItems; ii++) {
      int iShopCard;
      iShopCard = (rand() % 4);
      if ( (iShopCard % 2) == 0 ){
        snprintf(pstShop->aItems[ii].szName, sizeof(pstShop->aItems[ii].szName), "Veneno %d", ii + 1);
        pstShop->aItems[ii].iPrice = SHOP_PRICE_POISON;
      }
      else{
        snprintf(pstShop->aItems[ii].szName, sizeof(pstShop->aItems[ii].szName), "Paralisar %d", ii + 1);
        pstShop->aItems[ii].iPrice = SHOP_PRICE_PARALIZE;
      }
      pstShop->aItems[ii].iTextureId = -1;
    }
  }

  /* Desenha o Shop completo */
  int iShopDraw(SDL_Renderer *pSDL_Renderer, PSTRUCT_SHOP pstShop, PSTRUCT_PLAYER pstPlayer){
    SDL_Rect stItem;
    SDL_Color stCorBranco = {255, 255, 255, 255};
    SDL_Color stCorAmarelo = {255, 215, 0, 255};
    char szBuffer[64] = "";
    int i = 0;
    int iX = 0;
    int iY = 0;
    PSTRUCT_ELEMENT pstPanel = NULL;
    PSTRUCT_ELEMENT pstTitle = NULL;
    PSTRUCT_ELEMENT pstBuyBtn = NULL;
    PSTRUCT_ELEMENT pstBuyBtnLabel = NULL;
    PSTRUCT_ELEMENT pstExitBtn = NULL;
    PSTRUCT_ELEMENT pstExitBtnLabel = NULL;
    PSTRUCT_ELEMENT pstGoldLabel = NULL;

    if (pSDL_Renderer == NULL || pstShop == NULL || pstPlayer == NULL)
      return -1;

    SDL_RenderClear(pSDL_Renderer);

    iSCREEN_SetLayout(LAYOUT_SHOP);

    /* Painel principal */
    pstPanel = pstSCREEN_GetElementByName("PANEL");
    if ( !pstPanel ) {
      vTraceVarArgsFn("Falha ao obter o elemento PANEL!");
      _exit(1);
    }

    vSDL_DrawRectShadow(pSDL_Renderer, (SDL_Rect*) &pstPanel->stRect, 6, 6, 80);
    SDL_SetRenderDrawColor(pSDL_Renderer, 40, 40, 50, 255);
    SDL_RenderFillRect(pSDL_Renderer, (SDL_Rect*) &pstPanel->stRect);
    SDL_SetRenderDrawColor(pSDL_Renderer, 220, 220, 230, 255);
    SDL_RenderDrawRect(pSDL_Renderer, (SDL_Rect*) &pstPanel->stRect);

    /* Título */
    pstTitle = pstSCREEN_GetElementByName("TITLE");
    vSDL_DrawText(pSDL_Renderer, pstTitle->szText, pstTitle->stRect.x, pstTitle->stRect.y, *(SDL_Color*)&pstTitle->stFgColor);

    /* Itens da loja */
    iX = pstPanel->stRect.x + 40;
    iY = pstPanel->stRect.y + 60;

    for (i = 0; i < pstShop->iNumItems; i++) {
      pstShop->aItems[i].stRect.x = iX;
      pstShop->aItems[i].stRect.y = iY + (i * 60);
      pstShop->aItems[i].stRect.w = pstPanel->stRect.w - 80;
      pstShop->aItems[i].stRect.h = 50;

      stItem = pstShop->aItems[i].stRect;

      /* Fundo do item */
      if (i == pstShop->iSelected)
        SDL_SetRenderDrawColor(pSDL_Renderer, 70, 90, 130, 255);
      else
        SDL_SetRenderDrawColor(pSDL_Renderer, 50, 50, 60, 255);
      SDL_RenderFillRect(pSDL_Renderer, &stItem);

      SDL_SetRenderDrawColor(pSDL_Renderer, 200, 200, 210, 255);
      SDL_RenderDrawRect(pSDL_Renderer, &stItem);

      /* Nome e preço */
      vSDL_DrawText(pSDL_Renderer, pstShop->aItems[i].szName, stItem.x + 10, stItem.y + 15, stCorBranco);
      snprintf(szBuffer, sizeof(szBuffer), "%d G", pstShop->aItems[i].iPrice);
      vSDL_DrawText(pSDL_Renderer, szBuffer, stItem.x + stItem.w - 80, stItem.y + 15, stCorAmarelo);
    }

    /* Botão BUY */
    pstBuyBtn = pstSCREEN_GetElementByName("BUY_BUTTON");
    gstButtonBuy.x = pstBuyBtn->stRect.x;
    gstButtonBuy.y = pstBuyBtn->stRect.y;
    gstButtonBuy.w = pstBuyBtn->stRect.w;
    gstButtonBuy.h = pstBuyBtn->stRect.h;
    vSDL_DrawRectShadow(pSDL_Renderer, &gstButtonBuy, 3, 3, 90);
    SDL_SetRenderDrawColor(pSDL_Renderer, 40, 120, 40, 255);
    SDL_RenderFillRect(pSDL_Renderer, &gstButtonBuy);
    SDL_RenderDrawRect(pSDL_Renderer, &gstButtonBuy);
    pstBuyBtnLabel = pstSCREEN_GetElementByName("BUY_LABEL");
    vSDL_DrawText(pSDL_Renderer, pstBuyBtnLabel->szText, pstBuyBtnLabel->stRect.x, pstBuyBtnLabel->stRect.y, *(SDL_Color*)&pstBuyBtnLabel->stFgColor);

    /* Botão EXIT */
    pstExitBtn = pstSCREEN_GetElementByName("EXIT_BUTTON");
    gstButtonExit.x = pstExitBtn->stRect.x;
    gstButtonExit.y = pstExitBtn->stRect.y;
    gstButtonExit.w = pstExitBtn->stRect.w;
    gstButtonExit.h = pstExitBtn->stRect.h;
    vSDL_DrawRectShadow(pSDL_Renderer, &gstButtonExit, 3, 3, 90);
    SDL_SetRenderDrawColor(pSDL_Renderer, 120, 40, 40, 255);
    SDL_RenderFillRect(pSDL_Renderer, &gstButtonExit);
    SDL_RenderDrawRect(pSDL_Renderer, &gstButtonExit);
    pstExitBtnLabel = pstSCREEN_GetElementByName("EXIT_LABEL");
    vSDL_DrawText(pSDL_Renderer, pstExitBtnLabel->szText, pstExitBtnLabel->stRect.x, pstExitBtnLabel->stRect.y, *(SDL_Color*)&pstExitBtnLabel->stFgColor);

    /* Ouro atual */
    pstGoldLabel = pstSCREEN_GetElementByName("Gold");
    snprintf(szBuffer, sizeof(szBuffer), "%s: %d", pstGoldLabel->szText, pstPlayer->iGold);
    vSDL_DrawText(pSDL_Renderer, szBuffer, pstGoldLabel->stRect.x, pstGoldLabel->stRect.y, *(SDL_Color*)&pstGoldLabel->stFgColor);
    
    SDL_RenderPresent(pSDL_Renderer);
    return 0;
  }
  int bHasAnyGoldToBuy(int iPlayerGold, STRUCT_SHOP *pstShop){
    int ii;
    if ( pstShop == NULL )
      return FALSE;

    for (ii = 0; ii < pstShop->iNumItems; ii++) {
      if (iPlayerGold >= pstShop->aItems[ii].iPrice)
        return TRUE;
    }
    return FALSE;
  }
  int iSDL_OpenShop(SDL_Renderer *pSDL_Renderer, PSTRUCT_PLAYER pstPlayer, PSTRUCT_DECK pstDeck){
    SDL_Event stEvent;
    int bRunning = FALSE;
    int iDoBuy = 0;
    int iDoExit = 0;
    STRUCT_SHOP stShop;
    STRUCT_CARD stNewCard;

    memset(&stEvent  , 0x00, sizeof(stEvent  ));
    memset(&stShop   , 0x00, sizeof(stShop   ));
    memset(&stNewCard, 0x00, sizeof(stNewCard));

    if (pSDL_Renderer == NULL || pstPlayer == NULL || pstDeck == NULL)
      return -1;

    iSCREEN_SetLayout(LAYOUT_SHOP);

    vSDL_ShopInit(&stShop);
    gbShopOpen = TRUE;
    bRunning = TRUE;

    while (bRunning) {
      /* desenha a tela */
      iShopDraw(pSDL_Renderer, &stShop, pstPlayer);

      iDoBuy = 0;
      iDoExit = 0;

      while (SDL_PollEvent(&stEvent)) {
        if (stEvent.type == SDL_QUIT) {
          return SHOP_EXIT;
        }
        if (stEvent.type == SDL_KEYDOWN) {
          SDL_Keycode key = stEvent.key.keysym.sym;
          if (key == SDLK_ESCAPE)
            iDoExit = 1;
          else if (key == SDLK_DOWN) {
            stShop.iSelected++;
            if (stShop.iSelected >= stShop.iNumItems)
              stShop.iSelected = 0;
          }
          else if (key == SDLK_UP) {
            stShop.iSelected--;
            if (stShop.iSelected < 0)
              stShop.iSelected = stShop.iNumItems - 1;
          }
          else if (key == SDLK_RETURN)
            iDoBuy = 1;
        }
        else if (stEvent.type == SDL_MOUSEBUTTONDOWN) {
          int mx = stEvent.button.x;
          int my = stEvent.button.y;
          int ii = 0;
          /* Detectar clique no botão BUY */

          if ( bAreCoordsInSDL_Rect(&gstButtonBuy, mx, my) ) {
            iDoBuy = 1;
          }

          if ( bAreCoordsInSDL_Rect(&gstButtonExit, mx, my) ) {
            iDoExit = 1;
          }

          for ( ii = 0; ii < stShop.iNumItems; ii++ ) {
            if ( bAreCoordsInSDL_Rect(&stShop.aItems[ii].stRect, mx, my) ) {
              stShop.iSelected = ii;
              break;
            }
          }
        }
      }

      /* ==== Lógica da compra ==== */
      if (iDoBuy) {
        int iPrice = stShop.aItems[stShop.iSelected].iPrice;
        if (pstPlayer->iGold < iPrice) {
          vTraceVarArgsFn("Shop: ouro insuficiente (tem %d, precisa de %d)", pstPlayer->iGold, iPrice);
          vSDL_DrawText(pSDL_Renderer, "Ouro insuficiente!", 320, 520, (SDL_Color){255,100,100,255});
          SDL_RenderPresent(pSDL_Renderer);
          vSleepSeconds(1);
        } else {
          pstPlayer->iGold -= iPrice;

          /* Cria carta baseada no item */
          if (strstr(stShop.aItems[stShop.iSelected].szName, "Veneno")) {
            stNewCard = stMakeCard(CARD_POISON, "Veneno", 1, 3, CARD_TARGET_SINGLE);
          } else {
            stNewCard = stMakeCard(CARD_PARALIZE, "Paralisar", 2, 'P', CARD_TARGET_SINGLE);
          }

          vAddCardToDiscard(pstDeck, stNewCard);
          vTraceVarArgsFn("Shop: comprou %s. Ouro restante=%d",
                          stNewCard.szName, pstPlayer->iGold);

          vSDL_DrawText(pSDL_Renderer, "Compra realizada!", 320, 520, (SDL_Color){180,255,180,255});
          SDL_RenderPresent(pSDL_Renderer);
          vSleepSeconds(1);
        }
      }
      if (iDoExit || !bHasAnyGoldToBuy(pstPlayer->iGold, &stShop) ) {
        bRunning = FALSE;
      }
    }
    gbShopOpen = FALSE;
    return 0;
  }
#endif
static void vPrintShopHeader(void) {
  char sz[128];
  vClearTerminal();
  vPrintHighlitedLine("===== SHOP =====", INSERT_NEW_LINE);
  snprintf(sz, sizeof(sz), "Ouro disponivel: %d", gstPlayer.iGold);
  vPrintLine(sz, INSERT_NEW_LINE);
  vPrintLine("1) Upgrade de uma carta (+2 valor, -0 custo) [20 ouro]", INSERT_NEW_LINE);
  vPrintLine("2) Comprar carta nova. [15 ouro]", INSERT_NEW_LINE);
  vPrintLine("q) Sair", INSERT_NEW_LINE);
}


static void vPrintNewCardOptions(void) {
  char sz[128];
  vClearTerminal();
  sprintf(sz, "1) Veneno    - %d gold", SHOP_PRICE_POISON);
  vPrintLine(sz, INSERT_NEW_LINE);
  sprintf(sz, "2) Paralisar - %d gold", SHOP_PRICE_PARALIZE);
  vPrintLine(sz, INSERT_NEW_LINE);
  vPrintLine("q) Sair", INSERT_NEW_LINE);
}

void vAddPlayerReward(PSTRUCT_PLAYER pstPlayer) {
  double dGoldRewarded = SHOP_STD_GOLD_AMOUNT_REWARD + (SHOP_GOLD_MULTIPLIYER * giLevel);
  pstPlayer->iGold += (int) dGoldRewarded;

  #ifdef USE_SDL2
    if ( !gbSDL_Mode)
      vPrintLine(" ", INSERT_NEW_LINE);
  #else
    vPrintLine(" ", INSERT_NEW_LINE);
  #endif
    
  {
    char szMsg[128];

    snprintf(szMsg, sizeof(szMsg),
  "*** Voce recebeu %d de gold para o nivel %d! ***", 
      (int) dGoldRewarded , 
      giLevel
    );
    vPrintHighlitedLine(szMsg, INSERT_NEW_LINE);
    #ifdef USE_SDL2
      if ( !gbSDL_Mode)
        vSleepSeconds(5);
    #else
      vSleepSeconds(5);
    #endif
  }
}

void vOpenShop(PSTRUCT_DECK pstDeck)
{
  int bLoop = TRUE;

  while (bLoop) {
    int iCh;
    int iNewCh;
    int iPrice;
    vPrintShopHeader();
    vPrintLine("\nEscolha uma opcao: ", NO_NEW_LINE);
    iCh = iPortableGetchar();

    if (iCh == 'q') break;
    
    if (iCh == '1') {
      char szWhich[_MAX_PATH];
      
      if (gstPlayer.iGold < SHOP_PRICE_UPGRADE) {
        vPrintLine("\nOuro insuficiente!", INSERT_NEW_LINE);
        vSleepSeconds(1);
        continue;
      }

      do {
        vClearTerminal();
        vPrintLine("\nDigite o nome exato da carta para upgrade (ex.: Strike, Defend, Heal, Fireball):", INSERT_NEW_LINE);
        vPrintLine("> ", NO_NEW_LINE);
        vReadCardName(szWhich, sizeof(szWhich));
      } while ( bStrIsEmpty(szWhich) );

      if ( iUpgradeFirstCardByName(pstDeck, szWhich, 2, 0) ) {
        gstPlayer.iGold -= SHOP_PRICE_UPGRADE;
        vTraceVarArgsFn("Shop: upgrade em '%s' aplicado. Ouro restante=%d", szWhich, gstPlayer.iGold);
        vPrintLine("Upgrade aplicado com sucesso!", INSERT_NEW_LINE);
      } else {
        vPrintLine("Carta nao encontrada em mao/draw/discard.", INSERT_NEW_LINE);
      }
      vSleepSeconds(1);
    } else if (iCh == '2') {
      STRUCT_CARD stNewCard;
      if (gstPlayer.iGold < SHOP_PRICE_POISON) {
        vPrintLine("\nOuro insuficiente!", INSERT_NEW_LINE);
        vSleepSeconds(1);
        continue;
      }
      do {
        vPrintNewCardOptions();
        vPrintLine("\nEscolha uma opcao: ", NO_NEW_LINE);
        iNewCh = iPortableGetchar();
      } while ( iNewCh != '1' && iNewCh != '2' && iNewCh != 'q');

      if ( iNewCh == '1'){
        stNewCard = stMakeCard(CARD_POISON,   "Veneno",    1,  3, CARD_TARGET_SINGLE);
        iPrice = SHOP_PRICE_POISON;
      }
      else if ( iNewCh == '2'){
        stNewCard = stMakeCard(CARD_PARALIZE, "Paralisar", 2, 'P', CARD_TARGET_SINGLE);
        iPrice = SHOP_PRICE_PARALIZE;
      }
      else 
        continue;
        
      vAddCardToDiscard(pstDeck, stNewCard);
      gstPlayer.iGold -= iPrice;
      vTraceVarArgsFn("Shop: comprou %s. Ouro restante=%d", stNewCard.szName, gstPlayer.iGold);
      vPrintLine("Nova carta adicionada ao descarte!", INSERT_NEW_LINE);
      vSleepSeconds(2);
      
    } else {
      vPrintLine("\nOpcao invalida.", INSERT_NEW_LINE);
      vSleepSeconds(1);
    }
  }
}
