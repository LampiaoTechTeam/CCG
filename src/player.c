/**
 * player.c
 *
 * Handles player data structures and actions
 *
 * Written by Renato Fermi<repiazza@gmail.com> in September 2025
 * Maintained by Gustavo <> since September 2025
 *
 */

#include <deck.h>
#include <debuff.h>
#include <trace.h>
#include <terminal_utils.h>
#include <monster.h>
#include <player.h>
#include <battle.h> 
#include <input.h>

#ifdef USE_SDL2
  SDL_Rect gSDL_Player_Rect;
#endif
void vTracePlayer(){
  char szDbg[1024];

  memset(szDbg, 0, sizeof(szDbg));
  sprintf(szDbg,
    "Jogador =[%s] "
    "Energia =[%d] "
    "Escudo  =[%d] "
    "Dinheiro=[%d] ",
    gstPlayer.szPlayerName,
    gstPlayer.iEnergy,
    gstPlayer.iBlock,
    gstPlayer.iGold
  );
  vTraceVarArgsFn(szDbg);
  vTraceDeck(gstPlayer.pstPlayerCards, TRACE_DECK_ALL);
  vTraceDebuffList(gstPlayer.stDebuff, gstPlayer.iDebuffCt);
}

void vInitPlayer(PSTRUCT_DECK pstGameDeck, int bReadName){
  gstPlayer.iHP = PLAYER_HP_MAX;
  gstPlayer.iEnergy = PLAYER_ENERGY_MAX;
  gstPlayer.iBlock = 0;
  gstPlayer.iGold = 30;

  if ( bReadName )
    vReadPlayerName(gstPlayer.szPlayerName, PLAYER_NAME_SIZE);
 
  gstPlayer.pstPlayerCards = pstGameDeck;
}


// void vSDL_ShowPlayer(){

// }
void vShowPlayer(){
  int ii;
  char szLine[1024];

  if (gbSDL_Mode){
    // vSDL_ShowPlayer();
    return ;
  }
  memset(szLine, 0, sizeof(szLine));
  vPrintLine(" ", INSERT_NEW_LINE);

  snprintf(szLine, sizeof(szLine),
           "%4.4s%-10.10s",
           " ",
           "Jogador");
  vPrintColored(szLine, TERMINAL_COLOR_BWHITE);

  snprintf(szLine, sizeof(szLine),
           "     HP  ");
  vPrintColored(szLine, TERMINAL_COLOR_BWHITE);

  snprintf(szLine, sizeof(szLine),
           "     ESCUDO");
  vPrintColored(szLine, TERMINAL_COLOR_BWHITE);

  snprintf(szLine, sizeof(szLine),
           "    ENERGIA");
  vPrintColored(szLine, TERMINAL_COLOR_BWHITE);

  vPrintLine(" ", INSERT_NEW_LINE);

  snprintf(szLine, sizeof(szLine),
           "%4.4s%-10.10s",
           " ",
           gstPlayer.szPlayerName);
  vPrintColored(szLine, TERMINAL_COLOR_BBLUE);

  snprintf(szLine, sizeof(szLine),
           " [%d/%d]",
           gstPlayer.iHP,
           PLAYER_HP_MAX);
  vPrintColored(szLine, TERMINAL_COLOR_RED);

  snprintf(szLine, sizeof(szLine),
           "     [%d]",
           gstPlayer.iBlock);
  vPrintColored(szLine, TERMINAL_COLOR_BCYAN);

  snprintf(szLine, sizeof(szLine),
           "       [%d/%d]",
           gstPlayer.iEnergy, PLAYER_ENERGY_MAX);
  vPrintColored(szLine, TERMINAL_COLOR_YELLOW);

  vPrintLine(" ", INSERT_NEW_LINE);

  for (ii = 0; ii < gstPlayer.iDebuffCt; ii++){
    if (gstPlayer.stDebuff[ii].iType == DEBUFF_TYPE_POISON){
      snprintf(szLine, sizeof(szLine),
      " Dano de Veneno(por turno)=%d  Turnos restantes=%d",
        gstPlayer.stDebuff[ii].iDamage,
        gstPlayer.stDebuff[ii].iRounds
      );
      vPrintColored(szLine, TERMINAL_COLOR_GREEN);
    }
  }
}

int iDoPlayerTurn(int *bRunning, PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonster, int iMonsterCt)
{
  int iCh;
  int iIdx;
  char szLine[1024];

  if (!bRunning)
    return 1;

  memset(szLine, 0, sizeof(szLine));
  sprintf(szLine,
          "\nEscolha carta (1..%d), 'e' encerra turno, 'q' sai:",
          pstDeck->iHandCount);

  vPrintLine(szLine, INSERT_NEW_LINE);
  iCh = iPortableGetchar();
  if (iCh == 'q')
  {
    bRunning = FALSE;
    return 1;
  }
  if (iCh == 'e')
  {
    return 1;
  }
  if (iCh >= '0' && iCh <= '9')
  {
    iIdx = iCh - '0';
    vPlayCard(iIdx, pstDeck, pastMonster, iMonsterCt);
    vClearTerminal();
    vSortHandByName(pstDeck);
    vShowTable(pstDeck, pastMonster, iMonsterCt);
    vTraceDeck(pstDeck, TRACE_DECK_ALL);
  }
  return 0;
}