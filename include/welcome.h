/**
 * welcome.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *
 * Description: Initial screen of the game
 */

#ifndef WELCOME_H
  #define WELCOME_H

  #define WELCOME_MAX_ITEMS 24
  #define WELCOME_COLS 4
  #define WELCOME_ROWS 3

  extern int gbWelcomeOpen;

  int bLoadWelcome(void);
  #ifdef USE_SDL2
    void vSDL_WelcomeInit();
    int iWelcomeDraw(SDL_Renderer *pSDL_Renderer);
    int iSDL_OpenWelcome(SDL_Renderer *pSDL_Renderer);
  #endif
#endif /* _WELCOME_H_ */

