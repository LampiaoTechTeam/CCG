#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <battle.h>
#include <deck.h>
#include <monster.h>
#include <input.h>
#include <player.h>
#include <terminal_utils.h>

int iSelectMonsterFromList(int iMonsterCt){
  int iChoice;
  char szLine[1024];
  
  sprintf(szLine, "\nEscolha um monstro (1..%d). 'q' para sair.", iMonsterCt);
  vPrintLine(szLine, INSERT_NEW_LINE);
  iChoice = iPortableGetchar();
  
  return (iChoice-48);
}

/**
 * @brief
 *  Imprime a mesa de jogo:
 *  - Dados do jogador
 *  - Cartas na mão
 *  - Monstros / inimigos
 */
void vShowTable(PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCount){
  vShowPlayer();
  vShowDeck(pstDeck);
  vShowMonsters(pastMonsters, iMonsterCount);
}

int iHandlePlayerActionByCard(PSTRUCT_CARD pstCard, PSTRUCT_MONSTER pastMonsters, int iMonsterCt){
  int iTarget = 0;
  int ii = 0;
  int bAlreadyApplied = FALSE;
  int bUsed = FALSE;

  /** TODO: Melhorar esta logica - Cartas terao macrotipos e depois tipos (suporte-heal, suporte- paralize, offensivo-strike, etc.) */
  switch ( pstCard->iType ){
    case CARD_STRIKE:
    case CARD_FIREBALL:
    case CARD_POISON:

      if ( iAliveMonsterQty(pastMonsters, iMonsterCt) == 1 ){
        iTarget = iGetFirstAliveMonster(pastMonsters, iMonsterCt);
      }
      else if ( pstCard->iTarget != CARD_TARGET_MULTIPLE && iMonsterCt > 1 ){
        if ( (iTarget = iSelectMonsterFromList(iMonsterCt)) < 0 || iTarget > iMonsterCt )
          return -1;
          
        iTarget--;
      }
      
      if ( pstCard->iTarget == CARD_TARGET_MULTIPLE ){
        bAlreadyApplied = FALSE;
        iTarget = iMonsterCt;
      }

      for ( ; ii <= iTarget; ii++ ) {
        char szLine[1024];
        if ( iTarget < iMonsterCt ) ii = iTarget;
        if ( pstCard->iTarget == CARD_TARGET_MULTIPLE && ii == iTarget ) break;
        if ( pastMonsters[ii].iHP <= 0 ) continue;

        if ( pstCard->iType == CARD_POISON ) {
          pastMonsters[ii].astDebuff[pastMonsters[ii].iDebuffCt].iType   = DEBUFF_TYPE_POISON;
          pastMonsters[ii].astDebuff[pastMonsters[ii].iDebuffCt].iDamage = pstCard->iValue;
          pastMonsters[ii].astDebuff[pastMonsters[ii].iDebuffCt].iRounds = DEBUFF_POISON_CYCS;
          pastMonsters[ii].iDebuffCt++;
          vPrintLine("Voce aplicou Veneno!", INSERT_NEW_LINE);
          bUsed = TRUE;
        }
        else {
          pastMonsters[ii].iBlock  -= pstCard->iValue;
          if (pastMonsters[ii].iBlock < 0) {
            pastMonsters[ii].iHP   -= (pastMonsters[ii].iBlock)*-1;
            pastMonsters[ii].iBlock = 0;
          }
          else
            pastMonsters[ii].iHP   -= pstCard->iValue;
            
          memset(szLine,0,sizeof(szLine));
          sprintf(szLine, "Voce utilizou %s:", pstCard->szName);
          
          if ( !bAlreadyApplied )
            vPrintLine(szLine, INSERT_NEW_LINE);

          if ( pstCard->iTarget == CARD_TARGET_MULTIPLE  ){
            bAlreadyApplied = TRUE;
          }
          sprintf(szLine, " - Causando %d dano a %s", pstCard->iValue, pastMonsters[ii].szName);
          vPrintLine(szLine, INSERT_NEW_LINE);

          bUsed = TRUE;
          
          if ( iTarget < iMonsterCt ) ii++;
        }
      }
      break;
    case CARD_DEFEND:
      gstPlayer.iBlock += pstCard->iValue;
      vPrintLine("Voce se defendeu.", INSERT_NEW_LINE);
      bUsed = TRUE;
      break;
    case CARD_HEAL:
      gstPlayer.iHP    += pstCard->iValue;
      if ( gstPlayer.iHP >= PLAYER_HP_MAX )
        gstPlayer.iHP = PLAYER_HP_MAX;
      
      bUsed = TRUE;
      vPrintLine("Voce se curou.", INSERT_NEW_LINE);
      break;
    case CARD_PARALIZE:
      // deixar o monster sem jogar por x turnos
      // rolar um dado de 1 a 4 para definir o x turnos

      int iRoundsParalized = 0;
      int iMonsterTarget = 0;
      int iDebuffCt = 0;
      char szLine[63];
      char* pszRoundStr = "rodadas";
      
      time_t lTime;
  
      time(&lTime);
      srand(lTime);

      iRoundsParalized = (rand() % 2) + 2;

      if ( iAliveMonsterQty(pastMonsters, iMonsterCt) == 1) {
        iMonsterTarget = iGetFirstAliveMonster(pastMonsters, iMonsterCt);
      } else {
        iMonsterTarget = iSelectMonsterFromList(iMonsterCt);
      }

      iDebuffCt = pastMonsters[iMonsterTarget].iDebuffCt;

      pastMonsters[iMonsterTarget].astDebuff[iDebuffCt].iType = DEBUFF_TYPE_PARALYZE;
      pastMonsters[iMonsterTarget].astDebuff[iDebuffCt].iDamage = pstCard->iValue;
      pastMonsters[iMonsterTarget].astDebuff[iDebuffCt].iRounds = iRoundsParalized;
      pastMonsters[iMonsterTarget].iDebuffCt ++;

      if ( iRoundsParalized == 1 )
        pszRoundStr = "rodada";

      memset(szLine, 0, sizeof(szLine));
      snprintf(szLine, sizeof(szLine),
        "Voce aplicou paralisia a o monstro %d por %d %s",
        iMonsterTarget,
        iRoundsParalized,
        pszRoundStr
      );
      vPrintLine(szLine, INSERT_NEW_LINE);
      vSleepSeconds(2);
      vTraceMonsters(pastMonsters, iMonsterCt);
      break;
    default:
      return CARD_NONE;

    vSleepSeconds(3);
  }
  
  if ( !bUsed ) return CARD_NULL; 
  
  return pstCard->iType;
}

void vPlayCard(int iCardIndex, PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCount){
  PSTRUCT_CARD pstCard;

  if (iCardIndex <= 0 || iCardIndex > pstDeck->iHandCount)
    return;

  pstCard = &pstDeck->astHand[iCardIndex - 1];
  
  if (gstPlayer.iEnergy <= 0 || pstCard->iCost > gstPlayer.iEnergy ) {
    char szMsg[128];
    vTraceVarArgsFn("Not enough energy to play card[%s]", pstCard->szName);
    sprintf(szMsg, "Energia insuficiente [%d/%d]", pstCard->iCost, gstPlayer.iEnergy);
    vPrintHighlitedLine(szMsg, INSERT_NEW_LINE);
    return;
  }

  vPrintHighlitedLine("Carta Escolhida: ", NO_NEW_LINE);
  vPrintHighlitedLine(pstCard->szName, INSERT_NEW_LINE);
  vSleepSeconds(1);
  
  if ( iHandlePlayerActionByCard(pstCard, pastMonsters, iMonsterCount) == CARD_NONE ) 
    return;
  
  vSleepSeconds(2);
  gstPlayer.iEnergy -= pstCard->iCost;
  vDiscardCard(pstDeck, (iCardIndex - 1));

  vTraceVarArgsFn("Energia restante=%d", gstPlayer.iEnergy);
}
