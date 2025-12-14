#ifdef USE_SDL2
  #include <trace.h>
  #include <image.h>
  #include <sys_interface.h>
  #include <string.h>

  STRUCT_IMAGE_CONF gstImgConf;
  STRUCT_IMG_ATLAS gstAtlasCards;

  void vInitImgConf(){
    
    vTraceVarArgsFn("begin");
    memset(&gstImgConf, 0, sizeof(STRUCT_IMAGE_CONF));
    gstImgConf.pstNext = NULL;
    
    vTraceVarArgsFn("end");
  }

  PSTRUCT_IMAGE_CONF pstCreateImgNode(){
    PSTRUCT_IMAGE_CONF pstImgConf = &gstImgConf;

    if ( !bStrIsEmpty(pstImgConf->pszPath) ){
      for ( ; pstImgConf->pstNext != NULL; pstImgConf = pstImgConf->pstNext );

      if ( (pstImgConf->pstNext = (PSTRUCT_IMAGE_CONF) malloc(sizeof(STRUCT_IMAGE_CONF))) == NULL )
        return NULL;

      pstImgConf = pstImgConf->pstNext;
      memset(pstImgConf, 0, sizeof(STRUCT_IMAGE_CONF));
      pstImgConf->pstNext = NULL;
    }

    return pstImgConf;
  }

  int bAddImg2Mem(int iImgType, char *pszPath, int iLen){
    PSTRUCT_IMAGE_CONF pstImgConf;
    
    if ( (pstImgConf = pstCreateImgNode()) == NULL ){
      return FALSE;
    }

    pstImgConf->iType = iImgType;
    pstImgConf->pszPath = (char *)malloc(iLen+2);
    memset(pstImgConf->pszPath, 0, iLen+2);
    sprintf(pstImgConf->pszPath, "%s/%s", ASSETS_BASE_DIR, pszPath);

    return TRUE;
  }

  SDL_Surface *pSDL_SRFC_LoadImage(char *pszImgPath) {
    SDL_Surface *SDL_SRFC_Img = IMG_Load(pszImgPath);
    
    vTraceVarArgsFn("begin");
  
    if ( SDL_SRFC_Img == NULL ) {
      vTraceVarArgsFn("Error loading image: %s\n", IMG_GetError());
      vTraceVarArgsFn("%s - end return NULL", __func__);
  
      return NULL;
    }
  
    vTraceVarArgsFn("end");
  
    return SDL_SRFC_Img;
  } /* pSDL_SRFC_LoadImage */

  int bLoadImgListFromFile(){
    char szPath[MAX_PATH+MAX_PATH];
    char szLine[1024];
    char *pTok;
    int bLoaded = FALSE;
    FILE *pfImg;
    
    vTraceVarArgsFn("begin");

    sprintf(szPath, "%s/%s/%s", gszRootPathFromBin, CONF_DIR, IMG_PATH_TITLE);

    if ( bOpenFile(&pfImg, szPath, "r") == FALSE ){
      vTraceVarArgsFn("Img Conf file not found[%s]", szPath);
      return FALSE;
    }    

    while ( fgets(szLine, sizeof(szLine), pfImg) ){
      int iType;

      if ( (pTok = strtok(szLine, "|\n")) == NULL ) continue;

      iType = atoi(pTok);
      pTok++;
      if ( (pTok = strtok(NULL, "|\n")) == NULL ) continue;

      if ( bAddImg2Mem(iType, pTok, strlen(pTok)) == FALSE ) continue;
      
      bLoaded = TRUE;
    }

    if ( bLoaded )
      vTraceImgList();

    bCloseFile(&pfImg);
    vTraceVarArgsFn("end");

    return bLoaded;
  }

  void vTraceImgList(){
    PSTRUCT_IMAGE_CONF pstWrkImg;

    vTraceVarArgsFn("--- Images ---");
    for ( pstWrkImg = &gstImgConf; pstWrkImg != NULL; pstWrkImg = pstWrkImg->pstNext ){
      if ( !bStrIsEmpty(pstWrkImg->pszPath) )
        vTraceVarArgsFn(" type=%d path=[%s]", pstWrkImg->iType, pstWrkImg->pszPath);
    }
    vTraceVarArgsFn("--------------");

  }
  void vFreeImgList(){
    PSTRUCT_IMAGE_CONF pstImgConf = &gstImgConf;
    PSTRUCT_IMAGE_CONF pstLast;

    while( pstImgConf != NULL){
      if ( pstImgConf->pszPath != NULL )
        free(pstImgConf->pszPath);
      
      pstLast = pstImgConf;
      pstImgConf = pstImgConf->pstNext;
      if ( pstLast == &gstImgConf ) continue;

      free(pstLast);
    }
    vInitImgConf();
  }

  static SDL_Texture *pSDL_LoadTextureFromPath(SDL_Renderer *pSDL_Renderer, char *pszPath) {
    SDL_Surface *pSDL_Srfc;
    SDL_Texture *pSDL_Txtr;

    if ( bStrIsEmpty(pszPath) )
      return NULL;

    pSDL_Srfc = pSDL_SRFC_LoadImage(pszPath);
    if ( pSDL_Srfc == NULL ) {
      vTraceVarArgsFn("IMG_Load falhou: [%s] err=%s", pszPath, IMG_GetError());
      return NULL;
    }

    pSDL_Txtr = SDL_CreateTextureFromSurface(pSDL_Renderer, pSDL_Srfc);
    SDL_FreeSurface(pSDL_Srfc);

    if ( pSDL_Txtr == NULL ) {
      vTraceVarArgsFn("SDL_CreateTextureFromSurface falhou: %s", SDL_GetError());
      return NULL;
    }

    /* NEW: alpha blend para PNGs com transparência */
    SDL_SetTextureBlendMode(pSDL_Txtr, SDL_BLENDMODE_BLEND);

    return pSDL_Txtr;
  }


  int iAtlas_InitFromImgList(SDL_Renderer *pSDL_Renderer) {
    PSTRUCT_IMAGE_CONF pstWrk;
    int iOk;
    int iTileW;
    int iTileH;

    memset(&gstAtlasCards, 0, sizeof(STRUCT_IMG_ATLAS));

    iOk = FALSE;
    iTileW = 300; /* cada carta 300x300 na imagem fornecida */
    iTileH = 300;

    for ( pstWrk = &gstImgConf; pstWrk != NULL; pstWrk = pstWrk->pstNext ) {
      if ( bStrIsEmpty(pstWrk->pszPath) )
        continue;
      if ( pstWrk->iType == IMAGE_TYPE_TILELIST ) {
        SDL_Texture *pSDL_Txtr;
        int iW;
        int iH;

        pSDL_Txtr = pSDL_LoadTextureFromPath(pSDL_Renderer, pstWrk->pszPath);
        if ( pSDL_Txtr == NULL ) {
          vTraceVarArgsFn("Falhou carregar atlas [%s]", pstWrk->pszPath);
          continue;
        }

        SDL_QueryTexture(pSDL_Txtr, NULL, NULL, &iW, &iH);

        gstAtlasCards.pSDL_Txtr = pSDL_Txtr;
        gstAtlasCards.iTileW = iTileW;
        gstAtlasCards.iTileH = iTileH;
        gstAtlasCards.iCols  = (iTileW > 0) ? (iW / iTileW) : 0;
        gstAtlasCards.iRows  = (iTileH > 0) ? (iH / iTileH) : 0;
        gstAtlasCards.iCount = gstAtlasCards.iCols * gstAtlasCards.iRows;

        vTraceVarArgsFn("Atlas cards: %dx%d tiles=%dx%d count=%d",
                        iW, iH, iTileW, iTileH, gstAtlasCards.iCount);
        iOk = TRUE;
        break;
      }
    }
    return iOk;
  }

  int bAtlas_SrcRectFromIndex(PSTRUCT_IMG_ATLAS pstAtlas, int iIndex, SDL_Rect *pstSrc) {
    int iCol;
    int iRow;

    if ( pstAtlas == NULL || pstAtlas->pSDL_Txtr == NULL )
      return FALSE;

    if ( iIndex < 0 || iIndex >= pstAtlas->iCount )
      return FALSE;

    iCol = iIndex % pstAtlas->iCols;
    iRow = iIndex / pstAtlas->iCols;

    pstSrc->x = iCol * pstAtlas->iTileW;
    pstSrc->y = iRow * pstAtlas->iTileH;
    pstSrc->w = pstAtlas->iTileW;
    pstSrc->h = pstAtlas->iTileH;

    return TRUE;
  }

  void vAtlas_Render(SDL_Renderer *pSDL_Renderer,
                      PSTRUCT_IMG_ATLAS pstAtlas,
                      int iIndex,
                      SDL_Rect *pstDst) {
    SDL_Rect stSrc;
    int bOk;
    if ( pSDL_Renderer == NULL || pstAtlas == NULL || pstDst == NULL )
      return;
    if ( pstAtlas->pSDL_Txtr == NULL )
      return;

    bOk = bAtlas_SrcRectFromIndex(pstAtlas, iIndex, &stSrc);
    if ( bOk == FALSE )
      return;

    SDL_RenderCopy(pSDL_Renderer, pstAtlas->pSDL_Txtr, &stSrc, pstDst);
  }
  void vAtlas_RenderScaled(SDL_Renderer *pSDL_Renderer,
                          PSTRUCT_IMG_ATLAS pstAtlas,
                          int iIndex,
                          SDL_Rect *pstDst,
                          double dScale,
                          int bKeepAspect) {
    SDL_Rect stSrc;
    SDL_Rect stDstScaled;
    int bOk;
    double dFinalScale;
    int iNewW;
    int iNewH;
    int iOffsetX;
    int iOffsetY;

    if ( pSDL_Renderer == NULL || pstAtlas == NULL || pstAtlas->pSDL_Txtr == NULL )
      return;

    bOk = bAtlas_SrcRectFromIndex(pstAtlas, iIndex, &stSrc);
    if ( bOk == FALSE )
      return;

    /* calcula escala final */
    if ( dScale <= 0.0 )
      dFinalScale = 1.0;
    else
      dFinalScale = dScale;

    iNewW = (int)((double)stSrc.w * dFinalScale);
    iNewH = (int)((double)stSrc.h * dFinalScale);

    /* ajusta para caber dentro de pstDst mantendo proporção */
    if ( bKeepAspect ) {
      double dScaleW;
      double dScaleH;
      dScaleW = (double)pstDst->w / (double)stSrc.w;
      dScaleH = (double)pstDst->h / (double)stSrc.h;
      dFinalScale = (dScaleW < dScaleH) ? dScaleW : dScaleH;
      iNewW = (int)((double)stSrc.w * dFinalScale);
      iNewH = (int)((double)stSrc.h * dFinalScale);
    }

    iOffsetX = pstDst->x + ((pstDst->w - iNewW) / 2);
    iOffsetY = pstDst->y + ((pstDst->h - iNewH) / 2);

    stDstScaled.x = iOffsetX;
    stDstScaled.y = iOffsetY;
    stDstScaled.w = iNewW;
    stDstScaled.h = iNewH;

    SDL_RenderCopy(pSDL_Renderer, pstAtlas->pSDL_Txtr, &stSrc, &stDstScaled);
  }

#endif
