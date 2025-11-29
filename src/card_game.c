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

int bShowVersion = FALSE;
int bShowHelp = FALSE;

STRUCT_GAME gstGame;

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
  gstGame.iLevel = giLevel;
  gstGame.iStatus = STATUS_RUN;
  gstGame.iState = 0;
}

static void vShowVersion(void) {
  printf(
    "%s %s %s [%s %s]\n",
    gkpszProgramName,
    CCG_VERSION,
    COPYRIGTH,
    __DATE__,
    __TIME__
  );
#ifdef BUILD_OS_NAME
  printf(
    "%s %s %s %s\n"
    "build with %s %s\n",
    BUILD_OS_NAME,
    BUILD_OS_VERSION,
    BUILD_OS_ARCH,
    BUILD_HOSTNAME,
    BUILD_COMPILER,
    BUILD_COMPILER_VERSION
  );
#endif
}

static void vShowHelp(void) {
  printf(
    "Usage: %s --argument=<parameter>\n"
    "  -h, --help\n"
    "    Show the version and exit.\n"
    "  -v, --version\n"
    "    Show the version and exit.\n"
    "  --debug=<number>\n"
    "    Set the debug level\n"
    "  --sdl\n"
    "    Start program in GUI mode\n",
    gkpszProgramName
  );
}

void vParseCmdlineArgs(int argc, char *argv[]){
  char *pTok = NULL;
  int bLongArgument = FALSE;
  int bShortArgument = FALSE;

  vSetProgramName(argv);
  
  if (argc <= 1)
    return ;

  if (bStrIsEmpty(argv[1]) ) {
    return;
  }

  if ( strstr(argv[1], "-") && strstr(argv[1], "--") == NULL ) {
    bShortArgument = TRUE;
    pTok = strtok(argv[1], "-");
  }
  else {
    if ( strstr(argv[1], "--") ) {
      bLongArgument = TRUE;
      pTok = strtok(argv[1], "--");
    }
  }

  if ( bShortArgument ) {
    if ( !memcmp(pTok, "h", 1) ) {
      bShowHelp = TRUE;
      return;
    }

    if ( !memcmp(pTok, "v", 1) ) {
      bShowVersion = TRUE;
      return;
    }
  }

  if ( bLongArgument ) {
    if ( !memcmp(pTok, "help", 4) ) {
      bShowHelp = TRUE;
      return;
    }

    if ( !memcmp(pTok, "version", 7) ) {
      bShowVersion = TRUE;
      return;
    }

    if (!memcmp(pTok, "sdl", 3))
      gbSDL_Mode = TRUE;
  }

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

/** ===  Main  === **/
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

  vParseCmdlineArgs(argc, argv);

  if ( bShowHelp ) {
    vShowHelp();
    return 0;
  }

  if ( bShowVersion ) {
    vShowVersion();
    return 0;
  }

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
