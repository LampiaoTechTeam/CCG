#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <debuff.h>
#include <trace.h>
#include <terminal_utils.h>
#include <deck.h>
#include <monster.h>
#ifdef USE_SDL2
  #include <SDL2/SDL.h>
#endif
#include <player.h>
#include <game.h>

#ifdef LINUX
  #include <unistd.h>
#endif
#include <consts.h>
#include <card_game.h>

char* gkpaszGameStatus[] = {
  "STATUS_WELCOME", "STATUS_NEW_GAME",
  "STATUS_GAMING" , "STATUS_SHOP",
  "STATUS_PAUSE"  , "STATUS_PAUSE",
  NULL
};

char* gkpaszGameStates[] = {
  "STATE_NONE",
  "STATE_WELCOME_BEGIN",
  "STATE_WELCOME_REGISTRATION_START",
  "STATE_WELCOME_REGISTRATION_DONE",
  "STATE_WELCOME_CONFIG",
  "STATE_WELCOME_LOAD",
  "STATE_WELCOME_END",

  "STATE_NEW_GAME_QUESTIONS_DONE",
  "STATE_NEW_GAME_TUTORIAL_SKIPPED",
  "STATE_NEW_GAME_SETUP_DONE",

  "STATE_GAMING_DRAWING",
  "STATE_GAMING_PLAYER_TURN",
  "STATE_GAMING_ENEMY_TURN",
  "STATE_GAMING_TURN_ENDED",
  "STATE_GAMING_LEVEL_WON",
  "STATE_GAMING_DEFEAT",
  "STATE_GAMING_WROTE_LEVEL",

  "STATE_SHOP_OPEN",
  "STATE_SHOP_LOAD_ITENS",
  "STATE_SHOP_WROTE",
  "STATE_SHOP_BAG",
  "STATE_SHOP_CLOSED",

  "STATE_PAUSE_GAMING",
  "STATE_PAUSE_MENU",
  "STATE_PAUSE_BAG",

  NULL
};

int gbLoadGameFromFile = FALSE;

void vTraceGame(void) {
  vTraceVarArgsFn("gstGame.iStatus: [%s]", gkpaszGameStatus[gstGame.iStatus]);
  vTraceVarArgsFn("gstGame.iLastStatus: [%s]", gkpaszGameStatus[gstGame.iLastStatus]);
  vTraceVarArgsFn("gstGame.iState.: [%s]", gkpaszGameStates[gstGame.iState]);
  vTraceVarArgsFn("gstGame.iLastState.: [%s]", gkpaszGameStates[gstGame.iLastState]);
  vTraceMonsters(gstGame.stGameContext.astMonster, gstGame.stGameContext.iCtMonster);
  vTracePlayer(&gstGame.stGameContext.stPlayer);
}

void vGameLoadCtxVars(PSTRUCT_GAME_CONTEXT pstGameCtx, PSTRUCT_DECK pstDeck, PSTRUCT_PLAYER pstPlayer, PSTRUCT_MONSTER pastMonster, int *piMonsterCt){
   vTraceVarArgsFn("memcpy(&stDeck, gstGame.stGameContext.stPlayer.astPlayerCards, sizeof(STRUCT_DECK));");
    memcpy(pstDeck, pstGameCtx->stPlayer.astPlayerCards, sizeof(STRUCT_DECK));
    vTraceVarArgsFn("memcpy(&gstPlayer, &gstGame.stGameContext.stPlayer, sizeof(STRUCT_PLAYER));");
    memcpy(pstPlayer, &pstGameCtx->stPlayer, sizeof(STRUCT_PLAYER));
    vTraceVarArgsFn("memcpy(&astMonsters, &gstGame.stGameContext.astMonster, sizeof(STRUCT_MONSTER) * gstGame.stGameContext.iCtMonster);");
    memcpy(pastMonster, pstGameCtx->astMonster, sizeof(STRUCT_MONSTER) * pstGameCtx->iCtMonster);
    vTraceVarArgsFn("iMonsterCount = gstGame.stGameContext.iCtMonster;");
    *piMonsterCt = pstGameCtx->iCtMonster;
    vTraceVarArgsFn("FINISH");
}

int iGameSave(void) {
  FILE* fpGameDat = NULL;
  char szGameDatPath[512] = "";
  memset(szGameDatPath, 0x00, sizeof(szGameDatPath));
  snprintf(szGameDatPath, sizeof(szGameDatPath), "%s%cGAME.dat", gstGlobalPrm.szWrkDir, DIR_SEPARATOR);
  if ( (fpGameDat = fopen(szGameDatPath, "wb")) == NULL ) {
    return 0;
  }
  fwrite(&gstGame, sizeof(gstGame), 1, fpGameDat);
  fflush(fpGameDat);
  #ifdef LINUX
  fsync(fileno(fpGameDat));
  #endif
  fclose(fpGameDat);
  fpGameDat = NULL;
  return 1;
}

int iGameLoad(void) {
  FILE* fpGameDat = NULL;
  char szGameDatPath[512] = "";

  memset(szGameDatPath, 0x00, sizeof(szGameDatPath));

  if ( DEBUG_ALL ) vTraceBegin();

  snprintf(szGameDatPath, sizeof(szGameDatPath), "%s%cGAME.dat", gstGlobalPrm.szWrkDir, DIR_SEPARATOR);
  if ( (fpGameDat = fopen(szGameDatPath, "rb")) == NULL ) {
    return 0;
  }
  if (fread(&gstGame, sizeof(gstGame), 1, fpGameDat) != 1) {
    fclose(fpGameDat);
    return 0;
  }
  fclose(fpGameDat);
  fpGameDat = NULL;
  if ( DEBUG_ALL ) {
    vTraceVarArgsFn("Jogo carregado:");
    vTraceGame();
  }

  if ( DEBUG_ALL ) vTraceEnd();

  return 1;
}

int iGameDelete(void) {
  char szGameDatPath[512] = "";
  memset(szGameDatPath, 0x00, sizeof(szGameDatPath));
  snprintf(szGameDatPath, sizeof(szGameDatPath), "%s%cGAME.dat", gstGlobalPrm.szWrkDir, DIR_SEPARATOR);
  remove(szGameDatPath);
  return 1;
}
