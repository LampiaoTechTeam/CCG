/**
 * card_game.h
 *
 * Written by Renato Fermi <repiazza@gmail.com>
 *
 * Description: Main header of the project
 */

#ifndef CARD_GAME_H
#define CARD_GAME_H

#define COPYRIGTH "Renato Fermi (C) 2025"
#define CCG_VERSION "1.0"

#ifndef SDL_MAIN_HANDLED
  #define SDL_MAIN_HANDLED
#endif

#ifndef USE_SDL2
  #define CCG_Main main
#endif

#ifdef FAKE
  #include <fdummies.h>
#endif

  extern char *gkpszProgramName;
  extern int giLevel;
  extern int gbSDL_Mode;

  #define INT_WINDOW_WIDTH  800
  #define INT_WINDOW_HEIGHT 600
  #define WINDOW_RATIO  INT_WINDOW_WIDTH/INT_WINDOW_HEIGHT
  #define COL_RATIO 0.04
  #define VSYNC_TIME 16.666666666

  #define MSG_GAME_SAVE_WITH_SUCCESS    1
  #define MSG_PRESS_ANY_KEY_TO_CONTINUE 2
  #define MSG_EXIT_GAME                 3
  #define MSG_YOU_LOSE                  4
  #define MSG_NOT_FOUND_GAME_SAVE       5

  /**
   * @struct STRUCT_GLOBAL_PRM
   * @brief Parametros globais
   */
  typedef struct STRUCT_GLOBAL_PRM {
    char szWrkDir[_MAX_PATH];   /**< Diretorio de arquivos temporarios e recuperacao de estados */
    char szTrace[_MAX_PATH];    /**< Caminho do arquivo de log                                  */
    char szDebugLevel[32];      /**< Nivel de puracao                                           */
    char szFontsDir[_MAX_PATH]; /**< Caminho para o diretorio das fontes                        */
    char szConfDir[_MAX_PATH];  /**< Caminho para o diretorio de configuracao                   */
    char szTraceOnTerminal[2];
  } STRUCT_GLOBAL_PRM, *PSTRUCT_GLOBAL_PRM;

  extern STRUCT_GLOBAL_PRM gstGlobalPrm;

#endif
