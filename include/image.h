/**
 * image.h
 *
 * Written by Renato Fermi <repizza@gmail.com>
 * 
 * Temporary path handling 
 * TODO: rework this logic.
 */

#ifdef USE_SDL2
  #ifndef _IMAGE_H_
    #define _IMAGE_H_

    #include <card_game.h>  
    #include <SDL2/SDL_image.h>

    #define IMAGE_TYPE_NONE        0 
    #define IMAGE_TYPE_PLAYER_CARD 1
    #define IMAGE_TYPE_MONSTER     2
    #define IMAGE_TYPE_TILELIST    3

    #define CONF_DIR       "conf"
    #define IMG_PATH_TITLE "images.dat"

    typedef struct STRUCT_IMAGE_CONF{
      int iType;
      char *pszPath;
      struct STRUCT_IMAGE_CONF *pstNext;
    }STRUCT_IMAGE_CONF, *PSTRUCT_IMAGE_CONF;

    extern STRUCT_IMAGE_CONF gstImgConf;
    int bLoadImgListFromFile();
    void vFreeImgList();
    void vInitImgConf();
    void vTraceImgList();
    SDL_Surface *pSDL_SRFC_LoadImage(char *pszImgPath);
  #endif /* _IMAGE_H */
  #ifndef _IMAGE_ATLAS_H_
    #define _IMAGE_ATLAS_H_

    typedef struct STRUCT_IMG_ATLAS {
      SDL_Texture *pSDL_Txtr;
      int iTileW;
      int iTileH;
      int iCols;
      int iRows;
      int iCount;
    } STRUCT_IMG_ATLAS, *PSTRUCT_IMG_ATLAS;

    extern STRUCT_IMG_ATLAS gstAtlasCards;

    int iAtlas_InitFromImgList(SDL_Renderer *pSDL_Renderer);
    int bAtlas_SrcRectFromIndex(PSTRUCT_IMG_ATLAS pstAtlas, int iIndex, SDL_Rect *pstSrc);
    void vAtlas_Render(SDL_Renderer *pSDL_Renderer,
                       PSTRUCT_IMG_ATLAS pstAtlas,
                       int iIndex,
                       SDL_Rect *pstDst);
    void vAtlas_RenderScaled(SDL_Renderer *pSDL_Renderer,
                          PSTRUCT_IMG_ATLAS pstAtlas,
                          int iIndex,
                          SDL_Rect *pstDst,
                          double dScale,
                          int bKeepAspect);
  #endif /* _IMAGE_ATLAS_H_ */
#endif