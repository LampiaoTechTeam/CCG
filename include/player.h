/**
 * player.h 
 * 
 * Header include for player.c 
 * Prototypes and defines pragma_once "ansi like".
 * 
 * Written by Renato Fermi<repiazza@gmail.com> in September 2025
 * Maintained by Gustavo <> since September 2025
 * 
 */

#ifndef PLAYER_H
  #define PLAYER_H

  // #include <card_game.h>

  #define PLAYER_GET_NAME_NONE 0
  #define PLAYER_GET_NAME      1


  #define PLAYER_HP_MAX       100
  #define PLAYER_NAME_SIZE    64
  #define PLAYER_ENERGY_MAX   3
  #define PLAYER_INIT_GOLD    30
  #define PLAYER_MAX_DEBUFFS  10

  typedef struct STRUCT_PLAYER{
    int  iHP;
    int  iBlock;
    int  iEnergy;
    int  iGold;
    int  iDebuffCt;
    char szPlayerName[PLAYER_NAME_SIZE];
    STRUCT_DEBUFF stDebuff[PLAYER_MAX_DEBUFFS];
    STRUCT_DECK  astPlayerCards[MAX_DECK];
  }STRUCT_PLAYER, *PSTRUCT_PLAYER;

  extern STRUCT_PLAYER gstPlayer;
#ifdef USE_SDL2
  extern SDL_Rect gSDL_Player_Rect;
#endif
  int  iDoPlayerTurn(int *bRunning, PSTRUCT_DECK pstDeck, PSTRUCT_MONSTER pastMonster, int iMonsterCt);
  void vInitPlayer(PSTRUCT_DECK pstGameDeck, int bReadName);
  void vShowPlayer();
  void vStartNewTurn(PSTRUCT_DECK pstDeck);
  void vTracePlayer(PSTRUCT_PLAYER pstPlayer);
#endif
