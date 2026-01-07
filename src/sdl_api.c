/**
 * sdl_api.c
 *
 * Written by Renato Fermi <repiazza@gmail.com>
 *
 * Description: SDL API
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
#ifdef USE_SDL2
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
  #include <SDL2/SDL_ttf.h>
#endif
#include <player.h>
#include <welcome.h>

#include <trace.h>
#include <battle.h>
#include <sdl_api.h>
#include <shop.h>
#include <dialog.h>
#include <input.h>
#include <terminal_utils.h>
#include <event.h>
#include <sdl_animation.h>
#include <image.h>
#include <event_render.h>
#include <game.h>
#include <screen.h>
#include <msg.h>

#if SDL_MAJOR_VERSION >=2 && SDL_MINOR_VERSION >= 0 && SDL_PATCHLEVEL >= 18
  #define u64SDL_GetTicks SDL_GetTicks64
#else
  #define u64SDL_GetTicks SDL_GetTicks
#endif
/* ---------------------------------------------  */
/*                   Locals                       */
/* ---------------------------------------------  */
static int giCardCount = 0;
static int giMonsterCount = 0;
static int giDlgTopIndex = 0;
static TTF_Font *gFont = NULL;
STRUCT_SDL_DIALOG_LAYOUT gstDialogLayout;

/* ---------------------------------------------  */
/*                   Globals                      */
/* ---------------------------------------------  */
int giSelectedMonster = -1;
int gbSelectingTarget = 0;
SDL_Rect gCardRects[64];
SDL_Rect gMonsterRects[64];
static SDL_Renderer *gpSDL_Renderer = NULL;
static SDL_Window   *gpSDL_Window   = NULL;

const SDLEventName gEventNames[] = {
  { SDL_FIRSTEVENT, "SDL_FIRSTEVENT" },
  { SDL_QUIT, "SDL_QUIT" },
  { SDL_APP_TERMINATING, "SDL_APP_TERMINATING" },
  { SDL_APP_LOWMEMORY, "SDL_APP_LOWMEMORY" },
  { SDL_APP_WILLENTERBACKGROUND, "SDL_APP_WILLENTERBACKGROUND" },
  { SDL_APP_DIDENTERBACKGROUND, "SDL_APP_DIDENTERBACKGROUND" },
  { SDL_APP_WILLENTERFOREGROUND, "SDL_APP_WILLENTERFOREGROUND" },
  { SDL_APP_DIDENTERFOREGROUND, "SDL_APP_DIDENTERFOREGROUND" },
  #ifdef SDL_LOCALECHANGED
  { SDL_LOCALECHANGED, "SDL_LOCALECHANGED" },
  #endif
  #ifdef SDL_DISPLAYEVENT
  { SDL_DISPLAYEVENT, "SDL_DISPLAYEVENT" },
  #endif
  { SDL_WINDOWEVENT, "SDL_WINDOWEVENT" },
  { SDL_SYSWMEVENT, "SDL_SYSWMEVENT" },
  { SDL_KEYDOWN, "SDL_KEYDOWN" },
  { SDL_KEYUP, "SDL_KEYUP" },
  { SDL_TEXTEDITING, "SDL_TEXTEDITING" },
  { SDL_TEXTINPUT, "SDL_TEXTINPUT" },
  { SDL_KEYMAPCHANGED, "SDL_KEYMAPCHANGED" },
  #ifdef SDL_TEXTEDITING_EXT
  { SDL_TEXTEDITING_EXT, "SDL_TEXTEDITING_EXT" },
  #endif
  { SDL_MOUSEMOTION, "SDL_MOUSEMOTION" },
  { SDL_MOUSEBUTTONDOWN, "SDL_MOUSEBUTTONDOWN" },
  { SDL_MOUSEBUTTONUP, "SDL_MOUSEBUTTONUP" },
  { SDL_MOUSEWHEEL, "SDL_MOUSEWHEEL" },
  { SDL_JOYAXISMOTION, "SDL_JOYAXISMOTION" },
  { SDL_JOYBALLMOTION, "SDL_JOYBALLMOTION" },
  { SDL_JOYHATMOTION, "SDL_JOYHATMOTION" },
  { SDL_JOYBUTTONDOWN, "SDL_JOYBUTTONDOWN" },
  { SDL_JOYBUTTONUP, "SDL_JOYBUTTONUP" },
  { SDL_JOYDEVICEADDED, "SDL_JOYDEVICEADDED" },
  { SDL_JOYDEVICEREMOVED, "SDL_JOYDEVICEREMOVED" },
  #ifdef SDL_JOYBATTERYUPDATED
  { SDL_JOYBATTERYUPDATED, "SDL_JOYBATTERYUPDATED" },
  #endif
  { SDL_CONTROLLERAXISMOTION, "SDL_CONTROLLERAXISMOTION" },
  { SDL_CONTROLLERBUTTONDOWN, "SDL_CONTROLLERBUTTONDOWN" },
  { SDL_CONTROLLERBUTTONUP, "SDL_CONTROLLERBUTTONUP" },
  { SDL_CONTROLLERDEVICEADDED, "SDL_CONTROLLERDEVICEADDED" },
  { SDL_CONTROLLERDEVICEREMOVED, "SDL_CONTROLLERDEVICEREMOVED" },
  { SDL_CONTROLLERDEVICEREMAPPED, "SDL_CONTROLLERDEVICEREMAPPED" },
  #ifdef SDL_CONTROLLERTOUCHPADDOWN
  { SDL_CONTROLLERTOUCHPADDOWN, "SDL_CONTROLLERTOUCHPADDOWN" },
  #endif
  #ifdef SDL_CONTROLLERTOUCHPADMOTION
  { SDL_CONTROLLERTOUCHPADMOTION, "SDL_CONTROLLERTOUCHPADMOTION" },
  #endif
  #ifdef SDL_CONTROLLERTOUCHPADUP
  { SDL_CONTROLLERTOUCHPADUP, "SDL_CONTROLLERTOUCHPADUP" },
  #endif
  #ifdef SDL_CONTROLLERSENSORUPDATE
  { SDL_CONTROLLERSENSORUPDATE, "SDL_CONTROLLERSENSORUPDATE" },
  #endif
  #ifdef SDL_CONTROLLERUPDATECOMPLETE_RESERVED_FOR_SDL3
  { SDL_CONTROLLERUPDATECOMPLETE_RESERVED_FOR_SDL3, "SDL_CONTROLLERUPDATECOMPLETE_RESERVED_FOR_SDL3" },
  #endif
  #ifdef SDL_CONTROLLERSTEAMHANDLEUPDATED
  { SDL_CONTROLLERSTEAMHANDLEUPDATED, "SDL_CONTROLLERSTEAMHANDLEUPDATED" },
  #endif
  { SDL_FINGERDOWN,   "SDL_FINGERDOWN" },
  { SDL_FINGERUP,     "SDL_FINGERUP" },
  { SDL_FINGERMOTION, "SDL_FINGERMOTION" },
  { SDL_DOLLARGESTURE,"SDL_DOLLARGESTURE" },
  { SDL_DOLLARRECORD, "SDL_DOLLARRECORD" },
  { SDL_MULTIGESTURE, "SDL_MULTIGESTURE" },
  { SDL_CLIPBOARDUPDATE, "SDL_CLIPBOARDUPDATE" },
  { SDL_DROPFILE,        "SDL_DROPFILE" },
  #ifdef SDL_DROPTEXT
  { SDL_DROPTEXT,        "SDL_DROPTEXT" },
  #endif
  #ifdef SDL_DROPBEGIN
  { SDL_DROPBEGIN,       "SDL_DROPBEGIN" },
  #endif
  #ifdef SDL_DROPCOMPLETE
  { SDL_DROPCOMPLETE,    "SDL_DROPCOMPLETE" },
  #endif
  { SDL_AUDIODEVICEADDED,"SDL_AUDIODEVICEADDED" },
  { SDL_AUDIODEVICEREMOVED, "SDL_AUDIODEVICEREMOVED" },
  #ifdef SDL_SENSORUPDATE
  { SDL_SENSORUPDATE,    "SDL_SENSORUPDATE" },
  #endif
  { SDL_RENDER_TARGETS_RESET, "SDL_RENDER_TARGETS_RESET" },
  { SDL_RENDER_DEVICE_RESET, "SDL_RENDER_DEVICE_RESET" },
  #ifdef SDL_POLLSENTINEL
  { SDL_POLLSENTINEL,    "SDL_POLLSENTINEL" },
  #endif
  { SDL_USEREVENT,       "SDL_USEREVENT" },   // base
  { SDL_LASTEVENT,       "SDL_LASTEVENT" }
};

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

  *pSDL_Window = SDL_CreateWindow(
    "CCG",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    gstScreen.iWindowWidth,
    gstScreen.iWindowHeight,
    SDL_WINDOW_SHOWN
  );
  *pSDL_Renderer = SDL_CreateRenderer(*pSDL_Window, -1, SDL_RENDERER_ACCELERATED );

  if ( iIMG_LoadAll(*pSDL_Renderer) == FALSE )
    if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn("Imagens não inicializadas (iIMG_LoadAll falhou)");

  SDL_SetRenderDrawBlendMode(*pSDL_Renderer, SDL_BLENDMODE_BLEND);

  gpSDL_Renderer = *pSDL_Renderer;
  gpSDL_Window   = *pSDL_Window;
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

/* Calcula o retângulo final exatamente como o RenderScaled usa (scale + keep-aspect) */
static int iIMG_ComputeScaledRect(SDL_Rect *pstDst,
                                double dScale,
                                int bKeepAspect,
                                SDL_Rect *pstOutFinal) {
  SDL_Rect stDstScaled;
  SDL_Rect stDstFinal;
  int iCX;
  int iCY;
  int iNewW;
  int iNewH;
  double dAspectSrc;
  double dAspectDst;

  if (pstDst == NULL || pstOutFinal == NULL)
    return FALSE;

  if (dScale <= 0.0)
    dScale = 1.0;

  iCX = pstDst->x + pstDst->w / 2;
  iCY = pstDst->y + pstDst->h / 2;

  iNewW = (int)((double)pstDst->w * dScale);
  iNewH = (int)((double)pstDst->h * dScale);

  stDstScaled.w = iNewW;
  stDstScaled.h = iNewH;
  stDstScaled.x = iCX - stDstScaled.w / 2;
  stDstScaled.y = iCY - stDstScaled.h / 2;

  stDstFinal = stDstScaled;

  if (bKeepAspect != 0) {
    dAspectSrc = 280.0 / 300.0;

    if (stDstScaled.h <= 0)
      return FALSE;

    dAspectDst = (double)stDstScaled.w / (double)stDstScaled.h;

    if (dAspectDst > dAspectSrc) {
      stDstFinal.h = stDstScaled.h;
      stDstFinal.w = (int)((double)stDstFinal.h * dAspectSrc);
    } else {
      stDstFinal.w = stDstScaled.w;
      stDstFinal.h = (int)((double)stDstFinal.w / dAspectSrc);
    }

    stDstFinal.x = stDstScaled.x + (stDstScaled.w - stDstFinal.w) / 2;
    stDstFinal.y = stDstScaled.y + (stDstScaled.h - stDstFinal.h) / 2;
  }

  *pstOutFinal = stDstFinal;
  return TRUE;
}

/* Não mexi aqui: continua servindo player/monstros normalmente */
void vIMG_RenderScaled(SDL_Renderer *pSDL_Renderer,
                      STRUCT_IMAGE *pstImg,
                      SDL_Rect *pstDst,
                      double dScale,
                      int bKeepAspect) {
  SDL_Rect stDstScaled;
  SDL_Rect stDstFinal;
  int iCX;
  int iCY;
  int iNewW;
  int iNewH;
  double dAspectSrc;
  double dAspectDst;

  if ( pSDL_Renderer == NULL || pstImg == NULL || pstDst == NULL )
    return;

  if ( pstImg->pSDL_Txtr == NULL )
    return;

  if ( dScale <= 0.0 )
    dScale = 1.0;

  iCX = pstDst->x + pstDst->w / 2;
  iCY = pstDst->y + pstDst->h / 2;

  iNewW = (int)((double)pstDst->w * dScale);
  iNewH = (int)((double)pstDst->h * dScale);

  stDstScaled.w = iNewW;
  stDstScaled.h = iNewH;
  stDstScaled.x = iCX - stDstScaled.w / 2;
  stDstScaled.y = iCY - stDstScaled.h / 2;

  stDstFinal = stDstScaled;

  if ( bKeepAspect != 0 ) {
    dAspectSrc = 280.0 / 300.0;

    if ( stDstScaled.h <= 0 )
      return;

    dAspectDst = (double)stDstScaled.w / (double)stDstScaled.h;

    if ( dAspectDst > dAspectSrc ) {
      stDstFinal.h = stDstScaled.h;
      stDstFinal.w = (int)((double)stDstFinal.h * dAspectSrc);
    } else {
      stDstFinal.w = stDstScaled.w;
      stDstFinal.h = (int)((double)stDstFinal.w / dAspectSrc);
    }

    stDstFinal.x = stDstScaled.x + (stDstScaled.w - stDstFinal.w) / 2;
    stDstFinal.y = stDstScaled.y + (stDstScaled.h - stDstFinal.h) / 2;
  }

  SDL_RenderCopy(pSDL_Renderer, pstImg->pSDL_Txtr, &pstImg->pSDL_IMGRect, &stDstFinal);
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

  if ( DEBUG_SDL_MSGS ) {
    vTraceVarArgsFn(
      "Scroll atualizado: TopIndex=%d / Total=%d / Visible=%d",
      giDlgTopIndex,
      iTotal,
      iVisible
    );
  }
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
  iVisible = (iH - 16) / iLineH;
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
  int bRsl = FALSE;
  if ( DEBUG_SDL_ALL ) {
    vTraceVarArgsFn("begin");
    vTraceVarArgsFn("pSDL_RECT=%p", pSDL_RECT);
    vTraceVarArgsFn("iX=%d", iX);
    vTraceVarArgsFn("iY=%d", iY);
  }
  if ( pSDL_RECT == NULL ) return FALSE;
  if (iX >= pSDL_RECT->x &&
      iY >= pSDL_RECT->y &&
      iX <= pSDL_RECT->x + pSDL_RECT->w &&
      iY <= pSDL_RECT->y + pSDL_RECT->h) {
    bRsl = TRUE;
  }
  if ( DEBUG_SDL_ALL ) vTraceVarArgsFn("end - [%s]", bRsl ? "TRUE" : "FALSE");
  return bRsl;
}

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

  stRectShadow = stRectDialog;
  vSDL_DrawRectShadow(pSDL_Renderer, &stRectShadow, 3, 3, 150);

  SDL_SetRenderDrawColor(pSDL_Renderer,
                        stColPanel.r, stColPanel.g, stColPanel.b, stColPanel.a);
  SDL_RenderFillRect(pSDL_Renderer, &stRectDialog);

  SDL_SetRenderDrawColor(pSDL_Renderer,
                        stColBorder.r, stColBorder.g, stColBorder.b, stColBorder.a);
  SDL_RenderDrawRect(pSDL_Renderer, &stRectDialog);

  iDlgGetCount();
  iStartIndex = iDlgGetTopIndex();

  pstCur = pstDlgGetAt(iStartIndex);

  for (iLine = 0; iLine < pstLayout->iVisibleLines; iLine++) {
    int iX;
    int iY;

    iX = pstLayout->iX + 8;
    iY = pstLayout->iY + 8 + iLine * pstLayout->iLineHeight;

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
  iMarginTop    = 50 + 500 + 2;
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
  int iPaddingAdjust;

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

  stRectChip.x = iXHP - iPaddingAdjust;
  stRectChip.y = iY + 2;
  stRectChip.w = iChipW;
  stRectChip.h = iChipH;

  SDL_SetRenderDrawColor(pSDL_Renderer, stColHP.r, stColHP.g, stColHP.b, stColHP.a);
  SDL_RenderFillRect(pSDL_Renderer, &stRectChip);
  vSDL_DrawTextShadow(pSDL_Renderer, szHP, iXHP, iY, stColHP, stColShadow, 1, 1);
  vSDL_DrawText(pSDL_Renderer, szHP, iXHP, iY, stColHP);

  stRectChip.x = iXEnergy - iPaddingAdjust;
  SDL_SetRenderDrawColor(pSDL_Renderer, stColEnergy.r, stColEnergy.g, stColEnergy.b, stColEnergy.a);
  SDL_RenderFillRect(pSDL_Renderer, &stRectChip);
  vSDL_DrawTextShadow(pSDL_Renderer, szEnergy, iXEnergy, iY, stColEnergy, stColShadow, 1, 1);
  vSDL_DrawText(pSDL_Renderer, szEnergy, iXEnergy, iY, stColEnergy);

  stRectChip.x = iXBlock - iPaddingAdjust;
  SDL_SetRenderDrawColor(pSDL_Renderer, stColBlock.r, stColBlock.g, stColBlock.b, stColBlock.a);
  SDL_RenderFillRect(pSDL_Renderer, &stRectChip);
  vSDL_DrawTextShadow(pSDL_Renderer, szBlock, iXBlock, iY, stColBlock, stColShadow, 1, 1);
  vSDL_DrawText(pSDL_Renderer, szBlock, iXBlock, iY, stColBlock);
}

void vSDL_DrawTable(SDL_Renderer *pSDL_Renderer, PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCt) {
  int ii = 0;
  PSTRUCT_ELEMENT pstTable = NULL;

  if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn("iMonsterCt=%d iHandCt=%d", iMonsterCt, pstDeck->iHandCount);

  iSCREEN_SetLayout(LAYOUT_TABLE);

  pstTable = pstSCREEN_GetElementByName("TABLE");
  if ( !pstTable ) {
    if ( DEBUG_SDL_MORE_MSGS ) vTraceVarArgsFn("Falha ao pegar o elemento TABLE!");
    _exit(1);
  }

  SET_RENDER_DRAW_COLOR(pSDL_Renderer, SDL_COLOR_FROM_RGB_OPACITY(SDL_RGB_TABLE_GREEN, OPACITY_OPAQUE));
  SDL_RenderFillRect(pSDL_Renderer, (SDL_Rect*)&pstTable->stRect);
  SET_RENDER_DRAW_COLOR(pSDL_Renderer, SDL_COLOR_FROM_RGB_OPACITY(SDL_RGB_TABLE_BORDER, OPACITY_OPAQUE));
  SDL_RenderDrawRect(pSDL_Renderer, (SDL_Rect*)&pstTable->stRect);

  /* --------------------- */
  /*      MONSTROS         */
  /* --------------------- */
  {
    int iPad;
    int iUsableW;
    int iCols;
    int iGapX;
    int iGapY;
    int iSlotW;
    int iSlotX;
    int iSlotY;
    int iRow;
    int iCol;

    int iCardMaxW;
    int iCardMinW;
    int iCardW;
    int iCardH;

    SDL_Color stSDLColor;
    SDL_Color stShadow;

    iPad = 16;
    iGapX = 16;
    iGapY = 18;

    iUsableW = pstTable->stRect.w - iPad * 2;

    /* colunas (grid) */
    if (iMonsterCt <= 0) {
      iCols = 1;
    } else if (iMonsterCt <= 3) {
      iCols = iMonsterCt;
    } else if (iMonsterCt <= 6) {
      iCols = 3;
    } else if (iMonsterCt <= 9) {
      iCols = 3;
    } else {
      iCols = 4;
    }

    /* largura do "slot" (container) */
    if (iCols < 1)
      iCols = 1;

    iSlotW = (iUsableW - (iCols - 1) * iGapX) / iCols;
    if (iSlotW < 1)
      iSlotW = 1;

    /* tamanho de carta (como player) */
    iCardMaxW = 140;
    iCardMinW = 80;

    iCardW = iSlotW - 8;
    if (iCardW > iCardMaxW)
      iCardW = iCardMaxW;
    if (iCardW < iCardMinW)
      iCardW = iCardMinW;

    iCardH = (iCardW * 300) / 280;

    stSDLColor = (SDL_Color){255, 255, 255, 255};
    stShadow   = (SDL_Color){0, 0, 0, 180};

    for (ii = 0; ii < iMonsterCt; ii++) {
      SDL_Rect stRectMonster;
      SDL_Rect stRectFinal;
      PSTRUCT_MONSTER pstMonster;
      char szStatus[128];
      int iPoison;
      int iPoisonLeft;
      int iParalyzeLeft;
      int jj;

      STRUCT_IMAGE *pstImg;
      int iImgIndex;
      double dScaleMonstro;
      int bOkFinal;

      pstMonster = &pastMonsters[ii];

      /* morto: zera hitbox pra não ficar fantasma */
      if (pstMonster->iHP <= 0) {
        if (ii < (int)(sizeof(gMonsterRects) / sizeof(gMonsterRects[0]))) {
          gMonsterRects[ii].x = 0;
          gMonsterRects[ii].y = 0;
          gMonsterRects[ii].w = 0;
          gMonsterRects[ii].h = 0;
        }
        continue;
      }

      iCol = ii % iCols;
      iRow = ii / iCols;

      iSlotX = pstTable->stRect.x + iPad + iCol * (iSlotW + iGapX);
      iSlotY = pstTable->stRect.y + iPad + iRow * (iCardH + iGapY);

      /* carta centralizada dentro do slot */
      stRectMonster.w = iCardW;
      stRectMonster.h = iCardH;
      stRectMonster.x = iSlotX + (iSlotW - stRectMonster.w) / 2;
      stRectMonster.y = iSlotY;

      /* render do monstro */
      iImgIndex = ii % 4;
      dScaleMonstro = 1.0; /* pode subir pra 1.2 se quiser; clique continuará certo */
      pstImg = pIMG_GetNextByType(IMAGE_TYPE_MONSTER, iImgIndex);
      bOkFinal = iIMG_ComputeScaledRect(&stRectMonster, dScaleMonstro, TRUE, &stRectFinal);

      if (pstImg != NULL && pstImg->pSDL_Txtr != NULL && bOkFinal != FALSE) {
        /* desenha exatamente no ret final */
        SDL_RenderCopy(pSDL_Renderer, pstImg->pSDL_Txtr, &pstImg->pSDL_IMGRect, &stRectFinal);
      } else {
        SET_RENDER_DRAW_COLOR(pSDL_Renderer, SDL_COLOR_FROM_RGB_OPACITY(SDL_RGB_RED_BLOOD, OPACITY_OPAQUE));
        SDL_RenderFillRect(pSDL_Renderer, &stRectMonster);
        stRectFinal = stRectMonster;
      }

      /* hitbox = área real da carta renderizada */
      if (ii < (int)(sizeof(gMonsterRects) / sizeof(gMonsterRects[0])))
        gMonsterRects[ii] = stRectFinal;

      {
        char szHp[32];
        char szBlk[32];
        SDL_Color stFG;
        SDL_Color stShadow;

        stFG.r = 255; stFG.g = 255; stFG.b = 255; stFG.a = 255;
        stShadow.r = 0; stShadow.g = 0; stShadow.b = 0; stShadow.a = 200;

        memset(szHp, 0x00, sizeof(szHp));
        snprintf(szHp, sizeof(szHp), "HP:%d", pstMonster->iHP);
        vSDL_DrawTextShadow(pSDL_Renderer, szHp, stRectFinal.x + 6, stRectFinal.y + 6, stFG, stShadow, 1, 1);

        if (pstMonster->iBlock > 0) {
          memset(szBlk, 0x00, sizeof(szBlk));
          snprintf(szBlk, sizeof(szBlk), "S:%d", pstMonster->iBlock);
          vSDL_DrawTextShadow(pSDL_Renderer, szBlk, stRectFinal.x + 6, stRectFinal.y + 22, stFG, stShadow, 1, 1);
        }
      }
      {
        int bHover;
        int bSel;
        int iK;
        SDL_Rect stTmp;

        bHover = (ii == giHoverMonster);
        bSel = (ii == giSelectedMonster);

        if (bHover || bSel) {
          stTmp = stRectFinal;

          if (bSel) SDL_SetRenderDrawColor(pSDL_Renderer, 240, 120, 20, 255);
          else SDL_SetRenderDrawColor(pSDL_Renderer, 240, 200, 20, 255);

          for (iK = 0; iK < 2; iK++) {
            SDL_RenderDrawRect(pSDL_Renderer, &stTmp);
            stTmp.x -= 1; stTmp.y -= 1;
            stTmp.w += 2; stTmp.h += 2;
          }
        }
      }

      /* status (veneno/paralyze) - desenha dentro/na base da carta */
      iPoison = 0;
      iPoisonLeft = 0;
      iParalyzeLeft = 0;
      jj = 0;

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
        int iLen;
        iLen = (int)strlen(szStatus) + 1;
        snprintf(szTmp, sizeof(szTmp), "V:%d R:%d ", iPoison, iPoisonLeft);
        strncat(szStatus, szTmp, sizeof(szStatus) - iLen);
      }
      if (iParalyzeLeft > 0) {
        char szTmp[64];
        int iLen;
        iLen = (int)strlen(szStatus) + 1;
        snprintf(szTmp, sizeof(szTmp), "Prlz:%d ", iParalyzeLeft);
        strncat(szStatus, szTmp, sizeof(szStatus) - iLen);
      }

      if (strlen(szStatus) > 0) {
        int iTxtX;
        int iTxtY;
        iTxtX = stRectFinal.x + 8;
        iTxtY = stRectFinal.y + stRectFinal.h - 20;
        vSDL_DrawTextShadow(pSDL_Renderer, szStatus, iTxtX, iTxtY, stSDLColor, stShadow, 1, 1);
      }
    }
    giMonsterCount = iMonsterCt;
  }

  /* --------------------- */
  /*    CARTAS PLAYER      */
  /* --------------------- */
  giCardCount = 0;
  if (pstDeck->iHandCount > 0) {
    int iPadX;
    int iUsableW;
    int iSlotW;

    iPadX = 24;
    iUsableW = pstTable->stRect.w - iPadX * 2;
    iSlotW = iUsableW / pstDeck->iHandCount;

    for (ii = 0; ii < pstDeck->iHandCount; ii++) {
      SDL_Rect stRectCard;
      int iCardMaxW;
      int iCardMinW;
      int iSlotInnerW;
      int iCardW;
      int iCardH;

      iCardMaxW = 140;
      iCardMinW = 60;

      iSlotInnerW = iSlotW - 8;
      iCardW = iSlotInnerW;

      if (iCardW > iCardMaxW)
        iCardW = iCardMaxW;
      if (iCardW < iCardMinW)
        iCardW = iCardMinW;

      iCardH = (iCardW * 300) / 280;

      stRectCard.w = iCardW;
      stRectCard.h = iCardH;
      stRectCard.x = pstTable->stRect.x + iPadX + ii * iSlotW + (iSlotW - stRectCard.w) / 2;
      stRectCard.y = pstTable->stRect.y + pstTable->stRect.h - stRectCard.h - 20;

      if (ii < (int)(sizeof(gCardRects) / sizeof(gCardRects[0])))
        gCardRects[ii] = stRectCard;

      if (ii < MAX_HAND) {
        int iR;
        int iG;
        int iB;

        iR = 200;
        iG = 200;
        iB = 200;

        {
          STRUCT_IMAGE *pstImg;
          pstImg = pIMG_FindFirstById(pstDeck->astHand[ii].iType);

          if (pstImg != NULL && pstImg->pSDL_Txtr != NULL) {
            vIMG_RenderScaled(pSDL_Renderer, pstImg, &stRectCard, 1.0, TRUE);
          } else {
            SDL_SetRenderDrawColor(pSDL_Renderer, iR, iG, iB, 255);
            SDL_RenderFillRect(pSDL_Renderer, &stRectCard);
          }
        }
        {
          int bSel;
          int bHover;
          int iK;
          SDL_Rect stTmp;

          bSel = (ii == giPendingCard);
          bHover = (ii == giHoverCard);

          stTmp = stRectCard;

          if (bSel) {
            SDL_SetRenderDrawColor(pSDL_Renderer, 240, 200, 20, 255);
            for (iK = 0; iK < 3; iK++) {
              SDL_RenderDrawRect(pSDL_Renderer, &stTmp);
              stTmp.x -= 1; stTmp.y -= 1;
              stTmp.w += 2; stTmp.h += 2;
            }
          } else if (bHover) {
            SDL_SetRenderDrawColor(pSDL_Renderer, 255, 255, 255, 255);
            for (iK = 0; iK < 2; iK++) {
              SDL_RenderDrawRect(pSDL_Renderer, &stTmp);
              stTmp.x -= 1; stTmp.y -= 1;
              stTmp.w += 2; stTmp.h += 2;
            }
          } else {
            SDL_SetRenderDrawColor(pSDL_Renderer, 30, 30, 30, 255);
            SDL_RenderDrawRect(pSDL_Renderer, &stRectCard);
          }
        }
      }
      giCardCount++;
    }
  }
}

/* -------------------------------------------------------------- */
/* Inicialização e loop principal */

void vSDL_MainInit(void) {
  int iImgFlags;
  int iImgInitted;
  if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn(" -- Begin");

  SDL_SetMainReady();

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn("Couldn't initialize SDL: %s", SDL_GetError());
    return;
  }

  iImgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  iImgInitted = IMG_Init(iImgFlags);
  if ((iImgInitted & iImgFlags) != iImgFlags) {
    if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn("Erro IMG_Init: %s", IMG_GetError());
  }

  if (TTF_WasInit() == 0) {
    if (TTF_Init() != 0) {
      if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn("Erro TTF_Init: %s", TTF_GetError());
    }
  }

  gFont = ttfSDL_InitFont();
  if (!gFont) {
    if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn("Erro ao carregar fonte: %s", TTF_GetError());
  }

  if (bLoadImgListFromFile() == FALSE) {
    if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn("Erro ao carregar image list");
  }

  vEVENT_Init();
  if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn(" -- End");
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
      continue;

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
  int iX;
  int iY;

  iX = pEv->button.x;
  iY = pEv->button.y;

  if (bAreCoordsInSDL_Rect(&pLayout->stBtnUp, iX, iY)) {
    vDlgScrollLines(-1, pLayout->iVisibleLines);
    return REDRAW_DIALOG;
  }

  if (bAreCoordsInSDL_Rect(&pLayout->stBtnDown, iX, iY)) {
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

  bRedrawDialog = (iRedrawAction & REDRAW_DIALOG) != 0;
  bRedrawTable  = (iRedrawAction & REDRAW_TABLE)  != 0;


  if ( DEBUG_LVL_ALL ) {
    vTraceVarArgsFn("gstGame.iStatus=[%s]", gkpaszGameStatus[gstGame.iStatus]);
    vTraceVarArgsFn("bRedrawDialog=%d bRedrawTable=%d", bRedrawDialog, bRedrawTable);
  }
  if (bRedrawDialog == 0 && bRedrawTable == 0)
    return;

  if ( gstGame.iStatus == STATUS_GAMING )
    gstGame.iState = STATE_GAMING_DRAWING;
    
  if (bRedrawTable) {
    SDL_RenderClear(pSDL_Renderer);
    vSDL_DrawTable(pSDL_Renderer, pstDeck, pastMonsters, iMonsterCt);
    vSDL_DrawHUD(pSDL_Renderer, &gstPlayer);
    vEVR_Render(pSDL_Renderer);
  }

  if (bRedrawDialog) {
    int iWinW;
    int iWinH;
    SDL_GetRendererOutputSize(pSDL_Renderer, &iWinW, &iWinH);
    vSDL_DialogComputeLayout(iWinW, iWinH, &gstDialogLayout);
    vSDL_DialogDraw(pSDL_Renderer, &gstDialogLayout);
  }

  SDL_RenderPresent(pSDL_Renderer);
}

void vSDL_MessageBox(const char *kpszMsg, const char *kpszFooterMsg) {
  SDL_Surface* pstMsgBoxTextSurface = NULL;
  SDL_Texture* pstMsgBoxTextTexture = NULL;
  SDL_Rect stFooterTextRect = { 0 };
  SDL_Surface* pstFooterTextSurface = NULL;
  SDL_Texture* pstFooterTextTexture = NULL;
  int bShowMsgBox = TRUE;
  int bRenderer = TRUE;
  SDL_Color stBackgroundColor;
  SDL_Color stBorderColor;
  SDL_Color stTextColor;
  SDL_Rect stRect;
  SDL_Rect stTextRect;
  SDL_Event unEvent;

  if ( bStrIsEmpty(kpszMsg) ) return;

  stBackgroundColor.r = 255;
  stBackgroundColor.g = 255;
  stBackgroundColor.b = 255;
  stBackgroundColor.a = 255;

  stBorderColor.r = 0;
  stBorderColor.g = 0;
  stBorderColor.b = 0;
  stBorderColor.a = 255;

  stTextColor.r = 0;
  stTextColor.g = 0;
  stTextColor.b = 0;
  stTextColor.a = 255;

  stRect.h = 100;
  stRect.w = 800 / 2;
  stRect.x = (800 - stRect.w) / 2;
  stRect.y = 800 / 4;

  pstMsgBoxTextSurface = TTF_RenderText_Blended(gFont, kpszMsg, stTextColor);
  pstMsgBoxTextTexture = SDL_CreateTextureFromSurface(gpSDL_Renderer, pstMsgBoxTextSurface);
  stTextRect.w = pstMsgBoxTextSurface->w;
  stTextRect.h = pstMsgBoxTextSurface->h;
  stTextRect.x = stRect.x + (stRect.w - stTextRect.w) / 2;
  stTextRect.y = stRect.y + (stRect.h - stTextRect.h) / 4;



  pstFooterTextSurface = TTF_RenderText_Blended(gFont, kpszFooterMsg, stTextColor);
  pstFooterTextTexture = SDL_CreateTextureFromSurface(gpSDL_Renderer, pstFooterTextSurface);

  stFooterTextRect.w = pstFooterTextSurface->w;
  stFooterTextRect.h = pstFooterTextSurface->h;
  stFooterTextRect.x = stRect.x + (stRect.w - stFooterTextRect.w) / 2;
  stFooterTextRect.y = stRect.y + stRect.h - stFooterTextRect.h - 10;

  bRenderer = TRUE;
  while ( bShowMsgBox ) {
    while ( SDL_PollEvent(&unEvent) ) {
      if ( unEvent.type == SDL_KEYDOWN || unEvent.type == SDL_QUIT ) {
        bShowMsgBox = FALSE;
        bRenderer = FALSE;
        break;
      }
    }

    if ( bRenderer ) {
      SDL_SetRenderDrawColor(gpSDL_Renderer, stBackgroundColor.r, stBackgroundColor.g, stBackgroundColor.b, stBackgroundColor.a);
      SDL_RenderFillRect(gpSDL_Renderer, &stRect);
      SDL_SetRenderDrawColor(
        gpSDL_Renderer,
        stBorderColor.r,
        stBorderColor.g,
        stBorderColor.b,
        stBorderColor.a
      );
      SDL_RenderDrawRect(gpSDL_Renderer, &stRect);
      SDL_RenderCopy(gpSDL_Renderer, pstMsgBoxTextTexture, NULL, &stTextRect);
      SDL_RenderCopy(gpSDL_Renderer, pstFooterTextTexture, NULL, &stFooterTextRect);
      SDL_RenderPresent(gpSDL_Renderer);
      if ( DEBUG_SDL_ALL ) {
        vTraceVarArgsFn("vSDL_MessageBox - %s", kpszMsg);
        vTraceVarArgsFn("vSDL_MessageBox - %s", kpszFooterMsg);
      }
      bRenderer = FALSE;
    }
  }

  SDL_FreeSurface(pstMsgBoxTextSurface);
  SDL_DestroyTexture(pstMsgBoxTextTexture);
  SDL_FreeSurface(pstFooterTextSurface);
  SDL_DestroyTexture(pstFooterTextTexture);
}

void vSDL_DrawMenu(SDL_Renderer* pSDL_Renderer, void* pstMenu) {
  int ii = 0;
  int iX = 0;
  int iY = 0;
  PSTRUCT_ELEMENT pstScreenMenu = (PSTRUCT_ELEMENT) pstMenu;

  if ( !pSDL_Renderer || !pstMenu ) return;

  SDL_RenderClear(pSDL_Renderer);

  vSDL_DrawRectShadow(pSDL_Renderer, (SDL_Rect*)&pstScreenMenu->stRect, 6, 6, 80);
  SDL_SetRenderDrawColor(pSDL_Renderer, pstScreenMenu->stBgColor.r, pstScreenMenu->stBgColor.g, pstScreenMenu->stBgColor.b, pstScreenMenu->stBgColor.a);
  SDL_RenderFillRect(pSDL_Renderer, (SDL_Rect*)&pstScreenMenu->stRect);
  SDL_SetRenderDrawColor(pSDL_Renderer, pstScreenMenu->stBorderColor.r, pstScreenMenu->stBorderColor.g, pstScreenMenu->stBorderColor.b, pstScreenMenu->stBorderColor.a);
  SDL_RenderDrawRect(pSDL_Renderer, (SDL_Rect*)&pstScreenMenu->stRect);

  vSDL_DrawText(pSDL_Renderer, pstScreenMenu->szText, pstScreenMenu->stRect.x + 20, pstScreenMenu->stRect.y + 20, *(SDL_Color*)&pstScreenMenu->stFgColor);

  iX = pstScreenMenu->stRect.x + 40;
  iY = pstScreenMenu->stRect.y + 60;
  for ( ii = 0; ii < pstScreenMenu->iCtItems; ii++ ) {
    STRUCT_SCREEN_ITEM* pstItem = &pstScreenMenu->astItem[ii];
    pstItem->stRect.x = iX;
    pstItem->stRect.y = iY + (ii * 60);
    pstItem->stRect.w = pstScreenMenu->stRect.w - 80;
    pstItem->stRect.h = 50;

    /* Fundo do item */
    if ( ii == pstScreenMenu->iSelectedItemIdx ) SDL_SetRenderDrawColor(pSDL_Renderer, 70, 90, 130, 255);
    else
      SDL_SetRenderDrawColor(pSDL_Renderer, 50, 50, 60, 255);
    SDL_RenderFillRect(pSDL_Renderer, (SDL_Rect*)&pstItem->stRect);

    SDL_SetRenderDrawColor(pSDL_Renderer, 200, 200, 210, 255);
    SDL_RenderDrawRect(pSDL_Renderer, (SDL_Rect*) &pstItem->stRect);

    vSDL_DrawText(pSDL_Renderer, pstItem->szText, pstItem->stRect.x + 10, pstItem->stRect.y + 15, *(SDL_Color*) &pstItem->stFgColor);
  }
  SDL_RenderPresent(pSDL_Renderer);
}

void vSDL_DrawPause(SDL_Renderer *pSDL_Renderer) {
  PSTRUCT_ELEMENT pstMenu = NULL;
  iSCREEN_SetLayout(LAYOUT_PAUSE);
  pstMenu = pstSCREEN_GetElementByType(ELM_TYPE_MENU);
  if ( !pstMenu ) {
    if ( DEBUG_SDL_MORE_MSGS ) vTraceVarArgsFn("Falha ao obter o menu de pausa!");
    return;
  }
  vSDL_DrawMenu(pSDL_Renderer, pstMenu);
}

int gbPauseOpen = FALSE;

static int iPauseMenuButtonAction(int *pbRunning, PSTRUCT_ELEMENT pstMenu, SDL_Event stEvent, PSTRUCT_MONSTER pastMonster, int iCtMonster);
static int iPauseMenuButtonAction(int *pbRunning, PSTRUCT_ELEMENT pstMenu, SDL_Event stEvent, PSTRUCT_MONSTER pastMonster, int iCtMonster) {
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

  switch ( pstMenu->astItem[pstMenu->iSelectedItemIdx].iAction ) {
    case ACTION_SAVE: {
      *pbRunning = FALSE;
      gstGame.iLastStatus = gstGame.iStatus;
      gstGame.iStatus = STATUS_GAMING;
      gstGame.stGameContext.stPlayer = gstPlayer;
      memcpy(gstGame.stGameContext.astMonster, pastMonster, sizeof(STRUCT_MONSTER) * iCtMonster);
      gstGame.stGameContext.iCtMonster = iCtMonster;
      iGameSave();
      gstGame.iStatus = gstGame.iLastStatus;
      gstGame.iLastStatus = gstGame.iStatus;
      vSDL_MessageBox(MSG(MSG_GAME_SAVE_WITH_SUCCESS), MSG(MSG_PRESS_ANY_KEY_TO_CONTINUE));
      break;
    }
    case ACTION_SAVE_AND_EXIT: {
      *pbRunning = FALSE;
      gstGame.iLastStatus = gstGame.iStatus;
      gstGame.iStatus = STATUS_GAMING;
      gstGame.stGameContext.stPlayer = gstPlayer;
      memcpy(gstGame.stGameContext.astMonster, pastMonster, sizeof(STRUCT_MONSTER) * iCtMonster);
      gstGame.stGameContext.iCtMonster = iCtMonster;
      iGameSave();
      gstGame.iStatus = gstGame.iLastStatus;
      gstGame.iLastStatus = gstGame.iStatus;
      vSDL_MessageBox(MSG(MSG_GAME_SAVE_WITH_SUCCESS), MSG(MSG_EXIT_GAME));
      gstGame.iStatus = (gstGame.iStatus == STATUS_PAUSE ? STATUS_GAMING : STATUS_PAUSE);
      return FINISH_PROGRAM;
    }
    case ACTION_SETTINGS: {
      *pbRunning = FALSE;
      break;
    }
    case ACTION_BACK: {
      gbPauseOpen = FALSE;
      *pbRunning = FALSE;
      gstGame.iStatus = (gstGame.iStatus == STATUS_PAUSE ? STATUS_GAMING : STATUS_PAUSE);
      break;
    }
    default: break;
  }

  return 0;
}

int iSDL_OpenPause(SDL_Renderer *pSDL_Renderer, PSTRUCT_MONSTER pastMonster, int iCtMonster){
  SDL_Event stEvent;
  PSTRUCT_ELEMENT pstMenu = NULL;
  static int bRunning = FALSE;

  if ( DEBUG_SDL_MORE_MSGS ) {
    vTraceVarArgsFn("begin");
    vTraceVarArgsFn("gstGame.iStatus [%s]", gkpaszGameStatus[gstGame.iStatus]);
  }
  memset(&stEvent  , 0x00, sizeof(stEvent  ));

  if ( gstGame.iStatus != STATUS_PAUSE ) return -1;

  iSCREEN_SetLayout(LAYOUT_PAUSE);

  pstMenu = pstSCREEN_GetElementByType(ELM_TYPE_MENU);
  if ( !pstMenu ) {
    if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn("FALHA em pstSCREEN_GetElementByType(ELM_TYPE_MENU)");
    return -1;
  }

  if ( pSDL_Renderer == NULL ) return -1;
  gbPauseOpen = TRUE;
  bRunning = TRUE;
  while ( bRunning ) {
    /* desenha a tela */
    vSDL_DrawPause(pSDL_Renderer);
    while ( SDL_PollEvent(&stEvent) ) {
      if ( stEvent.type == SDL_QUIT ) {
        gbPauseOpen = FALSE;
        gstGame.iStatus = (gstGame.iStatus == STATUS_PAUSE ? STATUS_GAMING : STATUS_PAUSE);
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
        else if ( key == SDLK_ESCAPE ) {
          gbPauseOpen = FALSE;
          return 0;
        }
        else if ( key == SDLK_RETURN || key == SDLK_KP_ENTER ) {
          if ( iPauseMenuButtonAction(&bRunning, pstMenu, stEvent, pastMonster, iCtMonster) == FINISH_PROGRAM ) {
            return FINISH_PROGRAM;
          }
        }
      }
      else if ( stEvent.type == SDL_MOUSEBUTTONDOWN ) {
        if ( iPauseMenuButtonAction(&bRunning, pstMenu, stEvent, pastMonster, iCtMonster) == FINISH_PROGRAM ) {
          return FINISH_PROGRAM;
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

  gbPauseOpen = FALSE;

  return 0;
}

void vSDL_MainLoop(int *pbRunning, SDL_Event *pSDL_Event, SDL_Renderer *pSDL_Renderer, PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCt) {
  int iRedrawAction;
  int bHasPlayableCards;
  
#if SDL_MAJOR_VERSION >=2 && SDL_MINOR_VERSION >= 0 && SDL_PATCHLEVEL >= 18
  uint64_t ui64FrameStart;
  uint64_t ui64FrameTime;
#else
  uint32_t ui64FrameStart;
  uint32_t ui64FrameTime;
#endif

  if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn(" --- SDL MAIN LOOP");

  iRedrawAction = REDRAW_TABLE;
  bHasPlayableCards = FALSE;
  
  gstGame.iStatus = STATUS_GAMING;

  vRedraw(pSDL_Renderer, iRedrawAction, pstDeck, pastMonsters, iMonsterCt);

  gstGame.iState = STATE_GAMING_PLAYER_TURN;

  while (*pbRunning) {
    ui64FrameStart = u64SDL_GetTicks();

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

    if (!(*pbRunning))
      break;

    if (gstGame.iStatus == STATUS_PAUSE) {
      vRedraw(pSDL_Renderer, iRedrawAction, pstDeck, pastMonsters, iMonsterCt);
      iRedrawAction = REDRAW_NONE;
      if ( iSDL_OpenPause(pSDL_Renderer, pastMonsters, iMonsterCt) == FINISH_PROGRAM ) {
        *pbRunning = FALSE;
        break;
      }
      iRedrawAction = REDRAW_TABLE;
      vRedraw(pSDL_Renderer, iRedrawAction, pstDeck, pastMonsters, iMonsterCt);
    }

    if (iRedrawAction == -2) {
      vTraceVarArgsFn("iRedrawAction == -2");
      vRedraw(pSDL_Renderer, REDRAW_TABLE, pstDeck, pastMonsters, iMonsterCt);
      iRedrawAction = REDRAW_NONE;
      continue;
    }

    if (!iAnyMonsterAlive(pastMonsters, iMonsterCt)) {
      char szMsg[128];
      gstGame.iState = STATE_GAMING_LEVEL_WON;
      memset(szMsg, 0x00, sizeof(szMsg));
      snprintf(szMsg, sizeof(szMsg), "*** Nivel %d completo! ***", giLevel);
      vPrintLine(szMsg, NO_NEW_LINE);
      iRedrawAction = REDRAW_TABLE;
      vAddPlayerReward(&gstPlayer);
      vEVR_ClearAll();
      if ( iSDL_OpenShop(pSDL_Renderer, &gstPlayer, pstDeck) == SHOP_EXIT ) {
        *pbRunning = FALSE;
        break;
      }
      vRedraw(pSDL_Renderer, iRedrawAction, pstDeck, pastMonsters, iMonsterCt);
      giLevel++;
      vInitMonstersForLevel(pastMonsters, giLevel, &iMonsterCt);
      vStartNewTurn(pstDeck);
      vTraceDeck(pstDeck, TRACE_DECK_ALL);
    }
    else if (gstPlayer.iEnergy <= 0 || !(bHasPlayableCards = bHasAnyPlayableCard(pstDeck))) {
      if ( DEBUG_SDL_MSGS ) {
        vTraceVarArgsFn("Player Energy=[%d] | Got any playable card?=%d",
                      gstPlayer.iEnergy, bHasPlayableCards);
      }
      vRedraw(pSDL_Renderer, REDRAW_TABLE, pstDeck, pastMonsters, iMonsterCt);
      gstGame.iLastState = gstGame.iState;
      gstGame.iState = STATE_GAMING_ENEMY_TURN;
      vDoEnemyActions(pastMonsters, iMonsterCt);
      gstGame.iLastState = gstGame.iState;
      gstGame.iState = STATE_GAMING_TURN_ENDED;
      vPrintLine("Iniciando novo turno, aguarde...", INSERT_NEW_LINE);
      vRedraw(pSDL_Renderer, REDRAW_TABLE, pstDeck, pastMonsters, iMonsterCt);
      iRedrawAction = REDRAW_NONE;
      iGameSave();
      if ( gstPlayer.iHP <= 0 ) {
        vPrintLine(MSG(MSG_YOU_LOSE), INSERT_NEW_LINE);
        vSDL_MessageBox(MSG(MSG_YOU_LOSE), MSG(MSG_PRESS_ANY_KEY_TO_CONTINUE));
        vRedraw(pSDL_Renderer, REDRAW_TABLE, pstDeck, pastMonsters, iMonsterCt);
        iRedrawAction = REDRAW_NONE;
        iGameDelete();
        vSDL_WelcomeInit();
        if ( iSDL_OpenWelcome(pSDL_Renderer) == FINISH_PROGRAM ) {
          pbRunning = FALSE;
          break;
        }
        vEVR_ClearAll();
        vInitBasicDeck(pstDeck);
        iDrawMultipleCard(INIT_HAND_CARDS, pstDeck);
        vInitPlayer(pstDeck, !gbSDL_Mode);
        giLevel = 1;
        memset(&gstGame, 0x00, sizeof(gstGame));
        gstGame.iLevel = giLevel;
        gstGame.iStatus = STATUS_WELCOME;
        gstGame.iLastStatus = STATUS_NONE;
        gstGame.iLastState = STATE_NONE;
        gstGame.iState = STATE_WELCOME_BEGIN;
        vInitMonstersForLevel(pastMonsters, giLevel, &iMonsterCt);
        vInitDialog();
        iRedrawAction = REDRAW_TABLE;
        vRedraw(pSDL_Renderer, REDRAW_TABLE, pstDeck, pastMonsters, iMonsterCt);
      }

      vStartNewTurn(pstDeck);
      gstGame.iStatus = STATUS_GAMING;
      gstGame.iState = STATE_GAMING_PLAYER_TURN;
      gstGame.iLastStatus = STATUS_NONE;
      gstGame.iLastState = STATE_NONE;
      vTraceDeck(pstDeck, TRACE_DECK_ALL);

      iRedrawAction = REDRAW_TABLE;
    }

    memcpy(gstPlayer.astPlayerCards, pstDeck, sizeof(STRUCT_DECK));
    if (iEVR_Tick(ui64FrameStart) != FALSE) iRedrawAction |= REDRAW_TABLE;

    if (iRedrawAction != REDRAW_NONE)
      vRedraw(pSDL_Renderer, iRedrawAction, pstDeck, pastMonsters, iMonsterCt);

    iRedrawAction = REDRAW_NONE;

    ui64FrameTime = u64SDL_GetTicks() - ui64FrameStart;

    if (ui64FrameTime < VSYNC_TIME) {
      SDL_Delay(VSYNC_TIME - ui64FrameTime);
    }
  }
  
  if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn(" --- SDL MAIN LOOP END");
}

void vSDL_MainQuit(void) {
  if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn(" -- Begin");

  if (gFont) {
    TTF_CloseFont(gFont);
    gFont = NULL;
  }

  vIMG_UnloadAll();

  if (gpSDL_Renderer) {
    SDL_DestroyRenderer(gpSDL_Renderer);
    gpSDL_Renderer = NULL;
  }

  if (gpSDL_Window) {
    SDL_DestroyWindow(gpSDL_Window);
    gpSDL_Window = NULL;
  }

  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
  vEVENT_Quit();

  if ( DEBUG_SDL_MSGS ) vTraceVarArgsFn(" -- End ");
}

#endif /* USE_SDL2 */
