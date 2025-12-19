#ifdef USE_SDL2
  #include <trace.h>
  #include <image.h>
  #include <sys_interface.h>
  #include <string.h>
  #include <stdlib.h>

  STRUCT_IMAGE_CONF gstImgConf;
  STRUCT_IMAGE gstImages[MAX_IMAGES];
  int giImageCount = 0;

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
    int iBaseLen;
    int iFullLen;

    if ( (pstImgConf = pstCreateImgNode()) == NULL )
      return FALSE;

    if ( bStrIsEmpty(pszPath) )
      return FALSE;

    iBaseLen = (int)strlen(ASSETS_BASE_DIR);
    iFullLen = iBaseLen + 1 + iLen + 1; /* base + '/' + nome + '\0' */

    pstImgConf->iType = iImgType;
    pstImgConf->pszPath = (char *)malloc(iFullLen);
    if ( pstImgConf->pszPath == NULL )
      return FALSE;

    memset(pstImgConf->pszPath, 0, iFullLen);
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

  int bLoadImgListFromFile() {
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
      if ( (pTok = strtok(NULL, "|\n")) == NULL )   continue;

      if ( bAddImg2Mem(iType, pTok, strlen(pTok)) == FALSE ) continue;
      
      bLoaded = TRUE;
    }

    if ( bLoaded )
      vTraceImgList();

    bCloseFile(&pfImg);
    vTraceVarArgsFn("end");

    return bLoaded;
  }

  void vTraceImgList() {
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

  SDL_Texture *pSDL_LoadTextureFromPath(SDL_Renderer *pSDL_Renderer, char *pszPath){
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

  int iIMG_LoadAll(SDL_Renderer *pSDL_Renderer){
    PSTRUCT_IMAGE_CONF pstImageCnf;
    STRUCT_IMAGE *pstImg;
    int bAnyLoaded;

    bAnyLoaded = FALSE;
    giImageCount = 0;

    pstImageCnf = &gstImgConf;
    while ( pstImageCnf != NULL ) {

      if ( !bStrIsEmpty(pstImageCnf->pszPath) ) {

        if ( giImageCount >= MAX_IMAGES )
          break;

        pstImg = &gstImages[giImageCount];
        memset(pstImg, 0, sizeof(*pstImg));

        pstImg->iType = pstImageCnf->iType;
        strncpy(pstImg->szPath, pstImageCnf->pszPath, sizeof(pstImg->szPath)-1);

        pstImg->pSDL_Txtr = pSDL_LoadTextureFromPath(pSDL_Renderer, pstImageCnf->pszPath);
        if ( pstImg->pSDL_Txtr != NULL ) {

          pstImg->pSDL_IMGRect.x = 0;
          pstImg->pSDL_IMGRect.y = 0;
          pstImg->pSDL_IMGRect.w = 280;
          pstImg->pSDL_IMGRect.h = 300;

          giImageCount++;
          bAnyLoaded = TRUE;
        }
      }

      pstImageCnf = pstImageCnf->pstNext;
    }

    vTraceVarArgsFn("iIMG_LoadAll: giImageCount=%d", giImageCount);
    return bAnyLoaded;
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
