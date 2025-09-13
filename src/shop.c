#include <stdio.h>
#include <string.h>
#include <shop.h>
#include <terminal_utils.h>
#include <input.h>
#include <trace.h>
#include <card_game.h>

static void vPrintShopHeader(void) {
  char sz[128];
  vClearTerminal();
  vPrintHighlitedLine("===== SHOP =====", INSERT_NEW_LINE);
  snprintf(sz, sizeof(sz), "Ouro disponivel: %d", gstPlayer.iGold);
  vPrintLine(sz, INSERT_NEW_LINE);
  vPrintLine("1) Upgrade de uma carta (+2 valor, -0 custo) [20 ouro]", INSERT_NEW_LINE);
  vPrintLine("2) Comprar carta nova: Poison (custo:1, aplica 3 veneno) [15 ouro]", INSERT_NEW_LINE);
  vPrintLine("q) Sair", INSERT_NEW_LINE);
}

void vAddPlayerReward(PSTRUCT_PLAYER pstPlayer) {
  double dGoldRewarded = SHOP_STD_GOLD_AMOUNT_REWARD + (SHOP_GOLD_MULTIPLIYER * giLevel);
  pstPlayer->iGold += (int) dGoldRewarded;

  {
    char szMsg[128];

    snprintf(szMsg, sizeof(szMsg),
  "\n*** Voce recebeu %d de gold para o nivel %d! ***", 
      (int) dGoldRewarded , 
      giLevel
    );
    vPrintHighlitedLine(szMsg, INSERT_NEW_LINE);
    vSleepSeconds(5);
  }
}

void vOpenShop(PSTRUCT_DECK pstDeck)
{
  int bLoop = TRUE;

  while (bLoop) {
    int iCh;
    vPrintShopHeader();
    vPrintLine("\nEscolha uma opcao: ", NO_NEW_LINE);
    iCh = iPortableGetchar();

    if (iCh == 'q') break;
    
    if (iCh == '1') {
      char szWhich[_MAX_PATH];
      
      if (gstPlayer.iGold < SHOP_PRICE_UPGRADE) {
        vPrintLine("\nOuro insuficiente!", INSERT_NEW_LINE);
        vSleepSeconds(1);
        continue;
      }

      do {
        vClearTerminal();
        vPrintLine("\nDigite o nome exato da carta para upgrade (ex.: Strike, Defend, Heal, Fireball):", INSERT_NEW_LINE);
        vPrintLine("> ", NO_NEW_LINE);
        vReadCardName(szWhich, sizeof(szWhich));
      } while ( bStrIsEmpty(szWhich) );

      if ( iUpgradeFirstCardByName(pstDeck, szWhich, 2, 0) ) {
        gstPlayer.iGold -= SHOP_PRICE_UPGRADE;
        vTraceVarArgsFn("Shop: upgrade em '%s' aplicado. Ouro restante=%d", szWhich, gstPlayer.iGold);
        vPrintLine("Upgrade aplicado com sucesso!", INSERT_NEW_LINE);
      } else {
        vPrintLine("Carta nao encontrada em mao/draw/discard.", INSERT_NEW_LINE);
      }
      vSleepSeconds(1);
    } else if (iCh == '2') {
      STRUCT_CARD stPoison;
      if (gstPlayer.iGold < SHOP_PRICE_POISON) {
        vPrintLine("\nOuro insuficiente!", INSERT_NEW_LINE);
        vSleepSeconds(1);
        continue;
      }
      stPoison = stMakeCard(CARD_POISON, "Poison", 1, 3, CARD_TARGET_SINGLE);
      vAddCardToDiscard(pstDeck, stPoison);
      gstPlayer.iGold -= SHOP_PRICE_POISON;
      vTraceVarArgsFn("Shop: comprou Poison. Ouro restante=%d", gstPlayer.iGold);
      vPrintLine("Poison adicionada ao descarte!", INSERT_NEW_LINE);
      vSleepSeconds(1);
    } else {
      vPrintLine("\nOpcao invalida.", INSERT_NEW_LINE);
      vSleepSeconds(1);
    }
  }
}
