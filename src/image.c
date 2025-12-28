#ifdef USE_SDL2
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
  #include <stdio.h>
  #include <trace.h>
  #include <deck.h>
  #include <image.h>
  #include <sys_interface.h>
  #include <string.h>
  #include <stdlib.h>
  #include <card_game.h>
  

  STRUCT_IMAGE stWrkImage;
  STRUCT_IMAGE gstImages[MAX_IMAGES];
  int giImageCount = 0;

  int icbackImageXml(xmlNode* pstNode, void* pData __attribute__((unused)));
  STRUCT_XML astImageXml[] = {
    { "IMAGE"     , XMLTYPE_PROC  , 0                         , NULL               , icbackImageXml },
    { "IMAGE_LIST", XMLTYPE_PROC  , 0                         , NULL               , icbackImageXml },
    { "TYPE"      , XMLTYPE_INT   , 0                         , &stWrkImage.iType  , NULL           },
    { "PATH"      , XMLTYPE_STRING, sizeof(stWrkImage.szPath) , stWrkImage.szPath  , NULL           },
    { "CARD_ID"   , XMLTYPE_INT   , sizeof(stWrkImage.iCardId), &stWrkImage.iCardId, NULL           },
    { NULL        , XMLTYPE_NULL  , 0                         , NULL               , NULL           }
  };
  int icbackImageXml(xmlNode* pstNode, void* pData __attribute__((unused))) {
    if ( !strcasecmp((char*)pstNode->name, "IMAGE_LIST") ) {
      memset(&stWrkImage, 0x00, sizeof(stWrkImage));
      iParseXmlFields(pstNode, astImageXml);
    }
    if ( !strcasecmp((char*)pstNode->name, "IMAGE") ) {
      memset(&stWrkImage, 0x00, sizeof(stWrkImage));
      if ( giImageCount >= MAX_IMAGES ) return 1;
      iParseXmlFields(pstNode, astImageXml);
      gstImages[giImageCount] = stWrkImage;
      if ( DEBUG_XML_MSGS ) vIMG_TraceIdx(giImageCount);
      giImageCount++;
    }
    return 1;
  }

  SDL_Surface *pSDL_SRFC_LoadImage(char *pszImgPath) {
    SDL_Surface *SDL_SRFC_Img = IMG_Load(pszImgPath);

    if ( DEBUG_LVL_MORE_DETAILS ) vTraceVarArgsFn("begin");

    if ( SDL_SRFC_Img == NULL ) {
      if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("Error loading image: %s\n", IMG_GetError());
      if ( DEBUG_LVL_MORE_DETAILS ) vTraceVarArgsFn("end return NULL");

      return NULL;
    }

    if ( DEBUG_LVL_MORE_DETAILS ) vTraceVarArgsFn("end");

    return SDL_SRFC_Img;
  } /* pSDL_SRFC_LoadImage */

  int bLoadImgListFromFile() {
    char szPath[_MAX_PATH+_MAX_PATH];
    int bLoaded = FALSE;
    if ( DEBUG_LVL_MORE_DETAILS ) vTraceVarArgsFn("begin");

    sprintf(szPath, "%s/%s/%s", gszRootPathFromBin, CONF_DIR, IMG_PATH_TITLE);
    bLoaded = bLoadXmlFromFile(szPath, astImageXml);

    if ( DEBUG_LVL_MORE_DETAILS ) vTraceVarArgsFn("end");
    return bLoaded;
  }

  SDL_Texture *pSDL_LoadTextureFromPath(SDL_Renderer *pSDL_Renderer, char *pszPath){
    SDL_Surface *pSDL_Srfc;
    SDL_Texture *pSDL_Txtr;
    char szPath[_MAX_PATH*4];
    char szName[_MAX_PATH];
    char szExt[_MAX_PATH];

    if ( bStrIsEmpty(pszPath) )
      return NULL;

    vSetRootPathFromCwd();
    
    iDIR_SplitFilename(pszPath, szPath, szName, szExt);
    snprintf(szPath, sizeof(szPath), "%s/%s/%s%s", gszRootPathFromBin, gstGlobalPrm.szAssetsDir, szName, szExt);
    if ( iDIR_IsDir(szPath) < 0 ){
      snprintf(szPath, sizeof(szPath), "../%s/%s%s", gstGlobalPrm.szAssetsDir, szName, szExt);
    }

    pSDL_Srfc = pSDL_SRFC_LoadImage(szPath);
    if ( pSDL_Srfc == NULL ) {
      if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("IMG_Load falhou: [%s] err=%s", szPath, IMG_GetError());
      return NULL;
    }

    pSDL_Txtr = SDL_CreateTextureFromSurface(pSDL_Renderer, pSDL_Srfc);
    SDL_FreeSurface(pSDL_Srfc);

    if ( pSDL_Txtr == NULL ) {
      if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("SDL_CreateTextureFromSurface falhou: %s", SDL_GetError());
      return NULL;
    }

    /* NEW: alpha blend para PNGs com transparência */
    SDL_SetTextureBlendMode(pSDL_Txtr, SDL_BLENDMODE_BLEND);

    return pSDL_Txtr;
  }

  int iIMG_LoadAll(SDL_Renderer *pSDL_Renderer){
    STRUCT_IMAGE *pstImg;
    int bAnyLoaded = FALSE;
    int iImageCount = 0;
    while ( iImageCount < giImageCount ) {
      pstImg = &gstImages[iImageCount];
      if ( !bStrIsEmpty(pstImg->szPath) ) {
        if ( iImageCount >= giImageCount ) break;
        pstImg->pSDL_Txtr = pSDL_LoadTextureFromPath(pSDL_Renderer, pstImg->szPath);
        if ( pstImg->pSDL_Txtr != NULL ) {
          pstImg->pSDL_IMGRect.x = 0;
          pstImg->pSDL_IMGRect.y = 0;
          pstImg->pSDL_IMGRect.w = 280;
          pstImg->pSDL_IMGRect.h = 300;
          bAnyLoaded = TRUE;
        }
      }
      iImageCount++;
    }
    if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("iIMG_LoadAll: giImageCount=%d", iImageCount);
    if ( DEBUG_LVL_MORE_DETAILS ) vIMG_TraceList();
    return bAnyLoaded;
  }

  void vIMG_TraceIdx(int iIdx){
    vTraceVarArgsFn("========= IMG LIST =========");
    vTraceVarArgsFn(" Type=[%d]", gstImages[iIdx].iType);
    vTraceVarArgsFn(" Id  =[%d]", gstImages[iIdx].iCardId);
    vTraceVarArgsFn(" Path=[%s]", gstImages[iIdx].szPath);
    vTraceVarArgsFn("============================");
  }
  void vIMG_TraceList(){
    int ii = 0;
    while ( ii < giImageCount ) {
      vIMG_TraceIdx(ii);
      ii++;
    }
  }

  STRUCT_IMAGE *pIMG_FindFirstById(int iCardId){
    int ii = 0;

    if ( iCardId == CARD_NONE )
      return NULL;

    while ( ii < giImageCount ) {
      if ( gstImages[ii].iCardId == iCardId )
          return &gstImages[ii];
      
      ii++;
    }

    return NULL;
  }

  STRUCT_IMAGE *pIMG_GetNextByType(int iType, int iIndex){
    int i;
    int iCount;

    i = 0;
    iCount = 0;

    while ( i < giImageCount ) {
      if ( gstImages[i].iType == iType ) {
        if ( iCount == iIndex )
          return &gstImages[i];
        iCount++;
      }
      i++;
    }

    return NULL;
  }
  void vIMG_UnloadAll(void){
    int i;

    i = 0;
    while ( i < giImageCount ) {
      if ( gstImages[i].pSDL_Txtr != NULL ) {
        SDL_DestroyTexture(gstImages[i].pSDL_Txtr);
        gstImages[i].pSDL_Txtr = NULL;
      }
      i++;
    }

    giImageCount = 0;
    memset(gstImages, 0, sizeof(gstImages));
  }

#endif
