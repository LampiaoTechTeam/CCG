#ifndef SDL_API_H
  #define SDL_API_H
  #ifdef USE_SDL2
  // #include <card_game.h>
  #include <font.h>
  #include <dialog.h>
    
    extern int giSelectedMonster;
    /** Bitwise redraw options */
    #define REDRAW_NONE    0x00
    #define REDRAW_DIALOG  0x01
    #define REDRAW_TABLE   0x02
    #define REDRAW_ALL     0x03

    /** COLOR MACRO SESSION  */
    #define OPACITY_OPAQUE           255  /* fully solid          */
    #define OPACITY_SEMI_OPAQUE      180  /* slightly translucent */
    #define OPACITY_SEMI_TRANSPARENT 100  /* mostly see-through   */
    #define OPACITY_TRANSPARENT        0  /* invisible            */

    #define SDL_RGB_BLACK        ((unsigned char[3]){   0,   0,   0 })
    #define SDL_RGB_TABLE_GREEN  ((unsigned char[3]){   0, 100,   0 })
    #define SDL_RGB_WHITE        ((unsigned char[3]){ 255, 255, 255 })
    #define SDL_RGB_ORANGE       ((unsigned char[3]){ 255, 165,   0 })
    #define SDL_RGB_RED_BLOOD    ((unsigned char[3]){ 180,  40,  40 })
    #define SDL_RGB_TABLE_BORDER ((unsigned char[3]){ 139,  69,  19 })
    #define SDL_RGB_GRAY40       ((unsigned char[3]){  40,  40,  40 })
    #define SDL_RGB_GRAY60       ((unsigned char[3]){  60,  60,  60 })
    #define SDL_RGB_GRAY180      ((unsigned char[3]){ 180, 180, 180 })
    #define SDL_RGB_GRAY200      ((unsigned char[3]){ 200, 200, 200 })

    #define SDL_COLOR_BG           SDL_COLOR_BLACK
    #define SDL_COLOR_DIALOG_BG    SDL_COLOR_GRAY40_A220
    #define SDL_COLOR_DIALOG_BRDR  SDL_COLOR_GRAY200
    #define SDL_COLOR_BTN_BG       SDL_COLOR_GRAY60_A220
    #define SDL_COLOR_BTN_BRDR     SDL_COLOR_GRAY180
    #define SDL_COLOR_MESA         SDL_COLOR_TABLE_GREEN
    #define SDL_COLOR_MESA_BRDR    SDL_COLOR_TABLE_BORDER
    #define SDL_COLOR_MONSTER      SDL_RGB_RED_BLOOD
    
    #define SDL_COLOR_FROM_RGB_OPACITY(RGB, OPACITY) \
      ((unsigned char[4]){ (RGB)[0], (RGB)[1], (RGB)[2], (unsigned char)(OPACITY) })

    #define SET_RENDER_DRAW_COLOR(RENDERER, RGBA) \
      SDL_SetRenderDrawColor( \
        (RENDERER), \
        (unsigned char)(RGBA)[0], \
        (unsigned char)(RGBA)[1], \
        (unsigned char)(RGBA)[2], \
        (unsigned char)(RGBA)[3] \
      )
    /** COLOR MACRO SESSION - END */
    void vSDL_DrawTextShadow(SDL_Renderer *pSDL_Renderer,
                         const char *pszTxt,
                         int iX, int iY,
                         SDL_Color stFG,
                         SDL_Color stShadow,
                         int iOffX, int iOffY);
    void vSDL_SetupMain(SDL_Renderer **pSDL_Renderer, SDL_Window **pSDL_Window);
    void vSDL_MainInit();
    void vSDL_MainLoop(int *pbRunning, SDL_Event *pSDL_Event, SDL_Renderer *pSDL_Renderer, PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCt);
    void vSDL_MainQuit(void);
    void vSDL_ToggleFullscreen(void); 
    void vSDL_DrawText(SDL_Renderer *pSDL_Renderer, const char *szTxt, int iX, int iY, SDL_Color stCol);
    void vSDL_DrawTable(SDL_Renderer *pSDL_Renderer, PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCt);
    void vSDL_DrawHUD(SDL_Renderer *pSDL_Renderer, PSTRUCT_PLAYER pstPlayer);
    void vSDL_DrawRectShadow(SDL_Renderer *pSDL_Renderer, SDL_Rect *pstRect, int iOffX, int iOffY, Uint8 uAlpha);
    void vSDL_DialogDraw(SDL_Renderer *pSDL_Renderer, PSTRUCT_SDL_DIALOG_LAYOUT pstLayout);
    void vSDL_DialogComputeLayout(int iWinW, int iWinH, PSTRUCT_SDL_DIALOG_LAYOUT pstLayout);
    int iDlgMaybeFollowTail(int iVisibleCount);
    int iSDL_HandIndexFromPoint(int iX, int iY, PSTRUCT_DECK pstDeck);
    int iSDL_MonsterIndexFromPoint(int iX, int iY, PSTRUCT_MONSTER pastMonsters, int iMonsterCt);
    int bAreCoordsInSDL_Rect( SDL_Rect *pSDL_RECT, int iX, int iY );
    int iSDL_DialogHandleMouse(SDL_Event *pEv, PSTRUCT_SDL_DIALOG_LAYOUT pLayout);
    extern STRUCT_SDL_DIALOG_LAYOUT gstDialogLayout;
    typedef struct { Uint32 type; const char *name; } SDLEventName;
    extern int gbSelectingTarget;
    static const SDLEventName gEventNames[] = {
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
  #endif
  
  #ifndef LINUX
    #define CCG_Main SDL_main
  #else
    #define CCG_Main main
  #endif
#endif
 
  
