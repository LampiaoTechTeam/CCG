/**
 * game.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in Nov 2025
 *
 * Description: Game state machine
 */

#ifndef _GAME_H_
#define _GAME_H_

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
 * @struct STRUCT_GAME_CONTEXT
 * @brief Game context
 */
typedef struct STRUCT_GAME_CONTEXT {
  STRUCT_PLAYER stPlayer;                  /**< Player                            */
  STRUCT_MONSTER astMonster[MAX_MONSTERS]; /**< Monsters                          */
  int iCtMonster;                          /**< Quantitiy of mosnter in the level */
} STRUCT_GAME_CONTEXT, *PSTRUCT_GAME_CONTEXT;

/**
 * @struct STRUCT_GAME
 * @brief That represents a game state machine
 */
typedef struct STRUCT_GAME {
  int iStatus;
  int iState;
  int iLastStatus;
  int iLastState;
  int iLevel;
  STRUCT_GAME_CONTEXT stGameContext;
  int iRedrawAction;
} STRUCT_GAME;

extern STRUCT_GAME gstGame;

extern int gbLoadGameFromFile;

extern char* gkpaszGameStatus[];
extern char* gkpaszGameStates[];

/**
 * @brief Create GAME.dat
 *
 * @return 1 success
 * @return 0 error
 */
int iGameSave(void);

/**
 * @brief Load gstGame from GAME.dat
 *
 * @return 1 success
 * @return 0 error
 */
int iGameLoad(void);

/**
 * @brief Delete GAME.dat
 *
 * @return 1 success
 * @return 0 error
 */
int iGameDelete(void);

/// @brief Load context from gstGame
void vGameLoadCtxVars(PSTRUCT_GAME_CONTEXT pstGameCtx, PSTRUCT_DECK pstDeck, PSTRUCT_PLAYER pstPlayer, PSTRUCT_MONSTER pastMonster, int *piMonsterCt);

#endif

