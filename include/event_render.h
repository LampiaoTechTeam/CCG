#ifndef _EVENT_RENDER_H_
  #define _EVENT_RENDER_H_

  #ifdef USE_SDL2
    // #include <card_game.h>

    void vEVR_Init(void);
    void vEVR_Quit(void);

    void vEVR_PushMonsterDamage(int iMonsterIdx, int iDamage);
    void vEVR_PushMonsterHeal(int iMonsterIdx, int iHeal);
    void vEVR_PushMonsterShield(int iMonsterIdx, int iShield);

    void vEVR_PushPlayerHeal(int iHeal);
    void vEVR_PushPlayerShield(int iShield);

    int iEVR_Tick(uint64_t ui64NowMs);
    void vEVR_Render(SDL_Renderer *pSDL_Renderer);
    void vEVR_ClearAll(void);

  #endif /* USE_SDL2 */

#endif /* _EVENT_RENDER_H_ */
