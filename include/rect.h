/**
 * rect.h
 *
 * Written by Renato Fermi <repiazza@gmail.com> in March 2024
 *
 * Rect handling
 *
 */

#ifndef _RECT_H_
  #define _RECT_H_
  #ifdef USE_SDL2
    // #include <card_game.h>

    // Definições de proporções como percentuais da tela
    #define HUD_X_FACTOR      0.06  // Porcentagem da largura da tela
    #define HUD_Y_FACTOR      0.04  // Porcentagem da altura da tela
    #define HUD_WIDTH_FACTOR  0.5   // 50% da largura da tela
    #define HUD_HEIGHT_FACTOR 0.05  // 5% da altura da tela

    typedef struct STRUCT_RECT_VALUES
    {
      double dFactor;   // Angular coefficient
      double dAddAmount;
    } STRUCT_RECT_VALUES, *PSTRUCT_RECT_VALUES;

    typedef struct STRUCT_RECT_DIMENSIONS
    {
      STRUCT_RECT_VALUES stX;
      STRUCT_RECT_VALUES stY;
      STRUCT_RECT_VALUES stW;
      STRUCT_RECT_VALUES stH;
    } STRUCT_RECT_DIMENSIONS, PSTRUCT_RECT_DIMENSIONS;

    extern STRUCT_RECT_DIMENSIONS gstButtonHUD_Dimension;
    extern STRUCT_RECT_DIMENSIONS gstCmdHUD_Dimensions;
    extern STRUCT_RECT_DIMENSIONS gstTmpHUD_Dimensions;
    extern STRUCT_RECT_DIMENSIONS gstPlayerHUD_Dimensions;

    /**
     * Set
     */
    int iRECT_SetDimensions(SDL_Rect *pSDL_Rect, STRUCT_RECT_DIMENSIONS *pst_RECT_Dimensions);
    void vRECT_InitGlobalDimensions(void);
    void vRECT_InitPlayerHud();
    void vCalculateHUDRect(SDL_Rect *pSDL_Rect, double xFactor, double yFactor, double widthFactor, double heightFactor);

  #endif
#endif
