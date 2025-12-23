#ifdef USE_SDL2
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <deck.h>
#include <debuff.h>
#include <monster.h>
#include <player.h>
#include <battle.h>
#include <sdl_api.h>
#include <event_render.h>
#include <SDL2/SDL.h>

#define EVR_MAX_POPUPS   16
#define EVR_MAX_MONSTERS 64

#define EVR_TARGET_PLAYER  0
#define EVR_TARGET_MONSTER 1

typedef struct {
  int bActive;
  int iTargetType;
  int iTargetIdx;
  char szText[32];
  uint64_t ui64Start;
  uint64_t ui64Dur;
} STRUCT_EVR_POPUP;

typedef struct {
  int bActive;
  uint64_t ui64End;
  SDL_Color stCol;
} STRUCT_EVR_FLASH;

static STRUCT_EVR_POPUP gastPopups[EVR_MAX_POPUPS];
static STRUCT_EVR_FLASH gastMonFlash[EVR_MAX_MONSTERS];
static STRUCT_EVR_FLASH gstPlayerFlash;

static uint64_t gui64NowMs = 0;

extern SDL_Rect gMonsterRects[64];

static void vEVR_ClearAll(void) {
  memset(gastPopups, 0x00, sizeof(gastPopups));
  memset(gastMonFlash, 0x00, sizeof(gastMonFlash));
  memset(&gstPlayerFlash, 0x00, sizeof(gstPlayerFlash));
  gui64NowMs = 0;
}

static void vEVR_SetMonsterFlash(int iMonsterIdx, SDL_Color stCol, uint64_t ui64Ms) {
  if (iMonsterIdx < 0 || iMonsterIdx >= EVR_MAX_MONSTERS)
    return;

  gastMonFlash[iMonsterIdx].bActive = 1;
  gastMonFlash[iMonsterIdx].stCol = stCol;
  gastMonFlash[iMonsterIdx].ui64End = gui64NowMs + ui64Ms;
}

static void vEVR_SetPlayerFlash(SDL_Color stCol, uint64_t ui64Ms) {
  gstPlayerFlash.bActive = 1;
  gstPlayerFlash.stCol = stCol;
  gstPlayerFlash.ui64End = gui64NowMs + ui64Ms;
}

static void vEVR_PushPopup(int iTargetType, int iTargetIdx, const char *pszText, uint64_t ui64Dur) {
  int i;
  int iSlot;

  iSlot = -1;

  for (i = 0; i < EVR_MAX_POPUPS; i++) {
    if (gastPopups[i].bActive == 0) {
      iSlot = i;
      break;
    }
  }

  if (iSlot < 0) {
    uint64_t ui64Oldest;
    int iOldIx;

    ui64Oldest = (uint64_t)-1;
    iOldIx = 0;

    for (i = 0; i < EVR_MAX_POPUPS; i++) {
      if (gastPopups[i].ui64Start < ui64Oldest) {
        ui64Oldest = gastPopups[i].ui64Start;
        iOldIx = i;
      }
    }
    iSlot = iOldIx;
  }

  gastPopups[iSlot].bActive = 1;
  gastPopups[iSlot].iTargetType = iTargetType;
  gastPopups[iSlot].iTargetIdx = iTargetIdx;
  gastPopups[iSlot].ui64Start = gui64NowMs;
  gastPopups[iSlot].ui64Dur = (ui64Dur == 0 ? 600 : ui64Dur);

  memset(gastPopups[iSlot].szText, 0x00, sizeof(gastPopups[iSlot].szText));
  if (pszText != NULL) {
    strncpy(gastPopups[iSlot].szText, pszText, sizeof(gastPopups[iSlot].szText) - 1);
  }
}

void vEVR_Init(void) {
  vEVR_ClearAll();
}

void vEVR_Quit(void) {
  vEVR_ClearAll();
}

void vEVR_PushMonsterDamage(int iMonsterIdx, int iDamage) {
  char szTmp[32];
  SDL_Color stFlash;

  memset(szTmp, 0x00, sizeof(szTmp));
  snprintf(szTmp, sizeof(szTmp), "-%d", iDamage);

  stFlash.r = 220; stFlash.g = 40; stFlash.b = 40; stFlash.a = 90;

  vEVR_PushPopup(EVR_TARGET_MONSTER, iMonsterIdx, szTmp, 700);
  vEVR_SetMonsterFlash(iMonsterIdx, stFlash, 220);
}

void vEVR_PushMonsterHeal(int iMonsterIdx, int iHeal) {
  char szTmp[32];
  SDL_Color stFlash;

  memset(szTmp, 0x00, sizeof(szTmp));
  snprintf(szTmp, sizeof(szTmp), "+%d", iHeal);

  stFlash.r = 40; stFlash.g = 200; stFlash.b = 40; stFlash.a = 90;

  vEVR_PushPopup(EVR_TARGET_MONSTER, iMonsterIdx, szTmp, 700);
  vEVR_SetMonsterFlash(iMonsterIdx, stFlash, 220);
}

void vEVR_PushMonsterShield(int iMonsterIdx, int iShield) {
  char szTmp[32];
  SDL_Color stFlash;

  memset(szTmp, 0x00, sizeof(szTmp));
  snprintf(szTmp, sizeof(szTmp), "+%d", iShield);

  stFlash.r = 40; stFlash.g = 120; stFlash.b = 220; stFlash.a = 90;

  vEVR_PushPopup(EVR_TARGET_MONSTER, iMonsterIdx, szTmp, 700);
  vEVR_SetMonsterFlash(iMonsterIdx, stFlash, 220);
}

void vEVR_PushPlayerHeal(int iHeal) {
  char szTmp[32];
  SDL_Color stFlash;

  memset(szTmp, 0x00, sizeof(szTmp));
  snprintf(szTmp, sizeof(szTmp), "+%d", iHeal);

  stFlash.r = 40; stFlash.g = 200; stFlash.b = 40; stFlash.a = 90;

  vEVR_PushPopup(EVR_TARGET_PLAYER, 0, szTmp, 700);
  vEVR_SetPlayerFlash(stFlash, 220);
}

void vEVR_PushPlayerShield(int iShield) {
  char szTmp[32];
  SDL_Color stFlash;

  memset(szTmp, 0x00, sizeof(szTmp));
  snprintf(szTmp, sizeof(szTmp), "+%d", iShield);

  stFlash.r = 40; stFlash.g = 120; stFlash.b = 220; stFlash.a = 90;

  vEVR_PushPopup(EVR_TARGET_PLAYER, 0, szTmp, 700);
  vEVR_SetPlayerFlash(stFlash, 220);
}

int iEVR_Tick(uint64_t ui64NowMs) {
  int i;
  int bAny;

  gui64NowMs = ui64NowMs;
  bAny = 0;

  for (i = 0; i < EVR_MAX_POPUPS; i++) {
    if (gastPopups[i].bActive == 0)
      continue;

    if (gui64NowMs - gastPopups[i].ui64Start >= gastPopups[i].ui64Dur) {
      gastPopups[i].bActive = 0;
    } else {
      bAny = 1;
    }
  }

  for (i = 0; i < EVR_MAX_MONSTERS; i++) {
    if (gastMonFlash[i].bActive == 0)
      continue;

    if (gui64NowMs >= gastMonFlash[i].ui64End) {
      gastMonFlash[i].bActive = 0;
    } else {
      bAny = 1;
    }
  }

  if (gstPlayerFlash.bActive != 0) {
    if (gui64NowMs >= gstPlayerFlash.ui64End) {
      gstPlayerFlash.bActive = 0;
    } else {
      bAny = 1;
    }
  }

  return bAny;
}

static void vEVR_DrawPopup(SDL_Renderer *pSDL_Renderer, const STRUCT_EVR_POPUP *pstP) {
  SDL_Color stFG;
  SDL_Color stShadow;
  int iX;
  int iY;
  uint64_t ui64Age;
  int iDy;

  if (pstP == NULL || pstP->bActive == 0)
    return;

  stFG.r = 255; stFG.g = 255; stFG.b = 255; stFG.a = 255;
  stShadow.r = 0; stShadow.g = 0; stShadow.b = 0; stShadow.a = 255;

  iX = 0;
  iY = 0;

  if (pstP->iTargetType == EVR_TARGET_MONSTER) {
    SDL_Rect stR;

    if (pstP->iTargetIdx < 0 || pstP->iTargetIdx >= EVR_MAX_MONSTERS)
      return;

    stR = gMonsterRects[pstP->iTargetIdx];
    if (stR.w <= 0 || stR.h <= 0)
      return;

    iX = stR.x + stR.w / 2 - 10;
    iY = stR.y + 10;
  } else {
    iX = 70;
    iY = 10;
  }

  ui64Age = gui64NowMs - pstP->ui64Start;
  iDy = 0;
  if (pstP->ui64Dur > 0) {
    iDy = (int)((ui64Age * 24) / pstP->ui64Dur);
  }

  vSDL_DrawTextShadow(pSDL_Renderer, pstP->szText, iX, iY - iDy, stFG, stShadow, 1, 1);
}

void vEVR_Render(SDL_Renderer *pSDL_Renderer) {
  int i;

  if (pSDL_Renderer == NULL)
    return;

  for (i = 0; i < EVR_MAX_MONSTERS; i++) {
    SDL_Rect stR;

    if (gastMonFlash[i].bActive == 0)
      continue;

    stR = gMonsterRects[i];
    if (stR.w <= 0 || stR.h <= 0)
      continue;

    SDL_SetRenderDrawBlendMode(pSDL_Renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(pSDL_Renderer,
                           gastMonFlash[i].stCol.r,
                           gastMonFlash[i].stCol.g,
                           gastMonFlash[i].stCol.b,
                           gastMonFlash[i].stCol.a);
    SDL_RenderFillRect(pSDL_Renderer, &stR);
  }

  if (gstPlayerFlash.bActive != 0) {
    SDL_Rect stR;

    stR.x = 50;
    stR.y = 8;
    stR.w = 330;
    stR.h = 40;

    SDL_SetRenderDrawBlendMode(pSDL_Renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(pSDL_Renderer,
                           gstPlayerFlash.stCol.r,
                           gstPlayerFlash.stCol.g,
                           gstPlayerFlash.stCol.b,
                           gstPlayerFlash.stCol.a);
    SDL_RenderFillRect(pSDL_Renderer, &stR);
  }

  for (i = 0; i < EVR_MAX_POPUPS; i++) {
    if (gastPopups[i].bActive == 0)
      continue;
    vEVR_DrawPopup(pSDL_Renderer, &gastPopups[i]);
  }
}

#endif /* USE_SDL2 */
