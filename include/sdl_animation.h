/**
 * sdl_animation.h
 *
 * Written by Renato Fermi <repizza@gmail.com>
 * 
 * Animation and transition effects for SDL2 rendering.
 * Example: card flip 3D illusion.
 */

#ifdef USE_SDL2
  #ifndef _SDL_ANIMATION_H_
    #define _SDL_ANIMATION_H_

    #include <card_game.h>

    typedef struct STRUCT_ANIMATE_FLIP {
      int bIsFlipping;             /* 1 = animando, 0 = parado */
      int bIsFront;                /* 1 = frente visível, 0 = verso */
      double dTime;                /* tempo acumulado (s) */
      double dDuration;            /* duração total (s), ex.: 0.6 */
      SDL_Texture *pstTexFront;    /* textura da frente */
      SDL_Texture *pstTexBack;     /* textura do verso */
      SDL_Rect stDst;              /* posição base da carta */
    } STRUCT_ANIMATE_FLIP, *PSTRUCT_ANIMATE_FLIP;

    extern STRUCT_ANIMATE_FLIP gastCardFlip[MAX_HAND];
    extern SDL_Rect gCardRects[64];

    /* -------------------------------------------------------------- */
    /* Funções principais de animação */
    void vAnimateFlipInit(PSTRUCT_ANIMATE_FLIP pstFlip,
                          SDL_Texture *pstTexFront,
                          SDL_Texture *pstTexBack,
                          SDL_Rect *pstDst,
                          double dDuration);

    void vAnimateFlipStart(PSTRUCT_ANIMATE_FLIP pstFlip);

    void vAnimateFlipUpdate(PSTRUCT_ANIMATE_FLIP pstFlip, int iDeltaMs);

    void vAnimateFlipRender(PSTRUCT_ANIMATE_FLIP pstFlip, SDL_Renderer *pstRenderer);

    /* -------------------------------------------------------------- */
    /* Função composta para flip da mão do jogador */
    void vAnimateFlipHand(SDL_Renderer *pstRenderer,
                          STRUCT_DECK *pstDeck,
                          STRUCT_MONSTER *pastMonsters,
                          int iMonsterCt,
                          STRUCT_PLAYER *pstPlayer);
    /* Velocidade global do flip (segundos) */
    #define FLIP_DURATION_S 0.8

    /* Largura mínima do squeeze para evitar aliás/linha de 1px sumindo */
    #define FLIP_MIN_SCALE_X 0.04
  #endif /* _SDL_ANIMATION_H_ */
#endif /* USE_SDL2 */
