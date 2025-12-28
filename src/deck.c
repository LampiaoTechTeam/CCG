#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys_interface.h>
#include <card_game.h>
#include <debuff.h>
#include <trace.h>
#include <terminal_utils.h>
#include <deck.h>

char *pszCardTypeDesc[] ={
  " ",
  "Strike",
  "Block",
  "Heal",
  "Spell",
  "Veneno",
  "Paralisar",
  NULL
};
char *pszDebuffTypeDesc[] ={
  " ",
  "Envenenado",
  "Paralisado",
  NULL
};
char *pszTargetableDesc[] ={
  " ",
  "Single-Targt",
  "Area (AOE)",
  "Self-Targt",
  NULL
};

int iGetMinimumEnergy(PSTRUCT_DECK pstDeck){
  int ii;
  int iMinCost = CARD_MAX_COST;

  if ( pstDeck->iHandCount <= 0 )
    return -1;
  
  for (ii = 0; ii < pstDeck->iHandCount; ii++){
    if ( iMinCost > pstDeck->astHand[ii].iCost )
      iMinCost = pstDeck->astHand[ii].iCost;
  }

  return iMinCost;
}
void vTraceCard(PSTRUCT_CARD pstCard){
  if ( pstCard == NULL ){
    vTraceVarArgsFn("pstCard == NULL");
    return;
  }
  vTraceVarArgsFn(
  "\t| Name=[%10.10s] Ty=%-6.6s Cost=%02d Val=%02d Targ=%-10.10s",
      pstCard->szName,
      pszCardTypeDesc[pstCard->iType],
      pstCard->iCost,
      pstCard->iValue,
      pszTargetableDesc[pstCard->iTarget]
    );
}
void vTraceCardList(STRUCT_CARD astCardList[], int iListCt){
  int ii;

  if ( iListCt <= 0 )
    return ;

  for (ii = 0; ii < iListCt; ii++)
    vTraceCard(&astCardList[ii]);
  
}

void vTraceDeck(PSTRUCT_DECK pstDeck, int iTraceOption){

  if ( TEST_TRACE_OPT(iTraceOption, TRACE_DRAW_PILE) ){
    vTraceVarArgsFn("%d cartas na pilha de compra:", pstDeck->iDrawCount);
    vTraceCardList(pstDeck->astDraw, pstDeck->iDrawCount);
  }
  if ( TEST_TRACE_OPT(iTraceOption, TRACE_HAND) ){
    vTraceVarArgsFn("%d cartas na mão do jogador:", pstDeck->iHandCount);
    vTraceCardList(pstDeck->astHand, pstDeck->iHandCount);
  }
  if ( TEST_TRACE_OPT(iTraceOption, TRACE_DISCARD_PILE) ){
    vTraceVarArgsFn("%d cartas na pilha de descarte:", pstDeck->iDiscardCount);
    vTraceCardList(pstDeck->astDiscard, pstDeck->iDiscardCount);
  }
}

void vInitBasicDeck(PSTRUCT_DECK pstDeck)
{
  int ii;
  pstDeck->iDrawCount = 0;
  pstDeck->iDiscardCount = 0;
  pstDeck->iHandCount = 0;
  
  memset(pstDeck->astDiscard, 0, MAX_DECK*sizeof(STRUCT_CARD));
  memset(pstDeck->astDraw,    0, MAX_DECK*sizeof(STRUCT_CARD));
  memset(pstDeck->astHand,    0, MAX_HAND*sizeof(STRUCT_CARD));

  /* 20 cartas: 8 Strike, 6 Defend, 4 Heal, 2 Fireball */
  for (ii = 0; ii < 8; ii++)
    pstDeck->astDraw[pstDeck->iDrawCount++] = stMakeCard(CARD_STRIKE, "Strike", 1, 6, CARD_TARGET_SINGLE);
  for (ii = 0; ii < 6; ii++)
    pstDeck->astDraw[pstDeck->iDrawCount++] = stMakeCard(CARD_DEFEND, "Defend", 2, 5, CARD_TARGET_SELF);
  for (ii = 0; ii < 4; ii++)
    pstDeck->astDraw[pstDeck->iDrawCount++] = stMakeCard(CARD_HEAL, "Heal", 1, 3, CARD_TARGET_SELF);
  for (ii = 0; ii < 2; ii++)
    pstDeck->astDraw[pstDeck->iDrawCount++] = stMakeCard(CARD_FIREBALL, "Fireball", 1, 4, CARD_TARGET_MULTIPLE);
  
  vShuffle(pstDeck->astDraw, pstDeck->iDrawCount);
  if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("Deck inicial embaralhado com %d cartas.", pstDeck->iDrawCount);
  if ( DEBUG_LVL_MORE_DETAILS ) vTraceDeck(pstDeck, TRACE_DECK_ALL);
}

/* descartar uma carta específica da mão (índice 0..iHandCount-1) */
void vDiscardCard(PSTRUCT_DECK pstDeck, int iIndex)
{
  int ii;

  if (iIndex < 0 || iIndex >= pstDeck->iHandCount) return;

  memcpy(&pstDeck->astDiscard[pstDeck->iDiscardCount++], &pstDeck->astHand[iIndex], sizeof(STRUCT_CARD));
  if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("Moved [%s] from hand -> to discard pile.", pstDeck->astHand[iIndex].szName);
  if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("Discard pile %d cards", pstDeck->iDiscardCount);
  memset(&pstDeck->astHand[iIndex], 0, sizeof(STRUCT_CARD));
  for (ii = iIndex; ii < pstDeck->iHandCount - 1; ii++)
    pstDeck->astHand[ii] = pstDeck->astHand[ii + 1];

  pstDeck->iHandCount--;
}

void vAddCardToDiscard(PSTRUCT_DECK pstDeck, STRUCT_CARD stCard)
{
  if (pstDeck->iDiscardCount < MAX_DECK)
    memcpy(&pstDeck->astDiscard[pstDeck->iDiscardCount++], &stCard, sizeof(STRUCT_CARD));
}

/* procura por nome nas zonas (mao -> draw -> discard) e aplica upgrade */
int iUpgradeFirstCardByName(PSTRUCT_DECK pstDeck, const char *kpszName, int iDeltaValue, int iDeltaCost)
{
  int ii;
  /* mao */
  for (ii = 0; ii < pstDeck->iHandCount; ii++) {
    if (strcmp(pstDeck->astHand[ii].szName, kpszName) == 0) {
      pstDeck->astHand[ii].iValue += iDeltaValue;
      pstDeck->astHand[ii].iCost += iDeltaCost;
      if (pstDeck->astHand[ii].iCost < 0) pstDeck->astHand[ii].iCost = 0;
      if (strchr(pstDeck->astHand[ii].szName, '+') == NULL) strcat(pstDeck->astHand[ii].szName, "+");
      return 1;
    }
  }
  /* draw */
  for (ii = 0; ii < pstDeck->iDrawCount; ii++) {
    if (strcmp(pstDeck->astDraw[ii].szName, kpszName) == 0) {
      pstDeck->astDraw[ii].iValue += iDeltaValue;
      pstDeck->astDraw[ii].iCost += iDeltaCost;
      if (pstDeck->astDraw[ii].iCost < 0) pstDeck->astDraw[ii].iCost = 0;
      if (strchr(pstDeck->astDraw[ii].szName, '+') == NULL) strcat(pstDeck->astDraw[ii].szName, "+");
      
      return 1;
    }
  }
  /* discard */
  for (ii = 0; ii < pstDeck->iDiscardCount; ii++) {
    if (strcmp(pstDeck->astDiscard[ii].szName, kpszName) == 0) {
      pstDeck->astDiscard[ii].iValue += iDeltaValue;
      pstDeck->astDiscard[ii].iCost += iDeltaCost;
      if (pstDeck->astDiscard[ii].iCost < 0) pstDeck->astDiscard[ii].iCost = 0;
      if (strchr(pstDeck->astDiscard[ii].szName, '+') == NULL) strcat(pstDeck->astDiscard[ii].szName, "+");

      return 1;
    }
  }
  return 0;
}

/* ---------------- Funções utilitárias ---------------- */
void vSwapCards(PSTRUCT_CARD pstA, PSTRUCT_CARD pstB)
{
  STRUCT_CARD stTmp;
  stTmp = *pstA;
  *pstA = *pstB;
  *pstB = stTmp;
}

/**
 * @brief Ordena as cartas na mão por nome (ordem alfabética)
 */
void vSortHandByName(PSTRUCT_DECK pstDeck) {
  int ii, jj;
  for (ii = 0; ii < pstDeck->iHandCount - 1; ii++) {
    for (jj = ii + 1; jj < pstDeck->iHandCount; jj++) {
      if (strcmp(pstDeck->astHand[ii].szName, pstDeck->astHand[jj].szName) > 0) {
        vSwapCards(&pstDeck->astHand[ii], &pstDeck->astHand[jj]);
      }
    }
  }
  if ( DEBUG_LVL_DETAILS ){
    vTraceVarArgsFn(
  "%d cartas ordenada%salfabeticamente.",
      pstDeck->iHandCount,
      pstDeck->iHandCount > 1 ? "s ": " "
    );
  }
  if ( DEBUG_LVL_MORE_DETAILS ) vTraceDeck(pstDeck, TRACE_HAND);
}

void vSortDiscardByName(PSTRUCT_DECK pstDeck) {
  int ii, jj;
  for (ii = 0; ii < pstDeck->iDiscardCount - 1; ii++) {
    for (jj = ii + 1; jj < pstDeck->iDiscardCount; jj++) {
      if (strcmp(pstDeck->astDiscard[ii].szName, pstDeck->astDiscard[jj].szName) < 0) {
        vSwapCards(&pstDeck->astDiscard[ii], &pstDeck->astDiscard[jj]);
      }
    }
  }
  if ( DEBUG_LVL_DETAILS ) 
    vTraceVarArgsFn("Pilha de descarte ordenada (%d cartas).", pstDeck->iDiscardCount);
}

void vShuffle(PSTRUCT_CARD paCards, int iCount) {
  int ii;
  int jj;

  for (ii = iCount - 1; ii > 0; ii--) {
    jj = rand() % (ii + 1);
    vSwapCards(&paCards[ii], &paCards[jj]);
  }
}

int iDrawMultipleCard(int iCardCt, PSTRUCT_DECK pstDeck)
{
  int ii = 0;
  for (ii = 0; ii < iCardCt; ii++){
    iDrawCard(pstDeck);
  }
  
  return ii;
}

int iDrawCard(PSTRUCT_DECK pstDeck)
{
  int ii;
  STRUCT_CARD stCard;
  char szMsg[_MAX_EXT];

  if ( DEBUG_LVL_DETAILS )
    vTraceVarArgsFn("Card Count Hand:%d Draw:%d Discard:%d", pstDeck->iHandCount, pstDeck->iDrawCount, pstDeck->iDiscardCount);

  if (pstDeck->iHandCount >= MAX_HAND) {
    if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("Mao cheia, nao pode comprar.");
    return 0;
  }
  if (pstDeck->iDrawCount <= 0) {
    if (pstDeck->iDiscardCount <= 0) {
      if ( DEBUG_LVL_DETAILS ) vTraceVarArgsFn("Nao ha cartas para comprar.");
      return 0;
    }

    pstDeck->iDrawCount    = 0;
    memset(szMsg, 0, sizeof(szMsg));
    sprintf(szMsg, "Embaralhando %d cartas do cemiterio", pstDeck->iDiscardCount);
    vPrintLine(szMsg, INSERT_NEW_LINE);
    
    vShuffle(pstDeck->astDiscard, pstDeck->iDiscardCount);
    for (ii = 0; ii < pstDeck->iDiscardCount; ii++) {
      if ( pstDeck->astDiscard[ii].iCost == 0 )
        continue;
      
      memcpy(&pstDeck->astDraw[ii], &pstDeck->astDiscard[ii], sizeof(STRUCT_CARD));
      memset(&pstDeck->astDiscard[ii], 0, sizeof(STRUCT_CARD));
      pstDeck->iDrawCount++;
    }

    vSortDiscardByName(pstDeck);

    /** TODO: */
    pstDeck->iDiscardCount = 
      (pstDeck->iDiscardCount - pstDeck->iDrawCount) < 0 
        ? 0
        : (pstDeck->iDiscardCount - pstDeck->iDrawCount);

    vShuffle(pstDeck->astDraw, pstDeck->iDrawCount);
    if ( DEBUG_LVL_DETAILS ) 
      vTraceVarArgsFn("Recycle: descarte movido para draw (%d cartas).", pstDeck->iDrawCount);
  }
  stCard = pstDeck->astDraw[pstDeck->iDrawCount - 1];
  pstDeck->iDrawCount--;
  pstDeck->astHand[pstDeck->iHandCount++] = stCard;
  memset(szMsg, 0, sizeof(szMsg));
  sprintf(szMsg, "Comprou : %s", stCard.szName);
  vPrintLine(szMsg, INSERT_NEW_LINE);

  return 1;
}
void vAddDiscardPile2Deck(PSTRUCT_DECK pstDeck){
  int ii;
  
  for (ii = 0; ii < pstDeck->iDiscardCount; ii++)  
    memcpy(&pstDeck->astDraw[pstDeck->iDrawCount++] , &pstDeck->astDiscard[ii], sizeof(STRUCT_CARD));
    
  memset(pstDeck->astDiscard, 0, MAX_DECK*sizeof(STRUCT_CARD));
  pstDeck->iDiscardCount = 0;
}
void vDiscardHand(PSTRUCT_DECK pstDeck)
{
  int ii;
  for (ii = 0; ii < pstDeck->iHandCount; ii++)
    memcpy(&pstDeck->astDiscard[pstDeck->iDiscardCount++], &pstDeck->astHand[ii], sizeof(STRUCT_CARD));
  
  memset(pstDeck->astHand, 0, MAX_HAND*sizeof(STRUCT_CARD));
  pstDeck->iHandCount = 0;
}

void vDiscardDraw(PSTRUCT_DECK pstDeck)
{
  int ii;
  for (ii = 0; ii < pstDeck->iDrawCount; ii++)
    memcpy(&pstDeck->astDiscard[pstDeck->iDiscardCount++], &pstDeck->astDraw[ii], sizeof(STRUCT_CARD));  
    
  memset(pstDeck->astDraw, 0, MAX_DECK*sizeof(STRUCT_CARD));
  pstDeck->iDrawCount = 0;
}

void vShowDeck(PSTRUCT_DECK pstDeck) {
  int ii;
  char szLine[1024];

  memset(szLine, 0, sizeof(szLine));
  snprintf(szLine, sizeof(szLine), 
"  |-----------------------------------|"
  );
  vPrintLine(szLine, INSERT_NEW_LINE);

  memset(szLine, 0, sizeof(szLine));
  snprintf(szLine, sizeof(szLine),
"%6.6s Carta%3.3sCusto  Tipo  Valor/Dano\n",
    " ", " "
  );
  vPrintColored(szLine, TERMINAL_COLOR_BWHITE);

  for (ii = 0; ii < pstDeck->iHandCount; ii++) {
    int iLineCt = ii+1;
    int iPadding = (iLineCt % (MAX_HAND+1)) ?  1: 0;
    char szFormat[128];
    
    sprintf(szFormat, "%d", pstDeck->astHand[ii].iValue);
    if(pstDeck->astHand[ii].iType == CARD_PARALIZE) {
      sprintf(szFormat, "\?\?/%d", DEBUFF_PARALISE_CYCS);
    }
    memset(szLine, 0, sizeof(szLine));
    snprintf(szLine, sizeof(szLine),
    "  [%d]%*.*s%-*.*s%5.5s%d%4.4s%-6.6s%4.4s%s",
      iLineCt, iPadding, iPadding, " ", 
      MAX_HAND, MAX_HAND, pstDeck->astHand[ii].szName,
      " ",
      pstDeck->astHand[ii].iCost,
      " ",
      pszCardTypeDesc[pstDeck->astHand[ii].iType],
      " ",
      szFormat
    );
    vPrintLine(szLine, INSERT_NEW_LINE);
  }
  vPrintLine(" ", INSERT_NEW_LINE);
} 

STRUCT_CARD stMakeCard(int iType, const char *pszName, int iCost, int iValue, int iTarget) {
  STRUCT_CARD st;
  st.iType = iType;
  st.iCost = iCost;
  st.iValue = iValue;
  st.iTarget = iTarget;
  strncpy(st.szName, pszName, sizeof(st.szName) - 1);
  st.szName[sizeof(st.szName) - 1] = '\0';
  return st;
}
