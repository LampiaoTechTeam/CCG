/**
 * sdl_animation.c
 *
 * Written by Renato Fermi <repizza@gmail.com>
 *
 * SDL2 animation effects module
 * Implements smooth flip for card textures or rectangles (fallback).
 */

#ifdef USE_SDL2
  #include <stdio.h>
  #include <debuff.h>
  #include <deck.h>
  #include <trace.h>
  #include <monster.h>
  #include <SDL2/SDL.h>
  #include <player.h>
  #include <sdl_animation.h>
  #include <SDL2/SDL_ttf.h>
  #include <sdl_api.h>
  #include <math.h>


  STRUCT_ANIMATE_FLIP gastCardFlip[MAX_HAND];

  /*--------------------------------------------------------------*/
  static double dClamp(double dX, double dA, double dB) {
    double dR;
    dR = dX;
    if (dR < dA) dR = dA;
    if (dR > dB) dR = dB;
    return dR;
  } /* dClamp */

  /*--------------------------------------------------------------*/
  /* easing senoidal suave: 0.5 - 0.5*cos(pi*t) */
  static double dEaseInOut(double dT) {
    double dR;
    dR = 0.5 - 0.5 * cos(3.14159265358979323846 * dT);
    return dR;
  } /* dEaseInOut */

  /*--------------------------------------------------------------*/
  void vAnimateFlipInit(PSTRUCT_ANIMATE_FLIP pstFlip, SDL_Texture *pstTexFront, SDL_Texture *pstTexBack, SDL_Rect *pstDst, double dDuration) {
    pstFlip->bIsFlipping = 0;
    pstFlip->bIsFront = 1;
    pstFlip->dTime = 0.0;
    pstFlip->dDuration = (dDuration > 0.0) ? dDuration : FLIP_DURATION_S;
    pstFlip->pstTexFront = pstTexFront;
    pstFlip->pstTexBack = pstTexBack;
    pstFlip->stDst = *pstDst;
  } /* vAnimateFlipInit */

  /*--------------------------------------------------------------*/
  void vAnimateFlipStart(PSTRUCT_ANIMATE_FLIP pstFlip) {
    if (pstFlip->bIsFlipping)
      return;
    pstFlip->bIsFlipping = 1;
    pstFlip->dTime = 0.0;
  } /* vAnimateFlipStart */

  /*--------------------------------------------------------------*/
  void vAnimateFlipUpdate(PSTRUCT_ANIMATE_FLIP pstFlip, int iDeltaMs) {
    double dDt;
    if (!pstFlip->bIsFlipping)
      return;

    /* evita saltos grandes (alt-tab, lag) */
    if (iDeltaMs > 66)
      iDeltaMs = 66;

    dDt = (double)iDeltaMs / 1000.0;
    pstFlip->dTime += dDt;

    if (pstFlip->dTime >= pstFlip->dDuration) {
      pstFlip->dTime = pstFlip->dDuration;
      pstFlip->bIsFlipping = 0;
      pstFlip->bIsFront = !pstFlip->bIsFront;
    }
  } /* vAnimateFlipUpdate */

  /*--------------------------------------------------------------*/
  void vAnimateFlipRender(PSTRUCT_ANIMATE_FLIP pstFlip, SDL_Renderer *pstRenderer) {
    SDL_Texture *pstTex;
    SDL_Rect stDstDraw;
    SDL_Rect stSrc;
    double dT;
    double dNorm;
    double dScaleX;
    int iMid;
    int iW;
    int iH;
    Uint8 iAlpha;

    /*-------------------------------------------*/
    /* Fallback: sem textura → desenha retângulo */
    if (pstFlip->pstTexFront == NULL || pstFlip->pstTexBack == NULL) {
      double dTNorm;
      double dScale;
      int iMidX;
      int bFrontNow;
      Uint8 iR;
      Uint8 iG;
      Uint8 iB;

      stDstDraw = pstFlip->stDst;
      dT = pstFlip->dTime;
      dTNorm = (pstFlip->dDuration > 0.0) ? dClamp(dT / pstFlip->dDuration, 0.0, 1.0) : 1.0;
      dTNorm = dEaseInOut(dTNorm);

      if (dTNorm <= 0.5)
        dScale = 1.0 - (dTNorm * 2.0);
      else
        dScale = (dTNorm - 0.5) * 2.0;

      if (dScale < FLIP_MIN_SCALE_X)
        dScale = FLIP_MIN_SCALE_X;

      iMidX = pstFlip->stDst.x + pstFlip->stDst.w / 2;
      stDstDraw.w = (int)((double)pstFlip->stDst.w * dScale);
      stDstDraw.h = pstFlip->stDst.h;
      stDstDraw.x = iMidX - (stDstDraw.w / 2);
      stDstDraw.y = pstFlip->stDst.y;

      /* face “lógica” do frame, sem alterar estado real */
      bFrontNow = pstFlip->bIsFront;
      if (pstFlip->bIsFlipping && dTNorm > 0.5)
        bFrontNow = !bFrontNow;

      if (bFrontNow) {
        iR = 200; iG = 200; iB = 200; /* frente: cinza claro */
      } else {
        iR = 100; iG = 80;  iB = 60;  /* verso: marrom */
      }

      SDL_SetRenderDrawColor(pstRenderer, iR, iG, iB, 255);
      SDL_RenderFillRect(pstRenderer, &stDstDraw);
      SDL_SetRenderDrawColor(pstRenderer, 30, 30, 30, 255);
      SDL_RenderDrawRect(pstRenderer, &stDstDraw);

      return;
    }

    /** Renderização normal (texturas reais) */
    stDstDraw = pstFlip->stDst;
    dT = pstFlip->dTime;
    dNorm = (pstFlip->dDuration > 0.0) ? dClamp(dT / pstFlip->dDuration, 0.0, 1.0) : 1.0;
    dNorm = dEaseInOut(dNorm);

    /* escala X 1→0→1 (fecha e abre) */
    if (dNorm <= 0.5) {
      dScaleX = 1.0 - (dNorm * 2.0);
    } else {
      dScaleX = (dNorm - 0.5) * 2.0;
    }

    if (dScaleX < FLIP_MIN_SCALE_X)
      dScaleX = FLIP_MIN_SCALE_X;

    /* troca da face no meio */
    pstTex = pstFlip->bIsFront ? pstFlip->pstTexFront : pstFlip->pstTexBack;
    if (pstFlip->bIsFlipping && dNorm > 0.5)
      pstTex = pstFlip->bIsFront ? pstFlip->pstTexBack : pstFlip->pstTexFront;

    SDL_QueryTexture(pstTex, NULL, NULL, &iW, &iH);
    stSrc.x = 0;
    stSrc.y = 0;
    stSrc.w = iW;
    stSrc.h = iH;

    /* centraliza */
    iMid = pstFlip->stDst.x + pstFlip->stDst.w / 2;
    stDstDraw.w = (int)((double)pstFlip->stDst.w * dScaleX);
    stDstDraw.h = pstFlip->stDst.h;
    stDstDraw.x = iMid - (stDstDraw.w / 2);
    stDstDraw.y = pstFlip->stDst.y;

    /* sombreamento proporcional ao “fechamento” */
    if (pstFlip->bIsFlipping) {
      iAlpha = (Uint8)(30 + 150 * (1.0 - dScaleX));
      SDL_SetRenderDrawBlendMode(pstRenderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(pstRenderer, 0, 0, 0, iAlpha);
      SDL_RenderFillRect(pstRenderer, &pstFlip->stDst);
    }

    SDL_RenderCopy(pstRenderer, pstTex, &stSrc, &stDstDraw);
  } /* vAnimateFlipRender */

  /*--------------------------------------------------------------*/
  /* Anima o flip das cartas recém-compradas (mão do jogador)     */
  /* Bloqueia a interação até a animação terminar                 */
  void vAnimateFlipHand(SDL_Renderer *pstRenderer,
                        STRUCT_DECK *pstDeck
                     ) {
    int ii;
    int iAllDone;
    int iDeltaMs;
    Uint32 iLastTicks;
    Uint32 iNowTicks;

    /* Inicializa flips: começam viradas para baixo (usa duração padrão) */
    for (ii = 0; ii < pstDeck->iHandCount && ii < MAX_HAND; ii++) {
      vAnimateFlipInit(&gastCardFlip[ii], NULL, NULL, &gCardRects[ii], FLIP_DURATION_S);
      gastCardFlip[ii].bIsFront = 0;
      vAnimateFlipStart(&gastCardFlip[ii]);
    }

    iLastTicks = SDL_GetTicks();
    iAllDone = 0;

    /* Loop de animação bloqueante */
    while (!iAllDone) {
      iNowTicks = SDL_GetTicks();
      iDeltaMs = (int)(iNowTicks - iLastTicks);
      iLastTicks = iNowTicks;

      iAllDone = 1;
      for (ii = 0; ii < pstDeck->iHandCount && ii < MAX_HAND; ii++) {
        vAnimateFlipUpdate(&gastCardFlip[ii], iDeltaMs);
        if (gastCardFlip[ii].bIsFlipping)
          iAllDone = 0;
      }

      /* Cartas com flip – sincroniza posição/size com a mesa a cada frame */
      for (ii = 0; ii < pstDeck->iHandCount && ii < MAX_HAND; ii++) {
        gastCardFlip[ii].stDst = gCardRects[ii];
        vAnimateFlipRender(&gastCardFlip[ii], pstRenderer);
      }

      /* Textos visíveis apenas se a carta já está virada (sem amassar) */
      for (ii = 0; ii < pstDeck->iHandCount && ii < MAX_HAND; ii++) {
        if (!gastCardFlip[ii].bIsFlipping && gastCardFlip[ii].bIsFront) {
          char szLine1[64];
          char szLine2[64];
          SDL_Color stBlack;
          int iTx;
          int iTy;

          stBlack = (SDL_Color){0, 0, 0, 255};
          iTx = gCardRects[ii].x + 8;
          iTy = gCardRects[ii].y + 8;

          snprintf(szLine1, sizeof(szLine1), "%s", pstDeck->astHand[ii].szName);
          snprintf(szLine2, sizeof(szLine2), "E:%d V:%d",
                   pstDeck->astHand[ii].iCost,
                   pstDeck->astHand[ii].iValue);

          vSDL_DrawText(pstRenderer, szLine1, iTx, iTy, stBlack);
          vSDL_DrawText(pstRenderer, szLine2, iTx, iTy + 21, stBlack);
        }
      }
    }
  } /* vAnimateFlipHand */

#endif /* USE_SDL2 */
