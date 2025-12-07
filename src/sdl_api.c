#ifdef USE_SDL2

#include <card_game.h>
#include <deck.h>
#include <debuff.h>
#include <monster.h>
#include <player.h>

#include <trace.h>
#include <battle.h>
#include <shop.h>
#include <dialog.h>
#include <input.h>
#include <terminal_utils.h>
#include <event.h>
#include <sdl_animation.h>
#include <sdl_api.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sdl_animation.h>
#include <game.h>

/* ---------------------------------------------  */
/*                   Locals                       */
/* ---------------------------------------------  */
static int giCardCount = 0;
static int giMonsterCount = 0;
static int giDlgTopIndex = 0; 
static int gbAnimateHandDraw = FALSE; 
static TTF_Font *gFont = NULL;
STRUCT_SDL_DIALOG_LAYOUT gstDialogLayout;

/* ---------------------------------------------  */
/*                   Globals                      */
/* ---------------------------------------------  */
int giSelectedMonster = -1;
int gbSelectingTarget = 0;
SDL_Rect gCardRects[64];
SDL_Rect gMonsterRects[64];

/* ---------------------------------------------  */
/*                   Functions                    */
/* ---------------------------------------------  */

void vSDL_DrawRectShadow(SDL_Renderer *pSDL_Renderer, SDL_Rect *pstRect, int iOffX, int iOffY, Uint8 uAlpha) {
  SDL_Rect stSh;
  if (pstRect == NULL)
    return;
  stSh = *pstRect;
  stSh.x += iOffX;
  stSh.y += iOffY;
  SET_RENDER_DRAW_COLOR(pSDL_Renderer, SDL_COLOR_FROM_RGB_OPACITY(SDL_RGB_BLACK, uAlpha));
  SDL_RenderFillRect(pSDL_Renderer, &stSh);
}

void vSDL_SetupMain(SDL_Renderer **pSDL_Renderer, SDL_Window **pSDL_Window){
  vSDL_MainInit();
  // Create a window
  *pSDL_Window = SDL_CreateWindow(
    "CCG",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    800,
    800,
    SDL_WINDOW_SHOWN
  );
  *pSDL_Renderer = SDL_CreateRenderer(*pSDL_Window, -1, SDL_RENDERER_ACCELERATED );
  SDL_SetRenderDrawBlendMode(*pSDL_Renderer, SDL_BLENDMODE_BLEND);
}

int iDlgMaybeFollowTail(int iVisibleCount) {
  int iWasAtEnd;
  int iMaxTop;

  if (iVisibleCount < 1)
    iVisibleCount = 1;

  iMaxTop = gstDlgList.iCount - iVisibleCount;
  if (iMaxTop < 0)
    iMaxTop = 0;

  iWasAtEnd = (gstDlgList.iTopIndex >= iMaxTop) ? 1 : 0;

  if (iWasAtEnd != 0) {
    gstDlgList.iTopIndex = iMaxTop;
    giDlgTopIndex = iMaxTop;
  }

  if (gstDlgList.iPinned != 0) {
    vClearPin();
  }

  vScrollToBottomInternal(iVisibleCount);

  return iWasAtEnd;
}

void vSDL_DrawTextShadow(SDL_Renderer *pSDL_Renderer,
                         const char *pszTxt,
                         int iX, int iY,
                         SDL_Color stFG,
                         SDL_Color stShadow,
                         int iOffX, int iOffY) {
  if (pszTxt == NULL || *pszTxt == '\0')
    return;

  vSDL_DrawText(pSDL_Renderer, pszTxt, iX + iOffX, iY + iOffY, stShadow);
  vSDL_DrawText(pSDL_Renderer, pszTxt, iX, iY, stFG);
}

static void vSDL_DialogScrollLines(int iDelta, int iVisible) {
  int iTotal;
  int iMaxTop;
  PSTRUCT_DIALOG pstWrk;

  pstWrk = gstDlgList.pstHead;
  iTotal = 0;
  while (pstWrk != NULL) {
    iTotal++;
    pstWrk = pstWrk->pstNext;
  }

  if (iVisible < 1)
    iVisible = 1;

  iMaxTop = (iTotal > iVisible) ? (iTotal - iVisible) : 0;

  giDlgTopIndex += iDelta;

  if (giDlgTopIndex < 0)
    giDlgTopIndex = 0;
  if (giDlgTopIndex > iMaxTop)
    giDlgTopIndex = iMaxTop;

  gstDlgList.iTopIndex = giDlgTopIndex;

  vTraceVarArgsFn(
"Scroll atualizado: TopIndex=%d / Total=%d / Visible=%d",
    giDlgTopIndex,
    iTotal,
    iVisible
  );
}

void vSDL_DialogHandleMouse(SDL_Event *pstEvt, int iX, int iY, int iW, int iH) {
  SDL_Rect stUp;
  SDL_Rect stDown;
  int iLineH;
  int iVisible;
  int iXMouse;
  int iYMouse;
  int bPressed;

  iLineH = 18;
  iVisible = (iH - 16) / iLineH; /* considera 8px topo + 8px base */
  if (iVisible < 1)
    iVisible = 1;

  stUp.x = iX + iW - 8 - 24;
  stUp.y = iY + 6;
  stUp.w = 24;
  stUp.h = 24;

  stDown.x = stUp.x;
  stDown.y = iY + iH - 6 - 24;
  stDown.w = 24;
  stDown.h = 24;

  if (pstEvt->type == SDL_MOUSEBUTTONDOWN || pstEvt->type == SDL_MOUSEBUTTONUP) {
    iXMouse = pstEvt->button.x;
    iYMouse = pstEvt->button.y;
    bPressed = (pstEvt->type == SDL_MOUSEBUTTONDOWN) ? 1 : 0;

    if (bPressed != 0) {
      if (bAreCoordsInSDL_Rect(&stUp, iXMouse, iYMouse)) {
        vSDL_DialogScrollLines(-1, iVisible);
      } else if (bAreCoordsInSDL_Rect(&stDown, iXMouse, iYMouse)) {
        vSDL_DialogScrollLines( 1, iVisible);
      }
    }
  }
}

void vSDL_DrawText(SDL_Renderer *pSDL_Renderer, const char *szTxt, int iX, int iY, SDL_Color stCol) {
  SDL_Surface *pSurf;
  SDL_Texture *pTex;
  SDL_Rect stDst;

  if (!gFont || !szTxt || !*szTxt)
    return;

  pSurf = TTF_RenderUTF8_Blended(gFont, szTxt, stCol);
  if (!pSurf)
    return;

  pTex = SDL_CreateTextureFromSurface(pSDL_Renderer, pSurf);
  if (pTex) {
    stDst.x = iX;
    stDst.y = iY;
    stDst.w = pSurf->w;
    stDst.h = pSurf->h;
    SDL_RenderCopy(pSDL_Renderer, pTex, NULL, &stDst);
    SDL_DestroyTexture(pTex);
  }
  SDL_FreeSurface(pSurf);
}

int bAreCoordsInSDL_Rect(SDL_Rect *pSDL_RECT, int iX, int iY) {
  if (pSDL_RECT == NULL)
    return FALSE;

  if (iX >= pSDL_RECT->x &&
      iY >= pSDL_RECT->y &&
      iX <= pSDL_RECT->x + pSDL_RECT->w &&
      iY <= pSDL_RECT->y + pSDL_RECT->h) {
    return TRUE;
  }
  return FALSE;
}


// /* -------------------------------------------------------------- */
// /* HUD / Dialog / Mesa */
// void vSDL_DrawDialog(SDL_Renderer *pSDL_Renderer, int iX, int iY, int iW, int iH) {
//   int iLineH;
//   int iBaseY;
//   int ii;
//   int iMaxLines;
//   int iTotal;
//   int iStart;
//   int iIdx;
//   char szGlyphUp[4];
//   char szGlyphDown[4];
//   SDL_Rect stRectDialog;
//   SDL_Rect stRectShadow;
//   SDL_Color stColorWhite;
//   SDL_Rect stUp;
//   SDL_Rect stDown;
//   SDL_Color stBtnBG;
//   SDL_Color stBtnFG;
//   SDL_Color stShadowTxt;
//   SDL_Color stShadowGlyph;
//   PSTRUCT_DIALOG pstWrk;

//   iLineH = 18;
//   iBaseY = iY + 8;
//   ii = 0;

//   stRectDialog.x = iX;
//   stRectDialog.y = iY;
//   stRectDialog.w = iW;
//   stRectDialog.h = iH;

//   stColorWhite.r = 255; stColorWhite.g = 255; stColorWhite.b = 255; stColorWhite.a = 255;

//   stBtnBG.r = 60;  stBtnBG.g = 60;  stBtnBG.b = 60;  stBtnBG.a = 220;
//   stBtnFG.r = 255; stBtnFG.g = 255; stBtnFG.b = 255; stBtnFG.a = 255;

//   stShadowTxt.r   = 0; stShadowTxt.g   = 0; stShadowTxt.b   = 0; stShadowTxt.a   = 180;
//   stShadowGlyph.r = 0; stShadowGlyph.g = 0; stShadowGlyph.b = 0; stShadowGlyph.a = 180;

//   /* --- painel com sombra (drop) --- */
//   stRectShadow = stRectDialog;
//   vSDL_DrawRectShadow(pSDL_Renderer, &stRectShadow, 3, 3, OPACITY_SEMI_TRANSPARENT);

//   SDL_SetRenderDrawColor(pSDL_Renderer, 40, 40, 40, 220);
//   SDL_RenderFillRect(pSDL_Renderer, &stRectDialog);
//   SDL_SetRenderDrawColor(pSDL_Renderer, 200, 200, 200, 255);
//   SDL_RenderDrawRect(pSDL_Renderer, &stRectDialog);

//   /* capacidade de linhas visíveis */
//   iMaxLines = iH / iLineH - 1;
//   if (iMaxLines < 1)
//     iMaxLines = 1;

//   /* contagem total */
//   iTotal = 0;
//   pstWrk = gstDlgList.pstHead;
//   while (pstWrk != NULL) {
//     iTotal++;
//     pstWrk = pstWrk->pstNext;
//   }

//   /* início baseado no índice global de rolagem */
//   if (giDlgTopIndex < 0)
//     giDlgTopIndex = 0;
//   if (giDlgTopIndex > (iTotal - iMaxLines))
//     giDlgTopIndex = (iTotal - iMaxLines) < 0 ? 0 : (iTotal - iMaxLines);
//   iStart = giDlgTopIndex;

//   /* pula até a primeira linha visível */
//   iIdx = 0;
//   pstWrk = gstDlgList.pstHead;
//   while (pstWrk != NULL && iIdx < iStart) {
//     pstWrk = pstWrk->pstNext;
//     iIdx++;
//   }

//   /* desenha linhas com sombra sutil */
//   while (pstWrk != NULL && ii < iMaxLines) {
//     char szLine[1024];
//     int iTx;
//     int iTy;

//     if (bStrIsEmpty(pstWrk->pszMsg) == 0) {
//       snprintf(szLine, sizeof(szLine), "%s - %s", pstWrk->szDT, pstWrk->pszMsg);
//       iTx = iX + 8;
//       iTy = iBaseY + ii * iLineH;
//       vSDL_DrawTextShadow(pSDL_Renderer, szLine, iTx, iTy, stColorWhite, stShadowTxt, 1, 1);
//       ii++;
//     }

//     pstWrk = pstWrk->pstNext;
//   }

//   /* --- setas de rolagem --- */
//   stUp.x = iX + iW - 8 - 24;
//   stUp.y = iY + 6;
//   stUp.w = 24;
//   stUp.h = 24;

//   stDown.x = stUp.x;
//   stDown.y = iY + iH - 6 - 24;
//   stDown.w = 24;
//   stDown.h = 24;

//   /* sombra dos botões (leve) */
//   vSDL_DrawRectShadow(pSDL_Renderer, &stUp,   2, 2, 100);
//   vSDL_DrawRectShadow(pSDL_Renderer, &stDown, 2, 2, 100);

//   SDL_SetRenderDrawColor(pSDL_Renderer, stBtnBG.r, stBtnBG.g, stBtnBG.b, stBtnBG.a);
//   SDL_RenderFillRect(pSDL_Renderer, &stUp);
//   SDL_RenderFillRect(pSDL_Renderer, &stDown);

//   SDL_SetRenderDrawColor(pSDL_Renderer, 180, 180, 180, 255);
//   SDL_RenderDrawRect(pSDL_Renderer, &stUp);
//   SDL_RenderDrawRect(pSDL_Renderer, &stDown);

//   /* glifos ▲ ▼ com sombra; fallback '^'/'v' se necessário */
//   szGlyphUp[0] = '\0';
//   szGlyphDown[0] = '\0';
//   strcpy(szGlyphUp,   "▲");
//   strcpy(szGlyphDown, "▼");

//   vSDL_DrawTextShadow(pSDL_Renderer, szGlyphUp,   stUp.x + 6,   stUp.y + 2,   stBtnFG, stShadowGlyph, 1, 1);
//   vSDL_DrawTextShadow(pSDL_Renderer, szGlyphDown, stDown.x + 6, stDown.y + 2, stBtnFG, stShadowGlyph, 1, 1);

//   /* desabilita setas quando no topo/fundo (efeito visual) */
//   if (giDlgTopIndex <= 0) {
//     SDL_SetRenderDrawColor(pSDL_Renderer, 0, 0, 0, 140);
//     SDL_RenderFillRect(pSDL_Renderer, &stUp);
//     SDL_SetRenderDrawColor(pSDL_Renderer, 180, 180, 180, 255);
//     SDL_RenderDrawRect(pSDL_Renderer, &stUp);
//     vSDL_DrawTextShadow(pSDL_Renderer, szGlyphUp, stUp.x + 6, stUp.y + 2,
//                         (SDL_Color){160,160,160,255}, (SDL_Color){0,0,0,120}, 1, 1);
//   }
//   if (giDlgTopIndex >= (iTotal - iMaxLines)) {
//     SDL_SetRenderDrawColor(pSDL_Renderer, 0, 0, 0, 140);
//     SDL_RenderFillRect(pSDL_Renderer, &stDown);
//     SDL_SetRenderDrawColor(pSDL_Renderer, 180, 180, 180, 255);
//     SDL_RenderDrawRect(pSDL_Renderer, &stDown);
//     vSDL_DrawTextShadow(pSDL_Renderer, szGlyphDown, stDown.x + 6, stDown.y + 2,
//                         (SDL_Color){160,160,160,255}, (SDL_Color){0,0,0,120}, 1, 1);
//   }
// }

void vSDL_DialogDraw(SDL_Renderer *pSDL_Renderer,
                     PSTRUCT_SDL_DIALOG_LAYOUT pstLayout) {
  SDL_Rect stRectDialog;
  SDL_Rect stRectShadow;
  SDL_Color stColPanel;
  SDL_Color stColBorder;
  SDL_Color stColWhite;
  SDL_Color stColShadowTxt;
  SDL_Color stColBtnBG;
  SDL_Color stColBtnFG;
  SDL_Color stColGlyphShadow;
  int iStartIndex;
  int iLine;
  PSTRUCT_DIALOG pstCur;

  if (pstLayout == NULL)
    return;

  stRectDialog.x = pstLayout->iX;
  stRectDialog.y = pstLayout->iY;
  stRectDialog.w = pstLayout->iW;
  stRectDialog.h = pstLayout->iH;

  stColPanel  = (SDL_Color){40, 40, 40, 220};
  stColBorder = (SDL_Color){200, 200, 200, 255};
  stColWhite  = (SDL_Color){255, 255, 255, 255};

  stColShadowTxt = (SDL_Color){0, 0, 0, 180};
  stColBtnBG     = (SDL_Color){60, 60, 60, 220};
  stColBtnFG     = (SDL_Color){255, 255, 255, 255};
  stColGlyphShadow = (SDL_Color){0, 0, 0, 180};

  /** Shadow */
  stRectShadow = stRectDialog;
  vSDL_DrawRectShadow(pSDL_Renderer, &stRectShadow, 3, 3, 150);

  /** Panel */
  SDL_SetRenderDrawColor(pSDL_Renderer,
                         stColPanel.r, stColPanel.g, stColPanel.b, stColPanel.a);
  SDL_RenderFillRect(pSDL_Renderer, &stRectDialog);

  SDL_SetRenderDrawColor(pSDL_Renderer,
                         stColBorder.r, stColBorder.g, stColBorder.b, stColBorder.a);
  SDL_RenderDrawRect(pSDL_Renderer, &stRectDialog);

  /** Total lines */
  iDlgGetCount();
  iStartIndex = iDlgGetTopIndex();

  pstCur = pstDlgGetAt(iStartIndex);

  /** Draw lines */
  for (iLine = 0; iLine < pstLayout->iVisibleLines; iLine++) {
    int iX = pstLayout->iX + 8;
    int iY = pstLayout->iY + 8 + iLine * pstLayout->iLineHeight;

    if (pstCur != NULL) {
      char szBuf[1024];
      snprintf(szBuf, sizeof(szBuf), "%s - %s",
               pstCur->szDT, pstCur->pszMsg);

      vSDL_DrawTextShadow(pSDL_Renderer,
                          szBuf, iX, iY,
                          stColWhite,
                          stColShadowTxt,
                          1, 1);

      pstCur = pstCur->pstNext;
    }
  }

  /** Buttons Up/Down */
  vSDL_DrawRectShadow(pSDL_Renderer, &pstLayout->stBtnUp,   2, 2, 100);
  vSDL_DrawRectShadow(pSDL_Renderer, &pstLayout->stBtnDown, 2, 2, 100);

  SDL_SetRenderDrawColor(pSDL_Renderer,
                         stColBtnBG.r, stColBtnBG.g, stColBtnBG.b, stColBtnBG.a);
  SDL_RenderFillRect(pSDL_Renderer, &pstLayout->stBtnUp);
  SDL_RenderFillRect(pSDL_Renderer, &pstLayout->stBtnDown);

  SDL_SetRenderDrawColor(pSDL_Renderer, 180, 180, 180, 255);
  SDL_RenderDrawRect(pSDL_Renderer, &pstLayout->stBtnUp);
  SDL_RenderDrawRect(pSDL_Renderer, &pstLayout->stBtnDown);

  vSDL_DrawTextShadow(pSDL_Renderer, "▲",
                      pstLayout->stBtnUp.x + 6,
                      pstLayout->stBtnUp.y + 2,
                      stColBtnFG, stColGlyphShadow, 1, 1);

  vSDL_DrawTextShadow(pSDL_Renderer, "▼",
                      pstLayout->stBtnDown.x + 6,
                      pstLayout->stBtnDown.y + 2,
                      stColBtnFG, stColGlyphShadow, 1, 1);
}

void vSDL_DialogComputeLayout(int iWinW, int iWinH, PSTRUCT_SDL_DIALOG_LAYOUT pstLayout) {
  int iMarginTop;
  int iMarginBottom;
  int iDlgY;
  int iDlgH;
  int iLineH;

  if (pstLayout == NULL)
    return;

  iLineH        = 18;
  iMarginTop    = 50 + 500 + 2;  /* posição do topo da mesa + margem */
  iMarginBottom = 2;

  iDlgY = iMarginTop;
  iDlgH = iWinH - iDlgY - iMarginBottom;
  if (iDlgH < 0)
    iDlgH = 0;

  pstLayout->iX = 50;
  pstLayout->iY = iDlgY;
  pstLayout->iW = 700;
  pstLayout->iH = iDlgH;

  pstLayout->iLineHeight   = iLineH;
  pstLayout->iVisibleLines = (iDlgH - 16) / iLineH;
  if (pstLayout->iVisibleLines < 1)
    pstLayout->iVisibleLines = 1;

  pstLayout->stBtnUp.x = pstLayout->iX + pstLayout->iW - 32;
  pstLayout->stBtnUp.y = pstLayout->iY + 6;
  pstLayout->stBtnUp.w = 24;
  pstLayout->stBtnUp.h = 24;

  pstLayout->stBtnDown.x = pstLayout->stBtnUp.x;
  pstLayout->stBtnDown.y = pstLayout->iY + pstLayout->iH - 30;
  pstLayout->stBtnDown.w = 24;
  pstLayout->stBtnDown.h = 24;
  (void)iWinW;
}

void vSDL_DrawHUD(SDL_Renderer *pSDL_Renderer, PSTRUCT_PLAYER pstPlayer) {
  SDL_Color stColHP;
  SDL_Color stColEnergy;
  SDL_Color stColBlock;
  SDL_Color stColShadow;
  char szHP[32];
  char szEnergy[32];
  char szBlock[32];
  SDL_Rect stRectChip;
  int iBaseX;
  int iY;
  int iGap;
  int iXHP;
  int iXEnergy;
  int iXBlock;
  int iChipW;
  int iChipH;
  int iChipPad;
  int iPaddingAdjust = 0;

  stColHP.r     = 220; stColHP.g     = 40;  stColHP.b     = 40;  stColHP.a     = 255;
  stColEnergy.r = 240; stColEnergy.g = 220; stColEnergy.b = 0;   stColEnergy.a = 255;
  stColBlock.r  = 40;  stColBlock.g  = 120; stColBlock.b  = 220; stColBlock.a  = 255;
  stColShadow.r = 0;   stColShadow.g = 0;   stColShadow.b = 0;   stColShadow.a = 255;

  sprintf(szHP,     "HP: %d", pstPlayer->iHP);
  sprintf(szEnergy, "Energia: %d", pstPlayer->iEnergy);
  sprintf(szBlock,  "Escudo: %d", pstPlayer->iBlock);

  iBaseX = 60;
  iY     = 20;
  iGap   = 200;

  iXHP     = iBaseX + 10;
  iXEnergy = iBaseX + iGap + 10;
  iXBlock  = iBaseX + 2 * iGap + 10;

  iChipW   = 12;
  iChipH   = 12;
  iChipPad =  6;

  iPaddingAdjust = iChipW + iChipPad;
  /* chip + texto HP */
  stRectChip.x = iXHP - iPaddingAdjust;
  stRectChip.y = iY + 2;
  stRectChip.w = iChipW;
  stRectChip.h = iChipH;
  
  SDL_SetRenderDrawColor(pSDL_Renderer, stColHP.r, stColHP.g, stColHP.b, stColHP.a);
  SDL_RenderFillRect(pSDL_Renderer, &stRectChip);
  vSDL_DrawTextShadow(pSDL_Renderer, szHP, iXHP, iY, stColHP, stColShadow, 1, 1);
  vSDL_DrawText(pSDL_Renderer, szHP, iXHP,     iY,     stColHP);

  /* chip + texto Energia */
  stRectChip.x = iXEnergy - iPaddingAdjust;
  SDL_SetRenderDrawColor(pSDL_Renderer, stColEnergy.r, stColEnergy.g, stColEnergy.b, stColEnergy.a);
  SDL_RenderFillRect(pSDL_Renderer, &stRectChip);
  vSDL_DrawTextShadow(pSDL_Renderer, szEnergy, iXEnergy, iY, stColEnergy, stColShadow, 1, 1);
  vSDL_DrawText(pSDL_Renderer, szEnergy, iXEnergy,     iY,     stColEnergy);

  /* chip + texto Escudo/Block */
  stRectChip.x = iXBlock - iPaddingAdjust;
  SDL_SetRenderDrawColor(pSDL_Renderer, stColBlock.r, stColBlock.g, stColBlock.b, stColBlock.a);
  SDL_RenderFillRect(pSDL_Renderer, &stRectChip);
  vSDL_DrawTextShadow(pSDL_Renderer, szBlock, iXBlock, iY, stColBlock, stColShadow, 1, 1);
  vSDL_DrawText(pSDL_Renderer, szBlock, iXBlock,     iY,     stColBlock);
}


void vSDL_DrawTable(SDL_Renderer *pSDL_Renderer, PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCt) {
  SDL_Rect stRectMesa;
  int ii;

  vTraceVarArgsFn("iMonsterCt=%d iHandCt=%d", iMonsterCt, pstDeck->iHandCount);

  stRectMesa.x = 50;
  stRectMesa.y = 50;
  stRectMesa.w = 700;
  stRectMesa.h = 500;

  /* Mesa verde, borda marrom */
  SET_RENDER_DRAW_COLOR(pSDL_Renderer, SDL_COLOR_FROM_RGB_OPACITY(SDL_RGB_TABLE_GREEN, OPACITY_OPAQUE));
  SDL_RenderFillRect(pSDL_Renderer, &stRectMesa);
  SET_RENDER_DRAW_COLOR(pSDL_Renderer, SDL_COLOR_FROM_RGB_OPACITY(SDL_RGB_TABLE_BORDER, OPACITY_OPAQUE));
  SDL_RenderDrawRect(pSDL_Renderer, &stRectMesa);

  /* --- Monstros --- */
  {
    int iPad;
    int iUsableW;
    int iSlotW;
    int iSlotH;
    
    SDL_Color stSDLColor;
    SDL_Color stShadow;

    iPad = 16;
    iUsableW = stRectMesa.w - iPad * 2;
    iSlotW = (iMonsterCt > 0) ? iUsableW / iMonsterCt : iUsableW;
    iSlotH = 80;

    stSDLColor = (SDL_Color){255, 255, 255, 255};
    stShadow = (SDL_Color){0, 0, 0, 180};

    for (ii = 0; ii < iMonsterCt; ++ii) {
      SDL_Rect stRectMonster;
      PSTRUCT_MONSTER pstMonster;
      char szLine1[128];
      char szLine2[128];
      char szStatus[128];
      const char *szName;
      int iHP;
      int iAtk;
      int iBlock;
      int iPoison = 0;
      int iPoisonLeft = 0;
      int iParalyzeLeft = 0;
      int iCols;
      int iRow;
      int iCol;
      int jj = 0;

      pstMonster = &pastMonsters[ii];

      if (pstMonster->iHP <= 0)
        continue;

      /* --- Definição de colunas e linhas --- */
      if (iMonsterCt <= 3) {
        iCols = iMonsterCt;
      } else if (iMonsterCt <= 6) {
        iCols = 3;
      } else if (iMonsterCt <= 9) {
        iCols = 3;
      } else {
        iCols = 4;
      }

      iCol = ii % iCols;
      iRow = ii / iCols;

      /* --- Ajuste do tamanho dos retângulos --- */
      if (iMonsterCt <= 3) {
        iSlotW = (iUsableW / iCols) - 8;
        iSlotH = 80;
      } else if (iMonsterCt <= 6) {
        iSlotW = (iUsableW / iCols) - 8;
        iSlotH = 80;
      } else if (iMonsterCt <= 9) {
        iSlotW = (iUsableW / iCols) - 8;
        iSlotH = 70;
      } else {
        iSlotW = (iUsableW / iCols) - 8;
        iSlotH = 60;
      }

      /* --- Coordenadas finais --- */
      stRectMonster.x = stRectMesa.x + iPad + iCol * (iSlotW + iPad);
      stRectMonster.y = stRectMesa.y + iPad + iRow * (iSlotH + iPad);

      /* pequeno ajuste visual quando há 3+ linhas */
      if (iMonsterCt > 6)
        stRectMonster.y -= 8;

      stRectMonster.w = iSlotW - 16;
      stRectMonster.h = iSlotH;

      /* --- Desenho base do monstro --- */
      vSDL_DrawRectShadow(pSDL_Renderer, &stRectMonster, 2, 2, OPACITY_SEMI_OPAQUE);
      SET_RENDER_DRAW_COLOR(pSDL_Renderer, SDL_COLOR_FROM_RGB_OPACITY(SDL_RGB_RED_BLOOD, OPACITY_OPAQUE));
      SDL_RenderFillRect(pSDL_Renderer, &stRectMonster);
      SET_RENDER_DRAW_COLOR(pSDL_Renderer, SDL_COLOR_FROM_RGB_OPACITY(SDL_RGB_BLACK, OPACITY_OPAQUE));
      SDL_RenderDrawRect(pSDL_Renderer, &stRectMonster);

      /* --- Texto principal --- */
      szName = pstMonster->szName;
      iHP = pstMonster->iHP;
      iAtk = pstMonster->iAttack;
      iBlock = pstMonster->iBlock;
      
      snprintf(szLine1, sizeof(szLine1), "%s", szName ? szName : "Monster");
      snprintf(szLine2, sizeof(szLine2), "HP:%d E:%d Dano:%d", iHP,  iBlock, iAtk);

      vSDL_DrawTextShadow(pSDL_Renderer, szLine1,
                          stRectMonster.x + 8, stRectMonster.y + 8,
                          stSDLColor, stShadow, 1, 1);
      vSDL_DrawTextShadow(pSDL_Renderer, szLine2,
                          stRectMonster.x + 8, stRectMonster.y + 28,
                          stSDLColor, stShadow, 1, 1);

      /* --- Status adicionais (veneno, paralyze) --- */
      while (jj < pstMonster->iDebuffCt) {
        PSTRUCT_DEBUFF pstDebuff;
        pstDebuff = &pstMonster->astDebuff[jj];
        if (pstDebuff->iType == DEBUFF_TYPE_POISON) {
          iPoisonLeft += pstDebuff->iRounds;
          if (iPoisonLeft > 0)
            iPoison += pstDebuff->iDamage;
        } else if (pstDebuff->iType == DEBUFF_TYPE_PARALYZE) {
          iParalyzeLeft += pstDebuff->iRounds;
        }
        jj++;
      }
      
      memset(szStatus, 0, sizeof(szStatus));
      if (iPoisonLeft > 0) {
        char szTmp[64];
        int iLen = strlen(szStatus) + 1;
        snprintf(szTmp, sizeof(szTmp), "V:%d Rest:%d ", iPoison, iPoisonLeft);
        strncat(szStatus, szTmp, sizeof(szStatus) - iLen);
      }     
      if (iParalyzeLeft > 0) {
        char szTmp[64];
        int iLen = strlen(szStatus) + 1;
        snprintf(szTmp, sizeof(szTmp), "Prlz:%d ", iParalyzeLeft);
        strncat(szStatus, szTmp, sizeof(szStatus) - iLen);
      }

      if (strlen(szStatus) > 0) {
        vTraceVarArgsFn("Debuff Line [%s]", szStatus);
        vSDL_DrawTextShadow(pSDL_Renderer, szStatus,
                            stRectMonster.x + 8,
                            stRectMonster.y + 50,
                            stSDLColor, stShadow, 1, 1);
      }

      if (ii < (int)(sizeof(gMonsterRects) / sizeof(gMonsterRects[0])))
        gMonsterRects[ii] = stRectMonster;
    }

    giMonsterCount = iMonsterCt;
  }



  /* --- Cartas do jogador --- */
  giCardCount = 0;
  if (pstDeck->iHandCount > 0) {
    int iPadX;
    int iUsableW;
    int iSlotW;
    SDL_Color stSDLColor;

    iPadX = 24;
    iUsableW = stRectMesa.w - iPadX * 2;
    iSlotW = iUsableW / pstDeck->iHandCount;
    stSDLColor = (SDL_Color){0, 0, 0, 255};

    for (ii = 0; ii < pstDeck->iHandCount; ii++) {
      SDL_Rect stRectCard;
      char szLine1[64];
      char szLine2[64];
      int iTx;
      int iTy;  

      stRectCard.w = 80;
      stRectCard.h = 112;
      stRectCard.x = stRectMesa.x + iPadX + ii * iSlotW + (iSlotW - stRectCard.w) / 2;
      stRectCard.y = stRectMesa.y + stRectMesa.h - stRectCard.h - 20;

      if (ii < (int)(sizeof(gCardRects) / sizeof(gCardRects[0])))
        gCardRects[ii] = stRectCard;
      
      if ( ii < MAX_HAND ) {
        int iR = 200;
        int iG = 200; 
        int iB = 200;
     
        int iPadLen = (8 - strlen(pstDeck->astHand[ii].szName)) / 2;
        if ( iPadLen < 0 ) iPadLen = 0;
        snprintf(szLine1, sizeof(szLine1), "%*.*s%s%*.*s", iPadLen, iPadLen, " ", pstDeck->astHand[ii].szName, iPadLen, iPadLen, " ");
        snprintf(szLine2, sizeof(szLine2), "E:%d V:%d", pstDeck->astHand[ii].iCost, pstDeck->astHand[ii].iValue);
        iTx = stRectCard.x;
        iTy = stRectCard.y + 30;
        SDL_SetRenderDrawColor(pSDL_Renderer, iR, iG, iB, 255);
        SDL_RenderFillRect(pSDL_Renderer, &stRectCard);
        SDL_SetRenderDrawColor(pSDL_Renderer, 30, 30, 30, 255);
        SDL_RenderDrawRect(pSDL_Renderer, &stRectCard);
        vSDL_DrawText(pSDL_Renderer, szLine1, iTx, iTy, stSDLColor);
        iTy += 21;
        vSDL_DrawText(pSDL_Renderer, szLine2, iTx, iTy, stSDLColor);
      }
      giCardCount++;
    }
  }
}

/* -------------------------------------------------------------- */
/* Inicialização e loop principal */

void vSDL_MainInit(void) {
  SDL_SetMainReady();

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    if (DEBUG_MSGS)
      vTraceVarArgsFn("Couldn't initialize SDL: %s", SDL_GetError());
    return;
  }

  /* Inicializa TTF (se ainda não) */
  if (TTF_WasInit() == 0) {
    if (TTF_Init() != 0)
      vTraceVarArgsFn("Erro TTF_Init: %s", TTF_GetError());
  }

  gFont = ttfSDL_InitFont();
  if (!gFont)
    vTraceVarArgsFn("Erro ao carregar fonte: %s", TTF_GetError());

}

int iHitTestMonster(int iX, int iY) {
  int ii;
  for (ii = 0; ii < giMonsterCount; ii++) {
    if (bAreCoordsInSDL_Rect(&gMonsterRects[ii], iX, iY)) {
      return ii;
    }
  }
  return -1;
}

/* Alterna fullscreen/windowed */
void vSDL_ToggleFullscreen(void) {
  SDL_Window *pWindow;
  Uint32 uFlags;

  pWindow = SDL_GetWindowFromID(1);
  if (pWindow == NULL)
    return;

  uFlags = SDL_GetWindowFlags(pWindow);
  if ((uFlags & SDL_WINDOW_FULLSCREEN) != 0)
    SDL_SetWindowFullscreen(pWindow, 0);
  else
    SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_FULLSCREEN);
}

int iSDL_MonsterIndexFromPoint(int iX, int iY, PSTRUCT_MONSTER pastMonsters, int iMonsterCt) {
  int iIdx;

  if (pastMonsters == NULL || iMonsterCt <= 0)
    return -1;

  for (iIdx = 0; iIdx < iMonsterCt; iIdx++) {
    if (pastMonsters[iIdx].iHP <= 0)
      continue;  /* morto não é clicável */

    if (bAreCoordsInSDL_Rect(&gMonsterRects[iIdx], iX, iY)) {
      return iIdx;
    }
  }
  return -1;
}

int iSDL_HandIndexFromPoint(int iX, int iY, PSTRUCT_DECK pstDeck) {
  int iIdx;
  int iHandCt;

  if (pstDeck == NULL)
    return -1;

  iHandCt = pstDeck->iHandCount;

  for (iIdx = 0; iIdx < iHandCt; iIdx++) {
    if (bAreCoordsInSDL_Rect(&gCardRects[iIdx], iX, iY)) {
      return iIdx;
    }
  }
  return -1;
}
int iSDL_DialogHandleMouse(SDL_Event *pEv, PSTRUCT_SDL_DIALOG_LAYOUT pLayout) {
  int iX = pEv->button.x;
  int iY = pEv->button.y;

  /* Clique em cima: scroll up */
  if ( bAreCoordsInSDL_Rect(&pLayout->stBtnUp, iX, iY) ){
    vDlgScrollLines(-1, pLayout->iVisibleLines);
    return REDRAW_DIALOG;
  }

  /* Clique em baixo: scroll down */
  if ( bAreCoordsInSDL_Rect(&pLayout->stBtnDown, iX, iY) ) {
    vDlgScrollLines(1, pLayout->iVisibleLines);
    return REDRAW_DIALOG;
  }

  return REDRAW_NONE;
}

void vRedraw(SDL_Renderer *pSDL_Renderer,
             int iRedrawAction,
             PSTRUCT_DECK pstDeck,
             PSTRUCT_MONSTER pastMonsters,
             int iMonsterCt) {
  int bRedrawDialog;
  int bRedrawTable;

  bRedrawDialog = (iRedrawAction & REDRAW_DIALOG) != 0 ;
  bRedrawTable  = (iRedrawAction & REDRAW_TABLE)  != 0 ;

  vTraceVarArgsFn("bRedrawDialog=%d bRedrawTable=%d gbAnimateHandDraw=%d", bRedrawDialog, bRedrawTable, gbAnimateHandDraw);

  if (bRedrawDialog == 0 && bRedrawTable == 0)
    return;

  if (bRedrawTable) {
    SDL_RenderClear(pSDL_Renderer);
    vSDL_DrawTable(pSDL_Renderer, pstDeck, pastMonsters, iMonsterCt);
    vSDL_DrawHUD(pSDL_Renderer, &gstPlayer);
  }

  if (bRedrawDialog) {
    int iWinW;
    int iWinH;
    SDL_GetRendererOutputSize(pSDL_Renderer, &iWinW, &iWinH);
    vSDL_DialogComputeLayout(iWinW, iWinH, &gstDialogLayout);
    vSDL_DialogDraw(pSDL_Renderer, &gstDialogLayout);
  }

  // if ( gbAnimateHandDraw )
    // vAnimateFlipHand(pSDL_Renderer, pstDeck);
  
    
  SDL_RenderPresent(pSDL_Renderer);
  if ( gbAnimateHandDraw ){
    gbAnimateHandDraw = FALSE;
  }

}

void vSDL_MainLoop(int *pbRunning, SDL_Event *pSDL_Event, SDL_Renderer *pSDL_Renderer, PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCt) {
  int iRedrawAction = REDRAW_ALL;
  int bHasPlayableCards = FALSE;
  uint64_t ui64FrameStart;
  uint64_t ui64FrameTime;

  vTraceVarArgsFn(" --- SDL MAIN LOOP");
  
  gbAnimateHandDraw = TRUE;

  /* ------------------------------ */
  /*          MAIN LOOP             */
  /* ------------------------------ */
  while (*pbRunning) {

    ui64FrameStart = SDL_GetTicks64();

    while (SDL_PollEvent(pSDL_Event)) {
      iRedrawAction |= iEVENT_HandlePollEv(
                          pSDL_Event,
                          iRedrawAction,
                          pSDL_Renderer,
                          pstDeck,
                          pastMonsters,
                          iMonsterCt,
                          pbRunning
                        );
    }
    

    /** Are we leaving ? */
    if ( !(*pbRunning) )
      break;

    if ( gstGame.iStatus == STATUS_PAUSE ) {
      vRedraw(pSDL_Renderer, iRedrawAction, pstDeck, pastMonsters, iMonsterCt);
      iRedrawAction = REDRAW_NONE;
      continue;
    }
    
    if ( iRedrawAction == -2 )
      continue;
    
    if (!iAnyMonsterAlive(pastMonsters, iMonsterCt)) {
      char szMsg[128] = "";
      memset(szMsg, 0x00, sizeof(szMsg));
      snprintf(szMsg, sizeof(szMsg), "*** Nivel %d completo! ***", giLevel);
      vPrintLine(szMsg, NO_NEW_LINE);
      vRedraw(pSDL_Renderer, REDRAW_DIALOG, pstDeck, pastMonsters, iMonsterCt);
      // SDL_Delay(16);
      iRedrawAction = REDRAW_ALL;
      vAddPlayerReward(&gstPlayer);
      iSDL_OpenShop(pSDL_Renderer, &gstPlayer, pstDeck);
      giLevel++;
      vInitMonstersForLevel(pastMonsters, giLevel, &iMonsterCt);
      vStartNewTurn(pstDeck);
      vTraceDeck(pstDeck, TRACE_DECK_ALL);
      gbAnimateHandDraw = TRUE;
    }
    /** Checks for enemy turn */
    else if (gstPlayer.iEnergy <= 0 || !(bHasPlayableCards = bHasAnyPlayableCard(pstDeck))) {
      vTraceVarArgsFn("Player Energy=[%d] | Got any playable card?=%d", gstPlayer.iEnergy, bHasPlayableCards);
      
      /** Refresh enemy statuses */
      vRedraw(pSDL_Renderer, REDRAW_ALL, pstDeck, pastMonsters, iMonsterCt);

      vDoEnemyActions(pastMonsters, iMonsterCt);
      vPrintLine("Iniciando novo turno, aguarde...", INSERT_NEW_LINE);
      // vRedraw(pSDL_Renderer, REDRAW_ALL, pstDeck, pastMonsters, iMonsterCt);
      // SDL_Delay(8);
      vRedraw(pSDL_Renderer, REDRAW_ALL, pstDeck, pastMonsters, iMonsterCt);
      iRedrawAction = REDRAW_NONE;

      /** Is player dead? */
      if (gstPlayer.iHP <= 0) {
        vPrintLine("Voce morreu!", INSERT_NEW_LINE);
        vRedraw(pSDL_Renderer, REDRAW_ALL, pstDeck, pastMonsters, iMonsterCt);
        iRedrawAction = REDRAW_NONE;
        SDL_Delay(2500);
        *pbRunning = FALSE;
        break;
      }

      /** Player still alive, initialize next turn */
      vStartNewTurn(pstDeck);
      vTraceDeck(pstDeck, TRACE_DECK_ALL);
      gbAnimateHandDraw = TRUE;

      iRedrawAction = REDRAW_ALL;
    }

    /** Should we rewdraw? */
    if (iRedrawAction != REDRAW_NONE) 
      vRedraw(pSDL_Renderer, iRedrawAction, pstDeck, pastMonsters, iMonsterCt);
    
    /** Reset redraw flag */
    iRedrawAction = REDRAW_NONE;

    /** Sync em 60Hz */
    ui64FrameTime = SDL_GetTicks64() - ui64FrameStart;
    if (ui64FrameTime < VSYNC_TIME) {
      SDL_Delay(VSYNC_TIME - ui64FrameTime);
    }
  }

  vTraceVarArgsFn(" --- SDL MAIN LOOP END");
}

/* -------------------------------------------------------------- */
/* Quit */

void vSDL_MainQuit(void) {
  vTraceVarArgsFn(" -- Begin");

  if (gFont) {
    TTF_CloseFont(gFont);
    gFont = NULL;
  }

  TTF_Quit();
  SDL_Quit();

  vTraceVarArgsFn(" -- End ");
}

#endif /* USE_SDL2 */
