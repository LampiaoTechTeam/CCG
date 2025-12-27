/**
 * hud.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in March 2024
 *
 * Hud data structures and rendering
 * 
 */

#ifndef _HUD_H_
  #define _HUD_H_
  #ifdef USE_SDL2
    #include <trace.h>
    #include <rect.h>
    #include <sdl_api.h>

    #define ZERO_RGB 'Z'
    #define RENDER_RECT(RENDERER, HUDRECT, operation) do { \
              if(strcmp(operation, "Fill") == 0) { \
                  SDL_RenderFillRect(RENDERER, HUDRECT); \
              } else if(strcmp(operation, "Draw") == 0) { \
                  SDL_RenderDrawRect(RENDERER, HUDRECT); \
              } \
            } while(0)
    
    typedef struct STRUCT_HUD_LIST {
      SDL_Rect *pSDL_Rect;
      char szFillRGBA[5];
      char szDrawRGBA[5];
      struct STRUCT_HUD_LIST *pstNext;
    } STRUCT_HUD_LIST, *PSTRUCT_HUD_LIST;
    
    extern STRUCT_HUD_LIST gstHudList;

    extern STRUCT_RECT_DIMENSIONS gstPlayerHUD_Dimensions;
    void vHUD_InitList(void);

    void vHUD_FreeList(void);

    void vHUD_SetRGBA(SDL_Rect *pSDL_Rect_HUD, const char *kpszFillRGBA, const char *kpszDrawRGBA);
    
    void vHUD_Draw(SDL_Renderer *renderer, STRUCT_HUD_LIST *pSDL_HUD_List);

    void vHUD_DrawList(SDL_Renderer *renderer);

    SDL_Rect *pSDL_HUD_AddToList( SDL_Rect *pSDL_Rect, const char *kpszFillRGBA, const char *kpszDrawRGBA );
    
    void vSetTmpHUDRect( SDL_Rect *pSDL_RECT_Hud );

    void vSetCmdHUDRect( SDL_Rect *pSDL_RECT_Hud );

    void vSetButtonHUDRect( SDL_Rect *pSDL_RECT_Hud );

    void vSetPlayerHUDRect( SDL_Rect *pSDL_RECT_Hud );
  #endif
#endif /* _HUD_H_ */
