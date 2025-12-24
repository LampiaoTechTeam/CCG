/**
 * button.h
 *
 * Written by Renato Fermi <repizza@gmail.com>
 * 
 * Button data structures and rendering
 * 
 */

#ifdef USE_SDL2
  #ifndef _BUTTON_H_
    #define _BUTTON_H_
    #include <image.h>
    // #include <card_game.h>

    #define BUTTON_SIZE_RATIO 0.06

    typedef enum ButtonAction {
      FORWARD = 6,
      TURN,
      FIRE_LASER,
      ERASE,
      CONFIRM,
      CONFIGURE,
      CMD_LIST
    } eBtnAction;

    typedef struct STRUCT_BUTTON_LIST {
      SDL_Rect *pSDL_RECT_Button;
      int iAction;
      int iButtonType;
      struct STRUCT_BUTTON_LIST* pstNext;
    } STRUCT_BUTTON_LIST, *PSTRUCT_BUTTON_LIST;

    extern STRUCT_BUTTON_LIST gstButtonList;
    
    int iBUTTON_CheckInteraction( int iXCursor, int iYCursor ) {
      STRUCT_BUTTON_LIST *pstWrkButtonList = NULL;

      for ( pstWrkButtonList = &gstButtonList; pstWrkButtonList != NULL; pstWrkButtonList = pstWrkButtonList->pstNext ) {
        SDL_Rect *pSDL_RECT_Btn = pstWrkButtonList->pSDL_RECT_Button;

        if ( pSDL_RECT_Btn == NULL )
          break;
          
        if ( bAreCoordsInSDL_Rect(pSDL_RECT_Btn, iXCursor, iYCursor) ) {
          return pstWrkButtonList->iAction;
        }
      }
      return 0;
      // return REDRAW_NONE;
    } /* iBUTTON_CheckInteraction */

    void vBUTTON_InitList( void ) {
      memset( &gstButtonList, 0x00, sizeof(STRUCT_BUTTON_LIST ) );
      gstButtonList.pstNext = NULL;
    } /* vBUTTON_InitList */
    
    void vBUTTON_FreeList( void ) {
      STRUCT_BUTTON_LIST *pstWrkButton = NULL;

      if ( DEBUG_MSGS ) vTraceBegin();
      
      for ( pstWrkButton = gstButtonList.pstNext; pstWrkButton != NULL; ) {
        STRUCT_BUTTON_LIST *pstLastButton = pstWrkButton;
        pstWrkButton = pstWrkButton->pstNext;
        free( pstLastButton->pSDL_RECT_Button);
        free( pstLastButton );
      }
      
      vBUTTON_InitList();

      if ( DEBUG_MSGS ) vTraceEnd();
    } /* vBUTTON_FreeList */

    void vBUTTON_DrawList( SDL_Renderer *renderer ) {
      STRUCT_BUTTON_LIST *pstWrkButton = NULL;

      for ( pstWrkButton = &gstButtonList; pstWrkButton != NULL; pstWrkButton = pstWrkButton->pstNext ) {
        // vBUTTON_Draw( renderer, pstWrkButton->pSDL_RECT_Button, pstWrkButton->iButtonType );
      }
    } /* vBUTTON_DrawList */
    void vBUTTON_SetDimensions( SDL_Rect *pSDL_RECT_Btn, int iXTrslt, int iYTrslt ) {
      if ( DEBUG_MSGS ) vTraceBegin();

      pSDL_RECT_Btn->w = BUTTON_SIZE_RATIO * INT_WINDOW_WIDTH;
      pSDL_RECT_Btn->h = BUTTON_SIZE_RATIO * INT_WINDOW_HEIGHT;
      pSDL_RECT_Btn->x = pSDL_RECT_Btn->w + iXTrslt;
      pSDL_RECT_Btn->y = INT_WINDOW_HEIGHT - pSDL_RECT_Btn->h - iYTrslt;

      vRECT_Trace(pSDL_RECT_Btn);

      if ( DEBUG_MSGS ) vTraceEnd();
    } /* vBUTTON_SetDimensions */
    /**
     *  ---------------------------------
     * |  __   __   __      __      __   | 
     * | |Wl| |Tr| |Fr|    |BS|    |OK|  |
     * | |__| |__| |__|    |__|    |__|  |
     * |_________________________________|
    */
    void vBUTTON_InitButtonHUDRect(){
      SDL_Rect SDL_RECT_ButtonArrowRight;
      SDL_Rect SDL_RECT_ButtonTurnArrow;
      SDL_Rect SDL_RECT_ButtonFireLaser;
      SDL_Rect SDL_RECT_ButtonUndoLast;
      SDL_Rect SDL_RECT_ButtonConfirm;
      SDL_Rect SDL_RECT_ButtonConfigure;
      int iXTranslation = 0;
      int iYTranslation = 20;

      /* Set button size and position [x,y] and width + height **/
      vBUTTON_SetDimensions( &SDL_RECT_ButtonArrowRight, iXTranslation , iYTranslation);
      iXTranslation += SDL_RECT_ButtonArrowRight.w + 10;
      vBUTTON_SetDimensions( &SDL_RECT_ButtonTurnArrow,  iXTranslation, iYTranslation);
      iXTranslation += SDL_RECT_ButtonTurnArrow.w  + 10;
      vBUTTON_SetDimensions( &SDL_RECT_ButtonFireLaser,  iXTranslation, iYTranslation);
      iXTranslation += SDL_RECT_ButtonFireLaser.w  + 50;
      vBUTTON_SetDimensions( &SDL_RECT_ButtonUndoLast,   iXTranslation, iYTranslation);
      iXTranslation += SDL_RECT_ButtonUndoLast.w   + 50;
      vBUTTON_SetDimensions( &SDL_RECT_ButtonConfirm,    iXTranslation, iYTranslation);
      iXTranslation += SDL_RECT_ButtonConfirm.w    + 100;
      vBUTTON_SetDimensions( &SDL_RECT_ButtonConfigure,  iXTranslation, iYTranslation);

    
      /* Add them to list **/
      iBUTTON_AddToList( &SDL_RECT_ButtonArrowRight   , FORWARD    );
      iBUTTON_AddToList( &SDL_RECT_ButtonTurnArrow    , TURN       );
      iBUTTON_AddToList( &SDL_RECT_ButtonFireLaser    , FIRE_LASER );
      iBUTTON_AddToList( &SDL_RECT_ButtonUndoLast     , ERASE      );
      iBUTTON_AddToList( &SDL_RECT_ButtonConfirm      , CONFIRM    );
      iBUTTON_AddToList( &SDL_RECT_ButtonConfigure    , CONFIGURE  );
    }
    
  #endif /* _BUTTON_H_ */
#endif
