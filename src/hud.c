/**
 * hud.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in March 2024
 *
 * Hud data structures and rendering
 * 
 */

#ifdef USE_SDL2
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_rect.h>
  #include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <debuff.h>
#include <deck.h>
#include <dialog.h>
#include <monster.h>
#include <player.h>
#include <card_game.h>
#include <hud.h>

STRUCT_HUD_LIST gstHudList;

STRUCT_RECT_DIMENSIONS gstPlayerHUD_Dimensions;
void vHUD_InitList(void) {
  memset( &gstHudList, 0x00, sizeof( STRUCT_HUD_LIST ) );
  gstHudList.pSDL_Rect = NULL;
  gstHudList.pstNext = NULL;
}

void vHUD_FreeList(void) {
  STRUCT_HUD_LIST *pstWrkHud;

  if ( DEBUG_MSGS ) vTraceBegin();

  for ( pstWrkHud = gstHudList.pstNext; pstWrkHud != NULL; ) {
    STRUCT_HUD_LIST *pstLastHud = pstWrkHud;

    pstWrkHud = pstWrkHud->pstNext;

    free( pstLastHud );
    pstLastHud = NULL;
  }

  if ( DEBUG_MSGS ) vTraceEnd();
}

void vHUD_SetRGBA(SDL_Rect *pSDL_Rect_HUD, const char *kpszFillRGBA, const char *kpszDrawRGBA) {
  PSTRUCT_HUD_LIST pstWrkHud = NULL;

  for ( pstWrkHud = &gstHudList; pstWrkHud != NULL && pstWrkHud->pSDL_Rect != pSDL_Rect_HUD; pstWrkHud = pstWrkHud->pstNext);

  if ( pstWrkHud == NULL ) return;

  snprintf(pstWrkHud->szFillRGBA, sizeof(pstWrkHud->szFillRGBA), "%s", kpszFillRGBA);
  snprintf(pstWrkHud->szDrawRGBA, sizeof(pstWrkHud->szDrawRGBA), "%s", kpszDrawRGBA);
}

void vHUD_Draw(SDL_Renderer *renderer, STRUCT_HUD_LIST *pSDL_HUD_List) {
  SDL_Rect *pSDL_RECT_Hud = NULL;

  if( DEBUG_MSGS ) vTraceBegin();

  if ( pSDL_HUD_List == NULL ) return;

  pSDL_RECT_Hud = pSDL_HUD_List->pSDL_Rect;

  SET_RENDER_DRAW_COLOR(renderer, pSDL_HUD_List->szFillRGBA);
  RENDER_RECT(renderer, pSDL_RECT_Hud, "Fill");

  SET_RENDER_DRAW_COLOR(renderer, pSDL_HUD_List->szDrawRGBA);
  RENDER_RECT(renderer, pSDL_RECT_Hud, "Draw");

  if( DEBUG_MSGS ) vTraceEnd();
}

void vHUD_DrawList(SDL_Renderer *renderer) {
  PSTRUCT_HUD_LIST pstWrkHud = NULL;

  for ( pstWrkHud = &gstHudList; pstWrkHud != NULL; pstWrkHud = pstWrkHud->pstNext ) {
    vHUD_Draw(renderer, pstWrkHud);
  }
}

SDL_Rect *pSDL_HUD_AddToList( SDL_Rect *pSDL_Rect, const char *kpszFillRGBA, const char *kpszDrawRGBA ) {
  STRUCT_HUD_LIST *pstWrkHud;

  if ( DEBUG_MSGS ) vTraceBegin();

  if ( pSDL_Rect == NULL ) return NULL;

  for ( pstWrkHud = &gstHudList; pstWrkHud->pstNext != NULL; pstWrkHud = pstWrkHud->pstNext );

  if ( pstWrkHud != &gstHudList || gstHudList.pSDL_Rect != NULL ) {
    if ( (pstWrkHud->pstNext = ( PSTRUCT_HUD_LIST ) malloc( sizeof( STRUCT_HUD_LIST ) ) ) == NULL )
      return NULL;

    pstWrkHud = pstWrkHud->pstNext;
  }

  memset( pstWrkHud, 0x00, sizeof( STRUCT_HUD_LIST ) );

  snprintf(pstWrkHud->szFillRGBA, sizeof(pstWrkHud->szFillRGBA), "%s", kpszFillRGBA);
  snprintf(pstWrkHud->szDrawRGBA, sizeof(pstWrkHud->szDrawRGBA), "%s", kpszDrawRGBA);
  pstWrkHud->pSDL_Rect = pSDL_Rect;
  pstWrkHud->pstNext = NULL;

  if ( DEBUG_MSGS ) vTraceEnd();

  return pstWrkHud->pSDL_Rect;
} /* pSDL_HUD_AddToList */

void vSetTmpHUDRect( SDL_Rect *pSDL_RECT_Hud ) {
  if ( DEBUG_MSGS ) vTraceBegin();

  pSDL_RECT_Hud->w = 2 * INT_WINDOW_WIDTH / 4 - 20;
  pSDL_RECT_Hud->h = COL_RATIO - 10;
  pSDL_RECT_Hud->x = HUD_X_FACTOR * INT_WINDOW_WIDTH - 10;
  pSDL_RECT_Hud->y = INT_WINDOW_HEIGHT - HUD_X_FACTOR * INT_WINDOW_HEIGHT - ( 30 + pSDL_RECT_Hud->h );

  if ( DEBUG_MSGS ) vTraceEnd();
} /* vSetTmpHUDRect */

void vSetCmdHUDRect( SDL_Rect *pSDL_RECT_Hud ) {
  if ( DEBUG_MSGS ) vTraceBegin();

  pSDL_RECT_Hud->x = INT_WINDOW_WIDTH / 4;
  pSDL_RECT_Hud->y = 0;
  pSDL_RECT_Hud->w = 2 * pSDL_RECT_Hud->x;
  pSDL_RECT_Hud->h = COL_RATIO;

  if ( DEBUG_MSGS ) vTraceEnd();
} /* vSetCmdHUDRect */

void vSetButtonHUDRect( SDL_Rect *pSDL_RECT_Hud ) {
  if ( DEBUG_MSGS ) vTraceBegin();

  pSDL_RECT_Hud->x = HUD_X_FACTOR * INT_WINDOW_WIDTH - 10;
  pSDL_RECT_Hud->y = INT_WINDOW_HEIGHT - HUD_X_FACTOR * INT_WINDOW_HEIGHT - 30;
  pSDL_RECT_Hud->w = 2 * INT_WINDOW_WIDTH / 4 - 20;
  pSDL_RECT_Hud->h = COL_RATIO - 10;

  if( DEBUG_MSGS ) vTraceEnd();
} /* vSetButtonHUDRect */


void vSetPlayerHUDRect( SDL_Rect *pSDL_RECT_Hud ) {
  if ( DEBUG_MSGS ) vTraceBegin();

  pSDL_RECT_Hud->x = HUD_X_FACTOR * INT_WINDOW_WIDTH;
  pSDL_RECT_Hud->y = INT_WINDOW_HEIGHT - HUD_X_FACTOR * INT_WINDOW_HEIGHT - 30;
  pSDL_RECT_Hud->w = 2 * INT_WINDOW_WIDTH / 4 - 20;
  pSDL_RECT_Hud->h = COL_RATIO - 10;

  if( DEBUG_MSGS ) vTraceEnd();
}
#endif
