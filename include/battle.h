#include <deck.h>
#include <monster.h>

#ifndef BATTLE_H
    #define BATTLE_H
    
    void vPlayCard(int iCardIndex, PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER paMonsters, int iMonsterCount);
    void vShowTable(PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonsters, int iMonsterCount);
#endif