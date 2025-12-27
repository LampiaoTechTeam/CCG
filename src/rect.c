/**
 * rect.h
 *
 * Written by Renato Fermi <repiazza@gmail.com> in March 2024
 *
 * Rect handling
 *
 */

#include <stdio.h>
#include <sys_interface.h>
#include <card_game.h>
#ifdef USE_SDL2
#include <SDL2/SDL.h>
  #include <SDL2/SDL_rect.h>
#endif
#include <debuff.h>
#include <deck.h>
#include <monster.h>
#include <player.h>
#include <rect.h>

#ifdef USE_SDL2
  STRUCT_RECT_DIMENSIONS gstButtonHUD_Dimension;
  STRUCT_RECT_DIMENSIONS gstCmdHUD_Dimensions;
  STRUCT_RECT_DIMENSIONS gstTmpHUD_Dimensions;

  /**
    * Set
    */
  int iRECT_SetDimensions(SDL_Rect *pSDL_Rect, STRUCT_RECT_DIMENSIONS *pst_RECT_Dimensions) {
    if (pSDL_Rect == NULL)
      return FALSE;

    pSDL_Rect->x = pst_RECT_Dimensions->stX.dFactor + pst_RECT_Dimensions->stX.dAddAmount;
    pSDL_Rect->y = pst_RECT_Dimensions->stY.dFactor + pst_RECT_Dimensions->stY.dAddAmount;
    pSDL_Rect->w = pst_RECT_Dimensions->stW.dFactor + pst_RECT_Dimensions->stW.dAddAmount;
    pSDL_Rect->h = pst_RECT_Dimensions->stH.dFactor + pst_RECT_Dimensions->stH.dAddAmount;
    return TRUE;
  }


  void vRECT_InitGlobalDimensions(void) {
    (&gstPlayerHUD_Dimensions)->stX.dFactor = 0;
    (&gstPlayerHUD_Dimensions)->stX.dAddAmount = 0;
    (&gstPlayerHUD_Dimensions)->stY.dFactor = 0;
    (&gstPlayerHUD_Dimensions)->stY.dAddAmount = 0;
    (&gstPlayerHUD_Dimensions)->stW.dFactor = INT_WINDOW_WIDTH;
    (&gstPlayerHUD_Dimensions)->stW.dAddAmount = 0;
    (&gstPlayerHUD_Dimensions)->stH.dFactor = INT_WINDOW_HEIGHT;
    (&gstPlayerHUD_Dimensions)->stH.dAddAmount = 0;
  }

  void vRECT_InitPlayerHud()
  {
    vRECT_InitGlobalDimensions();
    iRECT_SetDimensions(&gSDL_Player_Rect, &gstPlayerHUD_Dimensions);
  }
  void vCalculateHUDRect(SDL_Rect *pSDL_Rect, double xFactor, double yFactor, double widthFactor, double heightFactor)
  {
    if (pSDL_Rect == NULL)
      return;

    pSDL_Rect->x = (int)(xFactor * INT_WINDOW_WIDTH);
    pSDL_Rect->y = (int)(yFactor * INT_WINDOW_HEIGHT);
    pSDL_Rect->w = (int)(widthFactor * INT_WINDOW_WIDTH);
    pSDL_Rect->h = (int)(heightFactor * INT_WINDOW_HEIGHT);
  }
#endif

