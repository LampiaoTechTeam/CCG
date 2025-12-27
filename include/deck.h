#ifndef DECK_H
  #define DECK_H

  #define MAX_DECK             50
  #define INIT_HAND_CARDS       6
  #define MAX_HAND             INIT_HAND_CARDS /** TODO: Global enhancement by dngn lvl */

  #define CARD_TYPE_NONE        0
  #define CARD_TYPE_OFFENSIVE   1
  #define CARD_TYPE_DEFFENSIVE  2
  #define CARD_TYPE_DEBUFF      3
  #define CARD_TYPE_SUPPORT     4
 
  #define CARD_TARGET_SINGLE    1
  #define CARD_TARGET_MULTIPLE  2
  #define CARD_TARGET_SELF      3

  #define CARD_NONE             0
  #define CARD_STRIKE           1
  #define CARD_DEFEND           2
  #define CARD_HEAL             3
  #define CARD_FIREBALL         4
  #define CARD_POISON           5
  #define CARD_PARALIZE         6
  #define CARD_CURSE_BLOCK      7
  #define CARD_NULL            -1
  
  #define CARD_MAX_COST        10

  #define DEBUFF_POISON_CYCS    3
  #define DEBUFF_PARALISE_CYCS  3

  #define TARGET_ALL           99

  #define TRACE_DRAW_PILE    0x01
  #define TRACE_HAND         0x02
  #define TRACE_DISCARD_PILE 0x04

  /**  @brief 
   * TRACE_DECK_ALL ->
   *    TRACE_DRAW_PILE 0x01 = 0001
   *    | TRACE_HAND    0x02 = 0010    
   *    | TRACE_DISCARD_PILE = 0100
   * 
   *  TRACE DECK ALL -> 0x07 == 0111
   * */
  #define TRACE_DECK_ALL     TRACE_DRAW_PILE | TRACE_HAND | TRACE_DISCARD_PILE

  /**
   * @brief
   *   TEST_TRACE_OPT(OPT, TRACE_MASK) ->
   *   Tests OPT for specific binary mask
   *   In other words if OPT contains mask returns true
   * 
   */
  #define TEST_TRACE_OPT(OPT, TRACE_MASK) (OPT & TRACE_MASK)

  typedef struct STRUCT_CARD{
    int  iClass;
    int  iType;
    int  iTarget;
    int  iCost;
    int  iValue;
    char szName[16];
  } STRUCT_CARD, *PSTRUCT_CARD;

  typedef struct STRUCT_DECK{
    STRUCT_CARD astDraw   [MAX_DECK];
    STRUCT_CARD astDiscard[MAX_DECK];
    STRUCT_CARD astHand   [MAX_HAND];
    int         iDrawCount;
    int         iDiscardCount;
    int         iHandCount;
  }STRUCT_DECK, *PSTRUCT_DECK;

  extern char *pszDebuffTypeDesc[];
  extern char *pszCardTypeDesc[];

  int iDrawMultipleCard(int iCardCt, PSTRUCT_DECK pstDeck);
  int iDrawCard(PSTRUCT_DECK pstDeck);
  int  iUpgradeFirstCardByName(PSTRUCT_DECK pstDeck, const char *kpszName, int iDeltaValue, int iDeltaCost);
  int iGetMinimumEnergy(PSTRUCT_DECK pstDeck);
  void vSwapCards(PSTRUCT_CARD pstA, PSTRUCT_CARD pstB);
  void vShuffle(PSTRUCT_CARD paCards, int iCount);
  void vInitBasicDeck(PSTRUCT_DECK pstDeck);
  void vDiscardHand(PSTRUCT_DECK pstDeck);
  void vShowDeck(PSTRUCT_DECK pstDeck);
  void vDiscardDraw(PSTRUCT_DECK pstDeck);
  void vSortHandByName(PSTRUCT_DECK pstDeck);
  void vDiscardCard(PSTRUCT_DECK pstDeck, int iCardIx);
  void vAddCardToDiscard(PSTRUCT_DECK pstDeck, STRUCT_CARD stCard);
  void vAddDiscardPile2Deck(PSTRUCT_DECK pstDeck);
  void vTraceDeck(PSTRUCT_DECK pstDeck, int iTraceLevel);
  
  STRUCT_CARD stMakeCard(int iType, const char *pszName, int iCost, int iValue, int iTarget);
#endif
