#ifndef _EVENT_H_
  #define _EVENT_H_

  #ifdef USE_SDL2

    /* Hub do projeto sempre primeiro */
    #include <card_game.h>
    extern int giPendingCard;
    /* API pública do módulo de eventos */
    void vEVENT_Init(void);
    void vEVENT_Quit(void);

    /* Handler único: processa 1 evento SDL e retorna REDRAW_NONE ou REDRAW_IMAGE */
    int iEVENT_HandlePollEv(SDL_Event *pSDL_EVENT_Ev,
                            int iRedrawCurrentAction,
                            SDL_Renderer *pSDL_Renderer,
                            PSTRUCT_DECK pstDeck,
                            PSTRUCT_MONSTER pastMonsters,
                            int iMonsterCt,
                            int *pbRunning);


  #endif /* USE_SDL2 */

#endif /* _EVENT_H_ */
