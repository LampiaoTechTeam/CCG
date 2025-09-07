#include <trace.h>
#include <stddef.h>

#ifndef DECK_H
  #define DECK_H

  #define MAX_DECK 50
  #define MAX_HAND 10
  #define INIT_HAND_CARDS 6

  #define CARD_STRIKE   0
  #define CARD_DEFEND   1
  #define CARD_HEAL     2
  #define CARD_FIREBALL 3
  #define CARD_POISON   4
  #define CARD_NULL    -1
  
  #define CARD_POISON_CYCS 3

  typedef struct STRUCT_CARD{
    int iType;
    int iCost;
    int iValue;
    char szName[16];
  }STRUCT_CARD, *PSTRUCT_CARD;

  typedef struct STRUCT_DECK{
    STRUCT_CARD aDraw[MAX_DECK];
    STRUCT_CARD aDiscard[MAX_DECK];
    STRUCT_CARD aHand[MAX_HAND];
    int iDrawCount;
    int iDiscardCount;
    int iHandCount;
  }STRUCT_DECK, *PSTRUCT_DECK;

  int iDrawMultipleCard(int iCardCt, PSTRUCT_DECK pstDeck);
  int iDrawCard(PSTRUCT_DECK pstDeck);
  int  iUpgradeFirstCardByName(PSTRUCT_DECK pstDeck, const char *kpszName, int iDeltaValue, int iDeltaCost);
  void vSwapCards(PSTRUCT_CARD pstA, PSTRUCT_CARD pstB);
  void vShuffle(PSTRUCT_CARD paCards, int iCount);
  void vInitBasicDeck(PSTRUCT_DECK pstDeck);
  void vDiscardHand(PSTRUCT_DECK pstDeck);
  void vShowDeck(PSTRUCT_DECK pstDeck);
  void vSortHandByName(PSTRUCT_DECK pstDeck);
  void vDiscardCard(PSTRUCT_DECK pstDeck, int iCardIx);
  void vAddCardToDiscard(PSTRUCT_DECK pstDeck, STRUCT_CARD stCard);
  void vAddDiscardPile2Deck(PSTRUCT_DECK pstDeck);
  void vLogDeck(PSTRUCT_DECK pstDeck, int iTraceLevel);
  STRUCT_CARD stMakeCard(int iType, const char *pszName, int iCost, int iValue);
  
  #define TRACE_DRAW_PILE    0x01
  #define TRACE_HAND         0x02
  #define TRACE_DISCARD_PILE 0x04
  #define TRACE_DECK_ALL     TRACE_DRAW_PILE | TRACE_HAND | TRACE_DISCARD_PILE

#endif