#ifndef CARD_GAME_H
#define CARD_GAME_H

#define COPYRIGTH "Renato Fermi (C) 2025"
#define CCG_VERSION "1.0"

#define LAYOUT_YOU_WIN   0
#define LAYOUT_GAME_OVER 1

#ifndef SDL_MAIN_HANDLED
  #define SDL_MAIN_HANDLED
#endif

#ifndef USE_SDL2
  #define CCG_Main main
#endif

#include <sys_interface.h>

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

  /**
   * @enum ENUM_GAME_STATUS
   * @brief Estados do jogo
   */
  typedef enum ENUM_GAME_STATUS {
    STATUS_NONE = -1,
    STATUS_WELCOME,
    STATUS_NEW_GAME,
    STATUS_GAMING,
    STATUS_SHOP,
    STATUS_PAUSE,
  } ENUM_GAME_STATUS, *PENUM_GAME_STATUS;

  /**
   * @enum ENUM_GAME_STATES
   * @brief Subestados do jogo
   */
  typedef enum ENUM_GAME_STATES {
    STATE_NONE = -1,
    STATE_WELCOME_BEGIN,
    STATE_WELCOME_REGISTRATION_START,
    STATE_WELCOME_REGISTRATION_DONE,
    STATE_WELCOME_CONFIG,
    STATE_WELCOME_LOAD,
    STATE_WELCOME_END,

    STATE_NEW_GAME_QUESTIONS_DONE,
    STATE_NEW_GAME_TUTORIAL_SKIPPED,
    STATE_NEW_GAME_SETUP_DONE,

    STATE_GAMING_DRAWING,
    STATE_GAMING_PLAYER_TURN,
    STATE_GAMING_ENEMY_TURN,
    STATE_GAMING_TURN_ENDED,
    STATE_GAMING_LEVEL_WON,
    STATE_GAMING_DEFEAT,
    STATE_GAMING_WROTE_LEVEL,

    STATE_SHOP_OPEN,
    STATE_SHOP_LOAD_ITENS,
    STATE_SHOP_WROTE,
    STATE_SHOP_BAG,
    STATE_SHOP_CLOSED,

    STATE_PAUSE_GAMING,
    STATE_PAUSE_MENU,
    STATE_PAUSE_BAG
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
    char szConfDir[_MAX_PATH];  /**< Caminho para o diretorio de configuracao                   */
  } STRUCT_GLOBAL_PRM, *PSTRUCT_GLOBAL_PRM;

  extern STRUCT_GLOBAL_PRM gstGlobalPrm;

#endif
