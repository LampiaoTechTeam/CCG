#include <stdio.h>
#include <stdlib.h>
#include <monster.h>
#include <string.h>
#include <debuff.h>
#include <player.h>
#include <input.h>
#include <terminal_utils.h>

int giMaxMonsterHP = MONSTER_INITAL_HP_MAX;

void vTraceMonster(PSTRUCT_MONSTER pstMonster){
  int jj;
  char szLine[1024];

  snprintf(szLine, sizeof(szLine),
"%s - HP:%d/Blk:%d/Atk:%d", 
    pstMonster->szName,
    pstMonster->iHP,
    pstMonster->iBlock,
    pstMonster->iAttack
  );
  vTraceVarArgsFn("M [%s]", szLine);

  for ( jj = 0; jj < pstMonster->iDebuffCt; jj++ ){
    snprintf(szLine, sizeof(szLine),
  "Tipo:%d Dmg:%d Cycles:%d", 
      pstMonster->astDebuff[jj].iType,
      pstMonster->astDebuff[jj].iDamage,
      pstMonster->astDebuff[jj].iRounds
    );
    vTraceVarArgsFn("---|>Dbff%d [%s]", jj, szLine);
  }

}

void vTraceMonsters(PSTRUCT_MONSTER pastMonster, int iMonsterCt){
  int ii;
  for (ii = 0; ii < iMonsterCt; ii++) {
    vTraceMonster(&pastMonster[ii]);
  }
}

void vInitMonsters(PSTRUCT_MONSTER pastMonster, int iCount) {
  int ii;
  
  for (ii = 0; ii < iCount; ii++) {
    snprintf(pastMonster[ii].szName, sizeof(pastMonster[ii].szName),"Monstro%02d", ii+1);
    pastMonster[ii].iHP               = 20 + (ii * 5);
    pastMonster[ii].iBlock            = 0;
    pastMonster[ii].iAttack           = 3 + ii;
    pastMonster[ii].astDebuff->iType   = 0;
    pastMonster[ii].astDebuff->iDamage = 0;
    pastMonster[ii].astDebuff->iRounds = 0;
    pastMonster[ii].iDebuffCt = 0;
  }
  vTraceVarArgsFn("Iniciados %d monstros.", iCount);
}

void vShowDebuffList(PSTRUCT_DEBUFF pstDebuff, int iDebuffCt){
  int ii;
  PSTRUCT_DEBUFF pstWrkDbf;

  if ( (pstWrkDbf = pstDebuff) == NULL ) return;

  for ( ii = 0; ii < iDebuffCt; ii++ ){
    if ( pstWrkDbf->iType == DEBUFF_TYPE_POISON && pstWrkDbf->iRounds > 0 ){
      char szLine[1024];
      memset(szLine, 0, sizeof(szLine));
      vPrintLine(" | ", NO_NEW_LINE);
      snprintf(szLine, sizeof(szLine),
    "Veneno/Turnos=%d/%d",
        pstWrkDbf->iDamage,
        pstWrkDbf->iRounds
      );
      vPrintColored(szLine, TERMINAL_COLOR_BGREEN);
    }
    pstWrkDbf++;
  }

}

void vShowMonsters(PSTRUCT_MONSTER pastMonster, int iCount) {
  int ii;
  char szLine[1024];

  for (ii = 0; ii < iCount; ii++) {
    if (pastMonster[ii].iHP > 0) {
      memset(szLine, 0, sizeof(szLine));
      snprintf(szLine, sizeof(szLine),
    "  [%d] %s ",
        ii+1,
        pastMonster[ii].szName
      );
      vPrintColored(szLine, TERMINAL_COLOR_BBLUE);
      vPrintLine(" (", NO_NEW_LINE);
      snprintf(szLine, sizeof(szLine),
    "HP: %d/%d",
        pastMonster[ii].iHP,
        giMaxMonsterHP
      );
      vPrintColored(szLine, TERMINAL_COLOR_BRED);
      
      vPrintLine(" | ", NO_NEW_LINE);
      snprintf(szLine, sizeof(szLine),
    "BLOCK: %d",
        pastMonster[ii].iBlock
      );
      vPrintColored(szLine, TERMINAL_COLOR_BCYAN);
      vPrintLine(" | ", NO_NEW_LINE);
      snprintf(szLine, sizeof(szLine),
    "ATK: %d",
        pastMonster[ii].iAttack
      );
      vPrintColored(szLine, TERMINAL_COLOR_BMAGENTA);
      vShowDebuffList(&pastMonster[ii].astDebuff[0], pastMonster[ii].iDebuffCt);
      vPrintLine(")", INSERT_NEW_LINE);
    }
  }
}

void vDoEnemyActions(PSTRUCT_MONSTER pastMonster, int iMonsterCount) {
  int ii;
  char szLine[128];
  time_t lTime;
  
  time(&lTime);
  srand(lTime);

  for (ii = 0; ii < iMonsterCount; ii++) {
    int iChoice;
    int jj;
    PSTRUCT_DEBUFF pstDebuff;

    if (pastMonster[ii].iHP <= 0) continue; /* monstro morto não age */

    iChoice = rand() % 3; /* 0 = attack, 1 = defend, 2 = heal */

    vPrintLine("", INSERT_NEW_LINE);
    
    if (iChoice == 0) {
      int iDamage = pastMonster[ii].iAttack;
      if (gstPlayer.iBlock > 0) {
        gstPlayer.iBlock -= iDamage;
        if (gstPlayer.iBlock < 0) {
          gstPlayer.iHP -= (gstPlayer.iBlock)*-1;
          gstPlayer.iBlock = 0;
        }
      } else {
        gstPlayer.iHP -= iDamage;
      }
      snprintf(szLine, sizeof(szLine),
    "%s ataca causando %d de dano!",
        pastMonster[ii].szName,
        iDamage
      );
      vPrintLine(szLine, INSERT_NEW_LINE);
    } else if (iChoice == 1) {
      pastMonster[ii].iBlock += 3;
      snprintf(szLine, sizeof(szLine),
    "%s se defende e ganha 3 de bloqueio (total=%d).",
        pastMonster[ii].szName,
        pastMonster[ii].iBlock
      );
      vPrintLine(szLine, INSERT_NEW_LINE);
    } else {
      pastMonster[ii].iHP += MONSTER_HEAL_VALUE;
      if (pastMonster[ii].iHP > giMaxMonsterHP)
        pastMonster[ii].iHP = giMaxMonsterHP;
      
      snprintf(szLine, sizeof(szLine),
    "%s se cura em %d (HP=%d).",
        pastMonster[ii].szName, 
        MONSTER_HEAL_VALUE, 
        pastMonster[ii].iHP
      );
      vPrintLine(szLine, INSERT_NEW_LINE);
    }
    for ( jj = 0; jj < pastMonster[ii].iDebuffCt; jj++ ){
      pstDebuff = &pastMonster[ii].astDebuff[jj];
      if (pstDebuff->iType == DEBUFF_TYPE_POISON && pstDebuff->iRounds > 0){
        pastMonster[ii].iHP -= pstDebuff->iDamage;
        pstDebuff->iRounds--;
        snprintf(szLine, sizeof(szLine),
      "%s recebe %d de dano do veneno.",
          pastMonster[ii].szName,
          pstDebuff->iDamage
        );
        vPrintLine(szLine, INSERT_NEW_LINE);
      }
    }
    if ( pastMonster[ii].iDebuffCt ){
      // int iDbf = 0;
      vTraceMonster(&pastMonster[ii]);
      iClearDebuff(&pastMonster[ii].astDebuff[0], pastMonster[ii].iDebuffCt);
      // pastMonster[ii].iDebuffCt -= iDbf;
      if ( pastMonster[ii].iDebuffCt < 0 ) pastMonster[ii].iDebuffCt = 0;
      vTraceMonster(&pastMonster[ii]);
    }
    
    vSleepSeconds(2);
  }
}

int iGetFirstAliveMonster(PSTRUCT_MONSTER pastMonster, int iCount){
  int ii;
  if ( !iAnyMonsterAlive(pastMonster, iCount) ) return -1;
  
  for (ii = 0; ii < iCount; ii++)
    if (iIsMonsterAlive(&pastMonster[ii]))
      return ii;

  return -1;
}

int iIsMonsterAlive(PSTRUCT_MONSTER pstMonster) {
  return pstMonster->iHP > 0;
}

int iAnyMonsterAlive(PSTRUCT_MONSTER pastMonster, int iCount) {
  int ii;

  for (ii = 0; ii < iCount; ii++){
    if (iIsMonsterAlive(&pastMonster[ii]))
      return 1;
  }
  
  return 0;
}

int iAliveMonsterQty(PSTRUCT_MONSTER pastMonster, int iCount) {
  int ii;
  int iCt = 0;

  for (ii = 0; ii < iCount; ii++){
    if (iIsMonsterAlive(&pastMonster[ii]))
      iCt++;
  }
  
  return iCt;
}

int iMonsterCountForLevel(int iLevel) {
  int iCount;
  iCount = 2 + (iLevel - 1); /* cresce 1 por nível, começa em 2 */
  if (iCount > MAX_MONSTERS) iCount = MAX_MONSTERS;
  return iCount;
}

void vInitMonstersForLevel(PSTRUCT_MONSTER pastMonster, int iLevel, int *piOutCount) {
  int ii;
  int iCount;
  int iAtkBase;

  iCount = iMonsterCountForLevel(iLevel);
  *piOutCount = iCount;

  for (ii = 0; ii < iCount; ii++) {
    snprintf(
      pastMonster[ii].szName, sizeof(pastMonster[ii].szName), 
    "Monstro %d-%d", 
      iLevel,
      ii + 1
    );
    giMaxMonsterHP = (iLevel-1) *MONSTER_HP_SCALING_RATE * MONSTER_INITAL_HP_MAX;
    giMaxMonsterHP += MONSTER_INITAL_HP_MAX;
    pastMonster[ii].iHP = giMaxMonsterHP;
    iAtkBase = 3 + ii + (iLevel - 1);
    pastMonster[ii].iAttack            = iAtkBase;
    pastMonster[ii].iBlock             = 0;
    pastMonster[ii].iDebuffCt          = 0;
    memset(&pastMonster[ii].astDebuff, 0, sizeof(STRUCT_DEBUFF)*10);
  }
  vTraceVarArgsFn("Nivel %d: iniciados %d monstros.", iLevel, iCount);
}

int iClearDebuff(PSTRUCT_DEBUFF pstDebuff, int iDebuffCt){
  int ii;
  int jj=0;
  PSTRUCT_DEBUFF pWrkDebuff = pstDebuff;

  for (ii = 0; ii < iDebuffCt; ii++){
    if ( pWrkDebuff->iRounds <= 0 ){
      pWrkDebuff->iType = DEBUFF_TYPE_NONE;
      pWrkDebuff->iRounds = 0;
      // memcpy(pWrkDebuff, &(*(pWrkDebuff+sizeof(STRUCT_DEBUFF))), sizeof(STRUCT_DEBUFF));
      // pWrkDebuff++;
      // memset(pWrkDebuff, 0, sizeof(STRUCT_DEBUFF));
      jj++;
    }
  }
  return jj;
}