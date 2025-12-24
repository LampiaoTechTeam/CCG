#ifndef FDUMMIES_H
  #define FDUMMIES_H  

  #include <deck.h>
  #include <player.h>
  // #include <card_game.h>

  void vFakeOpenShopEarly(PSTRUCT_DECK pstDeck){
      PSTRUCT_CARD pstCard = pstDeck->astHand;

      gstPlayer.iGold += 100;
      vOpenShop(pstDeck);
      vDiscardHand(pstDeck);
      iDrawMultipleCard(INIT_HAND_CARDS, pstDeck);
      vShuffle(pstCard, pstDeck->iHandCount);
  }
#endif
