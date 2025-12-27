#include <stdio.h>
#include <sys_interface.h>
#include <card_game.h>
#include <debuff.h>
#include <deck.h>
#include <trace.h>
#include <terminal_utils.h>
#include <monster.h>
#ifdef USE_SDL2
  #include <SDL2/SDL_rect.h>
#endif
#include <player.h>
#ifdef USE_SDL2
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_ttf.h>
  #include <sdl_api.h>
#endif
#include <battle.h>
#include <input.h>
#include <shop.h>
#include <dialog.h>
#include <console_api.h>

void vCNSL_MainLoop(int *pbRunning, PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCt){
  while (*pbRunning) {
    vClearTerminal();
    vSortHandByName(pstDeck);
    vShowTable(pstDeck, pastMonsters, iMonsterCt);
    vTraceDialog(FALSE);

    while (gstPlayer.iEnergy > 0 && iAnyMonsterAlive(pastMonsters, iMonsterCt)) {
      if (iDoPlayerTurn(pbRunning, pstDeck, pastMonsters, iMonsterCt))
        break;
    }

    /** Players turn end */
    vDiscardHand(pstDeck);
    vDoEnemyActions(pastMonsters, iMonsterCt);

    /** Tests for player defeat */
    if (gstPlayer.iHP <= 0) {
      vPrintLine("\n***  Derrota!! Você morreu! ***", INSERT_NEW_LINE);
      vSleepSeconds(3);
      break;
    }

    /** Tests for level clear */
    if (!iAnyMonsterAlive(pastMonsters, iMonsterCt)) {
      char szMsg[128];
      snprintf(szMsg, sizeof(szMsg), "\n*** Nivel %d completo! ***", giLevel);
      vPrintHighlitedLine(szMsg, INSERT_NEW_LINE);

      vFlushInput();
      vAddPlayerReward(&gstPlayer);
      vOpenShop(pstDeck);

      vFlushInput();
      giLevel++;
      vInitMonstersForLevel(pastMonsters, giLevel, &iMonsterCt);

      /** New level initialization */
      iDrawMultipleCard(INIT_HAND_CARDS, pstDeck);
      vSleepSeconds(3);
      gstPlayer.iEnergy = PLAYER_ENERGY_MAX;
      gstPlayer.iBlock = 0;
      continue;
    }

    /** Next turn actions */
    iDrawMultipleCard(INIT_HAND_CARDS, pstDeck);
    vSleepSeconds(3);
    gstPlayer.iEnergy = PLAYER_ENERGY_MAX;

    vTraceDeck(pstDeck, TRACE_DECK_ALL);
  }
}
