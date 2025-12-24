#ifndef _IMAGE_H_
  #define _IMAGE_H_
  #ifdef USE_SDL2

    // #include <card_game.h>
    // #include <SDL2/SDL.h>
    // #include <SDL2/SDL_image.h>
    #include <xml.h>

    #define MAX_IMAGES 64

    #define IMAGE_TYPE_NONE        0
    #define IMAGE_TYPE_PLAYER_CARD 1
    #define IMAGE_TYPE_MONSTER     2
    #define IMAGE_TYPE_TILELIST    3

    #define CONF_DIR       "conf"
    #define IMG_PATH_TITLE "image.xml"

    typedef struct STRUCT_IMAGE {
      int          iType;
      char         szPath[256];
      char iCardId;
      SDL_Texture  *pSDL_Txtr;
      SDL_Rect     pSDL_IMGRect; /* SEMPRE 280x300 */
      SDL_Rect     pSDL_IMGDst;  /* opcional */
    }STRUCT_IMAGE;

    extern STRUCT_IMAGE gImages[MAX_IMAGES];
    extern int giImageCount;

    int  bLoadImgListFromFile();
    void vIMG_TraceList();
    void vIMG_TraceIdx(int iIdx);

    SDL_Surface *pSDL_SRFC_LoadImage(char *pszImgPath);
    SDL_Texture *pSDL_LoadTextureFromPath(SDL_Renderer *pSDL_Renderer, char *pszPath);
    SDL_Texture *pSDL_LoadTextureFromPath(SDL_Renderer *pSDL_Renderer, char *pszPath);
    int iIMG_LoadAll(SDL_Renderer *pSDL_Renderer);
    void vIMG_UnloadAll(void);
    STRUCT_IMAGE *pIMG_GetNextByType(int iType, int iIndex);
    STRUCT_IMAGE *pIMG_FindFirstById(int iCardId);
    void vIMG_RenderScaled(SDL_Renderer *pSDL_Renderer,
                           STRUCT_IMAGE *pstImg,
                           SDL_Rect *pstDst,
                           double dScale,
                           int bKeepAspect);

  #endif
#endif
