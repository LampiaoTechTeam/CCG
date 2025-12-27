/** 
 * texture.h
 * 
 * Written by Renato Fermi <repizza@gmail.com>
 *
 * Handles surfaces, textures, images...
 *  |- Mainly data structures and rendering
 */

#ifndef _TEXTURE_H_
  #define _TEXTURE_H_
  #ifdef USE_SDL2

    // #include <card_game.h>

    #define DESTROY_TEXTURES_NONE 0
    #define DESTROY_TEXTURES      1
    
    #define BLENDED_SURFACE_NONE  0
    #define BLENDED_SURFACE_TXTR  1

    typedef struct STRUCT_TEXTURE_LIST {
      SDL_Texture *pSDL_Texture;
      SDL_Rect *pSDL_Rect;
      char *pszImgPath;
      struct STRUCT_TEXTURE_LIST* pstNext;
    } STRUCT_TEXTURE_LIST, *PSTRUCT_TEXTURE_LIST;
    
    extern STRUCT_TEXTURE_LIST gstTextureList;
    
    void vTXTR_RenderCopy( SDL_Renderer *renderer, int iDeg ) {
      STRUCT_TEXTURE_LIST *pstWrkTxtr;

      if ( DEBUG_MSGS ) vTraceBegin();

      for ( pstWrkTxtr = &gstTextureList; pstWrkTxtr != NULL; pstWrkTxtr = pstWrkTxtr->pstNext ) {
        if ( !bStrIsEmpty( pstWrkTxtr->pszImgPath ) && pstWrkTxtr->pSDL_Rect != NULL ) {
          if ( strstr( pstWrkTxtr->pszImgPath, "cat2" ) == NULL ) {
            SDL_RenderCopy( renderer, pstWrkTxtr->pSDL_Texture, NULL, pstWrkTxtr->pSDL_Rect );
          }
          else {
            SDL_RenderCopyEx( renderer, pstWrkTxtr->pSDL_Texture, NULL, pstWrkTxtr->pSDL_Rect, iDeg, NULL, SDL_FLIP_HORIZONTAL );
          }
        }
      }

      if ( DEBUG_MSGS ) vTraceEnd();

      return;
    } /* vTXTR_RenderCopy */

    SDL_Texture *pSDL_TXTR_AddToList( SDL_Renderer *renderer, SDL_Rect *pSDL_Rect, char *pszImgPath, int iSurface ) {
      STRUCT_TEXTURE_LIST *pstWrkTxtr;

      if ( DEBUG_MSGS ) vTraceBegin();
      for ( pstWrkTxtr = &gstTextureList; pstWrkTxtr->pstNext != NULL; pstWrkTxtr = pstWrkTxtr->pstNext );

      if ( pstWrkTxtr != &gstTextureList || gstTextureList.pSDL_Texture != NULL ) {
        if ( (pstWrkTxtr->pstNext = ( PSTRUCT_TEXTURE_LIST ) malloc( sizeof( STRUCT_TEXTURE_LIST ) ) ) == NULL )
          return NULL;

        pstWrkTxtr = pstWrkTxtr->pstNext;
      }

      memset( pstWrkTxtr, 0x00, sizeof( STRUCT_TEXTURE_LIST ) );
      pstWrkTxtr->pszImgPath = NULL;
      if ( !bStrIsEmpty( pszImgPath ) ) {  // Img type Txtr
        SDL_Surface* pSDLSurface = SDL_CreateRGBSurface( 0, 800, 800, 8, 0, 0, 0, 0 );
        pSDLSurface =  pSDL_SRFC_LoadImage( pszImgPath );
        pstWrkTxtr->pSDL_Texture = SDL_CreateTextureFromSurface( renderer, pSDLSurface );
        if ( pstWrkTxtr->pSDL_Texture ){
          vTraceVarArgsFn("SDL Error: %s\n", SDL_GetError());
          return NULL;
        }
        pstWrkTxtr->pszImgPath = pszImgPath;
      }
      else if ( pSDL_Rect != NULL ) {    // Rect type Txtr
        pstWrkTxtr->pSDL_Texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, pSDL_Rect->w, pSDL_Rect->h );
        if ( pstWrkTxtr->pSDL_Texture ){
          vTraceVarArgsFn("SDL Error: %s\n", SDL_GetError());
          return NULL;
        }
      }
      else if ( iSurface ) { // Surface type Txtr
        SDL_Surface* surface = SDL_CreateRGBSurface( 0, 800, 800, 8, 0, 0, 0, 0 );
        SDL_SetSurfaceBlendMode( surface, SDL_BLENDMODE_BLEND );
        pstWrkTxtr->pSDL_Texture = SDL_CreateTextureFromSurface( renderer, surface );
        if ( pstWrkTxtr->pSDL_Texture ){
          vTraceVarArgsFn("SDL Error: %s\n", SDL_GetError());
          return NULL;
        }
        SDL_SetTextureBlendMode( pstWrkTxtr->pSDL_Texture, SDL_BLENDMODE_BLEND );
        SDL_FreeSurface( surface );
      }

      if ( pSDL_Rect != NULL )
        pstWrkTxtr->pSDL_Rect = pSDL_Rect;

      pstWrkTxtr->pstNext = NULL;

      if ( DEBUG_MSGS ) vTraceEnd();

      return pstWrkTxtr->pSDL_Texture;
    } /* pSDL_TXTR_AddToList */

    void vTXTR_FreeList( int iDestroy ) {
      STRUCT_TEXTURE_LIST *pstWrkTxtr;

      if ( DEBUG_MSGS ) vTraceBegin();

      for ( pstWrkTxtr = gstTextureList.pstNext; pstWrkTxtr != NULL; ) {
        STRUCT_TEXTURE_LIST *pstLastTxtr = pstWrkTxtr;
        pstWrkTxtr = pstWrkTxtr->pstNext;
        if ( iDestroy && pstLastTxtr->pSDL_Texture != NULL ) {
          SDL_DestroyTexture(pstLastTxtr->pSDL_Texture);
        }
        free( pstLastTxtr );
        pstLastTxtr = NULL;
      }
      if ( DEBUG_MSGS ) vTraceEnd();
    } /* vTXTR_FreeList */

    void vTXTR_InitList( void ) {
      memset( &gstTextureList, 0x00, sizeof( STRUCT_TEXTURE_LIST ) );
      gstTextureList.pSDL_Texture = NULL;
      gstTextureList.pstNext = NULL;
    } /* vTXTR_InitList */
  #endif
#endif /* _TEXTURE_H_ */
