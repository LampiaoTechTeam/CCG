#include <stdio.h>
#include <sys_interface.h>
#include <trace.h>
#include <string.h>
#include <stdlib.h>
#include <terminal_utils.h>
#include <input.h>
#include <deck.h>
#include <monster.h>
#include <player.h>
#include <battle.h>
#include <dialog.h>
#include <shop.h>

char *gkpszProgramName;
int gbLogLevel = 1;
int giLevel;

STRUCT_PLAYER gstPlayer;

void vFreeProgramName(){ 
  if (gkpszProgramName != NULL)
    free(gkpszProgramName);
}

int main(int argc, char *argv[]) {
  
  STRUCT_MONSTER astMonsters[MAX_MONSTERS];
  STRUCT_DECK stDeck;
  char szPath[_MAX_PATH];
  char szName[_MAX_PATH];
  char szExt[_MAX_PATH];
  int bRunning;
  int iMonsterCount;

  bRunning = TRUE;
  giLevel = 1;

  gkpszProgramName = NULL;
  
  iDIR_SplitFilename(argv[0], szPath, szName, szExt);
  if (!bStrIsEmpty(szName)) {
    gkpszProgramName = (char *)malloc(strlen(szName)+1);
    memset(gkpszProgramName, 0, strlen(szName)+1);
    strcpy(gkpszProgramName, szName);
  }
  
  vInitLogs();
  vShowInitDialog();
  if ( argc > 1 ) vTraceVarArgsFn("excess of cmdline prms argc=%d", argc);

  vTraceVarArgsFn("Init OK ==== ***  main LOOP *** =====");
  vTraceVarArgsFn("             ***            *** =====");

  vInitBasicDeck(&stDeck);
  iDrawMultipleCard(INIT_HAND_CARDS, &stDeck);
  vInitPlayer(&stDeck);
  vInitMonstersForLevel(astMonsters, giLevel, &iMonsterCount);
  vInitDialog();

  #ifdef FAKE

    PSTRUCT_DECK pstDeck = &stDeck;
    PSTRUCT_CARD pstCard = pstDeck->astHand;

    gstPlayer.iGold += 100;
    vOpenShop(pstDeck);
    vDiscardHand(pstDeck);
    iDrawMultipleCard(INIT_HAND_CARDS, pstDeck);
    vShuffle(pstCard, stDeck.iHandCount);

  #endif
  
  while (bRunning) {
    vClearTerminal();

    vSortHandByName(&stDeck);
    vShowTable(&stDeck, astMonsters, iMonsterCount);
    vTraceDialog(FALSE);

    while (gstPlayer.iEnergy > 0 && iAnyMonsterAlive(astMonsters, iMonsterCount)) {
      if ( iDoPlayerTurn(&bRunning, &stDeck, astMonsters, iMonsterCount) )
        break;
    }

    /* fim de turno do jogador */
    vDiscardHand(&stDeck);
    vDoEnemyActions(astMonsters, iMonsterCount);

    /* checa derrota */
    if (gstPlayer.iHP <= 0) {
      vPrintLine("\n***  Derrota!! Você morreu! ***", INSERT_NEW_LINE);
      vSleepSeconds(3);
      break;
    }

    /* checa vitória do nível */
    if (!iAnyMonsterAlive(astMonsters, iMonsterCount)) {
      char szMsg[128];
      snprintf(szMsg, sizeof(szMsg), "\n*** Nivel %d completo! ***", giLevel);
      vPrintHighlitedLine(szMsg, INSERT_NEW_LINE);
      
      vFlushInput();
      vAddPlayerReward(&gstPlayer);
      vOpenShop(&stDeck);

      vFlushInput();
      giLevel++;
      vInitMonstersForLevel(astMonsters, giLevel, &iMonsterCount);

      /* reset de mão/energia para novo nível */
      iDrawMultipleCard(INIT_HAND_CARDS, &stDeck);
      gstPlayer.iEnergy = PLAYER_ENERGY_MAX;
      gstPlayer.iBlock = 0;
      continue;
    }

    /* próximo turno no mesmo nível */
    iDrawMultipleCard(INIT_HAND_CARDS, &stDeck);
    gstPlayer.iEnergy = PLAYER_ENERGY_MAX;
    
    vTraceDeck(&stDeck, TRACE_DECK_ALL);
    iMonsterCount = iAliveMonsterQty(astMonsters, iMonsterCount);
  }
  
  vTraceVarArgsFn("             ***                   *** =====");
  vTraceVarArgsFn("             ***   End Main LOOP   *** =====");
  vFreeDialog();
  vFreeProgramName();
  return 0;
}