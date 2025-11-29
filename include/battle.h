#ifndef BATTLE_H
    #define BATTLE_H
    #include <card_game.h>
    
    void vPlayCard(int *iCardIndex, PSTRUCT_DECK pstDeck);
    void vShowTable(PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCount);
    int bHasAnyPlayableCard(PSTRUCT_DECK pstDeck);
    int iInitCurrentLevel(int iCurrLevel, PSTRUCT_MONSTER pastMonsters, int *iMonsterCount);
    int iHandlePlayerActionByCard(PSTRUCT_CARD pstCard, PSTRUCT_MONSTER pastMonsters, int iMonsterCt);
#endif