#ifndef _GAME_H_
#define _GAME_H_

// #include <card_game.h>

/**
 * @brief ...
 *
 */
typedef struct STRUCT_GAME_CONTEXT {
  STRUCT_PLAYER stPlayer;
  STRUCT_MONSTER astMonster[MAX_MONSTERS];
} STRUCT_GAME_CONTEXT, *PSTRUCT_GAME_CONTEXT;

/**
 * @brief ...
 *
 */
typedef struct STRUCT_GAME {
  int iStatus;
  int iState;
  int iLastState;
  int iLevel;
  STRUCT_GAME_CONTEXT stGameContext;
  int iRedrawAction;
} STRUCT_GAME;

extern STRUCT_GAME gstGame;

/**
 * @brief ...
 *
 * @return int
 */
int iGameSave(void);

/**
 * @brief ...
 *
 * @return int
 */
int iGameLoad(void);

#endif

