#ifndef _EVENT_H_
  #define _EVENT_H_

  #ifdef USE_SDL2

    #include <card_game.h>

    extern int giPendingCard;
    extern int giHoverCard;
    extern int giHoverMonster;

    void vEVENT_Init(void);
    void vEVENT_Quit(void);

    int iEVENT_HandlePollEv(SDL_Event *pSDL_EVENT_Ev,
                            int iRedrawCurrentAction,
                            SDL_Renderer *pSDL_Renderer,
                            PSTRUCT_DECK pstDeck,
                            PSTRUCT_MONSTER pastMonsters,
                            int iMonsterCt,
                            int *pbRunning);

  #endif /* USE_SDL2 */

#endif /* _EVENT_H_ */



