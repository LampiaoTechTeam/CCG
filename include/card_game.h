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
  extern char gszBaseDir[_MAX_PATH];
  
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

  #define ACTION_START_NEW_GAME 1
  #define ACTION_LOAD_GAME      2
  #define ACTION_SETTINGS       3
  #define ACTION_EXIT           4
  #define ACTION_SAVE           5
  #define ACTION_SAVE_AND_EXIT  6
  #define ACTION_INVENTARY      7
  #define ACTION_BACK           8


  /**
   * @struct STRUCT_GLOBAL_PRM
   * @brief Parametros globais
   */
  typedef struct STRUCT_GLOBAL_PRM {
    char szWrkDir[_MAX_PATH];    /** Diretorio de arquivos temporarios e recuperacao de estados */
    char szTrace[_MAX_PATH];     /** Caminho do arquivo de log                                  */
    char szDebugLevel[32];       /** Nivel de puracao                                           */
    char szBaseDir[_MAX_PATH];   /** Caminho para a raiz do projeto                             */
    char szFontsDir[_MAX_PATH];  /** Caminho para o diretorio das fontes                        */
    char szConfDir[_MAX_PATH+64];   /** Caminho para o diretorio de configuracao                   */
    char szTraceOnTerminal[2];
    char szAssetsDir[_MAX_PATH]; /** Caminho para o diretorio de assets                         */
  } STRUCT_GLOBAL_PRM, *PSTRUCT_GLOBAL_PRM;

  extern STRUCT_GLOBAL_PRM gstGlobalPrm;
  
  /**
  * @brief Sets root project path depending on env setup
  *
  * @return  2 : Set from Cmdline
  * @return  1 : Set from XML
  * @return  0 : Set from env var CCG_ROOT_DIR
  * @return -1 : Could not set
  *
  */
  int iSetBaseDir(void);
    
#endif
