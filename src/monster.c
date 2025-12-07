#include <debuff.h>
#include <deck.h>
#include <trace.h>
#include <terminal_utils.h>
#include <monster.h>
#include <player.h>
#include <input.h>

int giMaxMonsterHP = MONSTER_INITAL_HP_MAX;

void vTraceMonster(PSTRUCT_MONSTER pstMonster){
  char szLine[1024];

  snprintf(szLine, sizeof(szLine),
"%s - HP:%d Blk:%d Atk:%d", 
    pstMonster->szName,
    pstMonster->iHP,
    pstMonster->iBlock,
    pstMonster->iAttack
  );
  vTraceVarArgsFn("[%s]", szLine);

  vTraceDebuffList(pstMonster->astDebuff, pstMonster->iDebuffCt);
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

  if ( (pstWrkDbf = pstDebuff) == NULL || iDebuffCt <= 0 ) return;

  for ( ii = 0; ii < iDebuffCt; ii++ ){
    if ( pstWrkDbf->iType == DEBUFF_TYPE_POISON && pstWrkDbf->iRounds > 0 ){
      char szLine[1024];
      memset(szLine, 0, sizeof(szLine));
      vPrintLine(" | ", NO_NEW_LINE);
      snprintf(szLine, sizeof(szLine),
    "Veneno Turnos=%d/%d",
        pstWrkDbf->iDamage,
        pstWrkDbf->iRounds
      );
      vPrintColored(szLine, TERMINAL_COLOR_BGREEN);
    }
    else if ( pstWrkDbf->iType == DEBUFF_TYPE_PARALYZE && pstWrkDbf->iRounds > 0 ) {
      char szLine[1024];
      memset(szLine, 0, sizeof(szLine));
      vPrintLine(" | ", NO_NEW_LINE);
      snprintf(szLine, sizeof(szLine),
        "Paralizia Turnos=%d",
        pstWrkDbf->iRounds
      );
      vPrintColored(szLine, TERMINAL_COLOR_BMAGENTA);
    }
    pstWrkDbf++;
  }

}


void vTraceDebuffList(PSTRUCT_DEBUFF pstDebuff, int iDebuffCt){
  int ii;
  PSTRUCT_DEBUFF pstWrkDbf;

  if ( (pstWrkDbf = pstDebuff) == NULL || iDebuffCt <= 0 ) return;
  
  for ( ii = 0; ii < iDebuffCt; ii++ ){
    char szLine[1024];
    snprintf(szLine, sizeof(szLine),
  "Tipo:%d Dmg:%d Cycles:%d", 
      pstWrkDbf->iType,
      pstWrkDbf->iDamage,
      pstWrkDbf->iRounds
    );
    vTraceVarArgsFn("|-->%s - [%s]", ii, pszDebuffTypeDesc[pstWrkDbf->iType], szLine);
    pstWrkDbf++;
  }

}

void vShowMonsters(PSTRUCT_MONSTER pastMonster, int iCount) {
  int ii;
  char szLine[1024];

  memset(szLine, 0, sizeof(szLine));
  snprintf(szLine, sizeof(szLine), 
"  |-----------------------------------|"
  );
  vPrintLine(szLine, INSERT_NEW_LINE);

  for (ii = 0; ii < iCount; ii++) {
    if (pastMonster[ii].iHP > 0) {
      memset(szLine, 0, sizeof(szLine));
      snprintf(szLine, sizeof(szLine),
    "  [%d] %s ",
        ii+1,
        pastMonster[ii].szName
      );
      vPrintColored(szLine, TERMINAL_COLOR_BLUE);
      vPrintLine(" (", NO_NEW_LINE);
      snprintf(szLine, sizeof(szLine),
    "HP: %d/%d",
        pastMonster[ii].iHP,
        giMaxMonsterHP
      );
      vPrintColored(szLine, TERMINAL_COLOR_BRED);
      
      vPrintLine(" | ", NO_NEW_LINE);
      snprintf(szLine, sizeof(szLine),
    "ESCUDO: %d",
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

void vFixDebuffs(PSTRUCT_MONSTER pstMonster) {
  if ( pstMonster->iDebuffCt ){
    iClearDebuff(&pstMonster->astDebuff[0], pstMonster->iDebuffCt);
    if ( pstMonster->iDebuffCt < 0 ) 
      pstMonster->iDebuffCt = 0;
    vTraceMonster(pstMonster);
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
    int bIsParalized = FALSE;

    /* monstro morto ou paralisado não age */
    if (pastMonster[ii].iHP <= 0) continue;

    iChoice = rand() % 3; /* 0 = attack, 1 = defend, 2 = heal */
    
    for ( jj = 0; jj < pastMonster[ii].iDebuffCt; jj++ ){
      pstDebuff = &pastMonster[ii].astDebuff[jj];
      if (pstDebuff->iType == DEBUFF_TYPE_POISON && pstDebuff->iRounds > 0){
        int iDamage = pstDebuff->iDamage;
        if (pastMonster[ii].iBlock > 0) {
          pastMonster[ii].iBlock -= iDamage;
          if (pastMonster[ii].iBlock < 0) {
            pastMonster[ii].iHP -= (pastMonster[ii].iBlock)*-1;
            pastMonster[ii].iBlock = 0;
          }
        } else {
          pastMonster[ii].iHP -= iDamage;
        }        

        pstDebuff->iRounds--;

        snprintf(szLine, sizeof(szLine),
      "%s recebe %d de dano do veneno.",
          pastMonster[ii].szName,
          pstDebuff->iDamage
        );
        vPrintLine(szLine, INSERT_NEW_LINE);

        if (pastMonster[ii].iHP <= 0) break;
      }
      if (pstDebuff->iType == DEBUFF_TYPE_PARALYZE && pstDebuff->iRounds > 0 && !bIsParalized){
        pstDebuff->iRounds--;
        snprintf(szLine, sizeof(szLine),
          "%s esta paralizado.",
          pastMonster[ii].szName
        );
        vPrintLine(szLine, INSERT_NEW_LINE);
        bIsParalized = TRUE;
      }
    }

    vFixDebuffs(&pastMonster[ii]);

    if (bIsParalized || pastMonster[ii].iHP <= 0){
      continue;
    }

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
    "%s se defende +3 de escudo (total=%d).",
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
  }
  
  vTraceMonsters(pastMonster, iMonsterCount);
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
  
  vTraceVarArgsFn("Alive Monsters=%d", iCt);

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
    //////////
    pastMonster[ii].iHP = 10;
    iAtkBase = 3 + ii + (iLevel - 1);
    pastMonster[ii].iAttack            = iAtkBase;
    pastMonster[ii].iBlock             = 0;
    pastMonster[ii].iDebuffCt          = 0;
    memset(&pastMonster[ii].astDebuff, 0, sizeof(STRUCT_DEBUFF)*10);
  }
  vTraceVarArgsFn("Nivel %d: iniciados %d monstros.", iLevel, iCount);
  vTraceMonsters(pastMonster, iCount);
}

int iClearDebuff(PSTRUCT_DEBUFF pstDebuff, int iDebuffCt){
  int ii;
  int jj=0;
  PSTRUCT_DEBUFF pWrkDebuff = pstDebuff;

  for (ii = 0; ii < iDebuffCt; ii++){
    if ( pWrkDebuff->iRounds <= 0 ){
      pWrkDebuff->iType = DEBUFF_TYPE_NONE;
      pWrkDebuff->iRounds = 0;
      jj++;
    }
  }
  return jj;
}
