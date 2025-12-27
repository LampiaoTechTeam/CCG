/**
 * monster.h
 *
 * Written by Renato Fermi <repiazza@gmail.com>
 *
 * Description: Monster module
 */

#ifndef MONSTER_H
  #define MONSTER_H

  #define MAX_MONSTERS             20
  #define MONSTER_INITAL_HP_MAX    50
  #define MONSTER_HP_SCALING_RATE  0.05
  #define MONSTER_HEAL_VALUE       5
  #define MONSTER_MAX_DEBUFFS      10

  extern int giMaxMonsterHP;

  typedef struct STRUCT_MONSTER {
    char szName[64];
    int iHP;
    int iAttack;
    int iBlock;
    int iDebuffCt;
    STRUCT_DEBUFF astDebuff[MONSTER_MAX_DEBUFFS]; 
  } STRUCT_MONSTER, *PSTRUCT_MONSTER;
  int iGetDebuffLeftCycles(PSTRUCT_MONSTER pstMonster, int iDebuffType);
  int iGetDebuffValue(PSTRUCT_MONSTER pstMonster, int iDebuffType);
  void vInitMonsters(PSTRUCT_MONSTER paMonsters, int iCount);
  void vShowMonsters(PSTRUCT_MONSTER paMonsters, int iCount);
  void vDoEnemyActions(PSTRUCT_MONSTER paMonsters, int iMonsterCount);
  int  iIsMonsterAlive(PSTRUCT_MONSTER pstMonster);
  int  iAnyMonsterAlive(PSTRUCT_MONSTER paMonsters, int iCount);
  int  iMonsterCountForLevel(int iLevel);
  void vInitMonstersForLevel(PSTRUCT_MONSTER paMonsters, int iLevel, int *piOutCount);
  int iGetFirstAliveMonster(PSTRUCT_MONSTER pastMonster, int iCount);
  int iAliveMonsterQty(PSTRUCT_MONSTER pastMonster, int iCount);
  int iClearDebuff(PSTRUCT_DEBUFF pstDebuff, int iDebuffCt);
  void vTraceMonsters(PSTRUCT_MONSTER pastMonster, int iMonsterCt);
  void vTraceMonster(PSTRUCT_MONSTER pstMonster);
  void vTraceDebuffList(PSTRUCT_DEBUFF pstDebuff, int iDebuffCt);
  
#endif
