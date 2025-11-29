#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#define COPYRIGTH "Renato Fermi (C) 2025"
#define CCG_VERSION "1.0"

#define LAYOUT_YOU_WIN   0
#define LAYOUT_GAME_OVER 1

#ifndef SDL_MAIN_HANDLED
  #define SDL_MAIN_HANDLED
#endif

#ifdef USE_SDL2
  #include <SDL2/SDL.h>
#else
  #define CCG_Main main
#endif

#include <sys_interface.h>

#ifdef FAKE
  #include <fdummies.h>
#endif

#ifndef CARD_GAME_H
  #define CARD_GAME_H

  extern char *gkpszProgramName;
  extern int giLevel;
  extern int gbSDL_Mode;

  #define INT_WINDOW_WIDTH  800
  #define INT_WINDOW_HEIGHT 600
  #define WINDOW_RATIO  INT_WINDOW_WIDTH/INT_WINDOW_HEIGHT
  #define COL_RATIO 0.04
  #define VSYNC_TIME 16.666666666 
  
  #define STATUS_RUN   0
  #define STATUS_PAUSE 1

  #define STATE_YOU_WIN  0
  #define STATE_YOU_LOSE 1

  typedef struct STRUCT_GAME {
    int iStatus;
    int iState;
    int iLevel;
  } STRUCT_GAME;

  extern STRUCT_GAME gstGame;

#endif

