#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <conf.h>

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

#ifdef _WIN32
  #define DIR_SEPARATOR '\\'
#else
  #define DIR_SEPARATOR '/'
#endif

  #define INT_WINDOW_WIDTH  800
  #define INT_WINDOW_HEIGHT 600
  #define WINDOW_RATIO  INT_WINDOW_WIDTH/INT_WINDOW_HEIGHT
  #define COL_RATIO 0.04
  #define VSYNC_TIME 16.666666666 

  /**
   * @enum ENUM_GAME_STATUS
   * @brief Estados do jogo
   */
  typedef enum ENUM_GAME_STATUS {
    STATUS_RUN,
    STATUS_PAUSE,
    STATUS_IN_GAME,
    STATUS_START_MENU
  } ENUM_GAME_STATUS, *PENUM_GAME_STATUS;

  /**
   * @enum ENUM_GAME_STATES
   * @brief Subestados do jogo
   */
  typedef enum ENUM_GAME_STATES {
    STATE_YOU_WIN,
    STATE_YOU_LOSE,
    STATE_IN_GAME_TABLE,
    STATE_IN_GAME_ENEMY_TURN,
    STATE_IN_GAME_PLAYER_TURN,
    STATE_IN_GAME_LEVEL_COMPLETE,
    STATE_IN_GAME_SHOP,
    STATE_IN_GAME_LEVEL_DONE
  } ENUM_GAME_STATES, *PENUM_GAME_STATES;

  /**
   * @struct STRUCT_GLOBAL_PRM
   * @brief Parametros globais
   */
  typedef struct STRUCT_GLOBAL_PRM {
    char szWrkDir[_MAX_PATH];   /**< Diretorio de arquivos temporarios e recuperacao de estados */
    char szTrace[256];          /**< Caminho do arquivo de log                                  */
    char szDebugLevel[32];      /**< Nivel de puracao                                           */
    char szFontsDir[_MAX_PATH]; /**< Caminho para o diretorio das fontes                        */
  } STRUCT_GLOBAL_PRM, *PSTRUCT_GLOBAL_PRM;

  extern STRUCT_GLOBAL_PRM gstGlobalPrm;

#endif

