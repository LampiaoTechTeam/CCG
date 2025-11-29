#include <debuff.h>
#include <deck.h>
#include <monster.h>
#include <player.h>
#include <hud.h>
#include <dialog.h>
#include <input.h>
#include <trace.h>
#include <console_api.h>
#ifdef USE_SDL2
  #include <sdl_api.h>
#endif
#include <card_game.h>

/** === Globals === */
char *gkpszProgramName;
int gbLogLevel = 1;
int giLevel;
int gbSDL_Mode;
STRUCT_PLAYER gstPlayer;
#ifdef USE_SDL2
  STRUCT_HUD_LIST gstHudList;
#endif
/** === Procedures === */
void vFreeProgramName(){
  if (gkpszProgramName != NULL)
    free(gkpszProgramName);
}
void vSetProgramName(char *argv[]){
  char szPath[_MAX_PATH];
  char szName[_MAX_PATH];
  char szExt [_MAX_PATH];

  iDIR_SplitFilename(argv[0], szPath, szName, szExt);

  if ( !bStrIsEmpty(szName) ) {
    gkpszProgramName = (char *)malloc(strlen(szName) + 1);
    memset(gkpszProgramName, 0, strlen(szName) + 1);
    strcpy(gkpszProgramName, szName);
  }
}
void vInitGlobals(){
  giLevel = 1;
  giDebugLevel = DEBUG_LVL_DETAILS;
  gbSDL_Mode = FALSE;
  gkpszProgramName = NULL;
}

void vParseCmdlineArgs(int argc, char *argv[]){
  char *pTok;

  vSetProgramName(argv);
  
  if (argc <= 1)
    return ;

  if (bStrIsEmpty(argv[1]) || strstr(argv[1], "--") == NULL)
    return ;

  pTok = strtok(argv[1], "--");

  if (!memcmp(pTok, "sdl", 3))
    gbSDL_Mode = TRUE;

  if (bStrIsEmpty(argv[2]) || strstr(argv[2], "--") == NULL)
    return ;
  
  pTok = strtok(argv[2], "--");

  if (!memcmp(pTok, "debug=", 6)){
    pTok += 6;
    pTok = strtok(argv[2]," \n\r\t");
    if ( bStrIsNumeric(pTok) )
      giDebugLevel = atoi(pTok);
  }
  
}
/**
 * 
 *  Main 
 * 
 *  CCG_Main is a Macro defined to use SDL_Main when 
 *  USE_SDL2 is defined.
 * 
 *  Otherwise(console mode) uses traditional main()
 * 
 */
int CCG_Main(int argc, char *argv[]){
  STRUCT_DECK stDeck;
  STRUCT_MONSTER astMonsters[MAX_MONSTERS];
  int bRunning = TRUE;
  int iMonsterCount;
#ifdef USE_SDL2
  SDL_Window *pSDL_Wndw = NULL;
  SDL_Renderer *pSDL_Rnd = NULL;
  SDL_Event SDL_Ev;
#endif

  vInitGlobals();

  /** Reading cmdline options like using SDL2 or not, debug level ... */
  vParseCmdlineArgs(argc, argv);

  vInitLogs();
     
  #ifdef USE_SDL2
    if ( gbSDL_Mode )      
      vSDL_SetupMain(&pSDL_Rnd, &pSDL_Wndw);
    else
      vShowInitDialog();
  #else
    vShowInitDialog();
  #endif

  vInitBasicDeck(&stDeck);
  iDrawMultipleCard(INIT_HAND_CARDS, &stDeck);
  vInitPlayer(&stDeck, !gbSDL_Mode);
  vInitMonstersForLevel(astMonsters, giLevel, &iMonsterCount);
  vInitDialog();
 
  #ifdef FAKE
    vFakeOpenShopEarly(&stDeck);
  #endif

  vTraceMainLoopInit(); 

  #ifdef USE_SDL2
    if ( gbSDL_Mode )
      vSDL_MainLoop(&bRunning, &SDL_Ev, pSDL_Rnd, &stDeck, astMonsters, iMonsterCount);
    else 
      vCNSL_MainLoop(&bRunning, &stDeck, astMonsters, iMonsterCount);
  #else
    vCNSL_MainLoop(&bRunning, &stDeck, astMonsters, iMonsterCount);
  #endif

  vTraceMainLoopEnd();
  vFreeDialog();
  vFreeProgramName();
  
  #ifdef USE_SDL2
    if ( gbSDL_Mode ) 
      vSDL_MainQuit();
  #endif

  return 0;
}