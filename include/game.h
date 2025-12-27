#ifndef _GAME_H_
#define _GAME_H_

/**
 * @brief ...
 *
 */
typedef struct STRUCT_GAME_CONTEXT {
  STRUCT_PLAYER stPlayer;
  STRUCT_MONSTER astMonster[MAX_MONSTERS];
  int iCtMonster;
} STRUCT_GAME_CONTEXT, *PSTRUCT_GAME_CONTEXT;

/**
 * @brief ...
 *
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

