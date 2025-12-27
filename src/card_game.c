#include <stdio.h>
#include <debuff.h>
#include <deck.h>
#include <monster.h>
#ifdef USE_SDL2
  #ifdef APPLE
    #define SDL_MAIN_HANDLED
  #endif
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_ttf.h>
#endif
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
#include <game.h>
#include <welcome.h>
#include <consts.h>
#include <conf.h>
#include <screen.h>
#include <getopt.h>
#include <cmdline.h>
#include <xml.h>

/** === Globals === */
char *gkpszProgramName;
int gbLogLevel = 1;
int giLevel;
int gbSDL_Mode;
STRUCT_PLAYER gstPlayer;

int bShowVersion = FALSE;
int bShowHelp = FALSE;

STRUCT_GAME gstGame;

STRUCT_GLOBAL_PRM gstGlobalPrm;

typedef struct STRUCT_CMDLINE {
  char szTrace[_MAX_PATH];
  char szDebugLevel[32];
  char szConfDir[_MAX_PATH];
} STRUCT_CMDLINE, *PSTRUCT_CMDLINE;

STRUCT_CMDLINE gstCmdLine;

char* gpszOptStr = "hvd:s";
/*
   Long Short has_arg pDataType exemple
    bSet default pData iDataSize
   Help
*/
STRUCT_COMMANDLINE_OPTIONS astCmdOpt[] = {
  /* 00 */
  { "help", 'h', no_argument, CMDLINETYPE_NULL, "",
     FALSE,  "",  NULL,        0,
    "Show this message and exit"
  },
  /* 01 */
  { "version", 'v', no_argument, CMDLINETYPE_NULL, "",
     FALSE,     "",  NULL,        0,
    "Show the version and exit"
  },
  /* 02 */
  { "trace", 't',    required_argument, CMDLINETYPE_STRING, "<number>",
    FALSE,   "", gstCmdLine.szTrace,     sizeof(gstCmdLine.szTrace),
    "<number> Set the trace file path"
  },
  /* 03 */
  { "debug", 'd',    required_argument, CMDLINETYPE_STRING, "<number>",
     FALSE,   "", gstCmdLine.szDebugLevel,     sizeof(gstCmdLine.szDebugLevel),
    "<number> Set the debug level (disable by default)"
  },
  /* 04 */
  { "sdl",   's', no_argument, CMDLINETYPE_NULL, "",
     FALSE,   "",  NULL,        0,
    "Start program in GUI mode (disable by default)"
  },
  /* 05 */
  { "conf-dir",   'c', required_argument,    CMDLINETYPE_NULL, "",
    FALSE,         "",  gstCmdLine.szConfDir, sizeof(gstCmdLine.szConfDir),
    "Set the path of conf directory"
  },
  { NULL, 0, no_argument, CMDLINETYPE_NULL, NULL,
     FALSE, NULL, NULL, 0,
    NULL
  }
};

int icbackCCGXml(xmlNodePtr pstNode, void* pData __attribute__((unused)));
STRUCT_XML astCCGXml[] = {
  { "CCG"        , XMLTYPE_PROC  , 0                                , NULL                     , icbackCCGXml },
  { "TRACE"      , XMLTYPE_STRING, sizeof(gstGlobalPrm.szTrace)     , gstGlobalPrm.szTrace     , NULL            },
  { "DEBUG_LEVEL", XMLTYPE_STRING, sizeof(gstGlobalPrm.szDebugLevel), gstGlobalPrm.szDebugLevel, NULL            },
  { "WRK_DIR"    , XMLTYPE_STRING, sizeof(gstGlobalPrm.szWrkDir)    , gstGlobalPrm.szWrkDir    , NULL            },
  { "FONT_DIR"   , XMLTYPE_STRING, sizeof(gstGlobalPrm.szFontsDir)  , gstGlobalPrm.szFontsDir  , NULL            },
  { NULL         , XMLTYPE_NULL  , 0                                , NULL                     , NULL            }
};
int icbackCCGXml(xmlNodePtr pstNode, void* pData __attribute__((unused))) {
  if ( !strcasecmp((char*)pstNode->name, "CCG") ) {
    memset(&gstGlobalPrm, 0x00, sizeof(gstGlobalPrm));
    iParseXmlFields(pstNode, astCCGXml);
  }
  return 1;
}

STRUCT_CONF_FILE astConfFile[] = {
  { "trace"      , gstGlobalPrm.szTrace     , sizeof(gstGlobalPrm.szTrace)     , DATATYPE_STRING, "log/card_game.log" },
  { "debug-level", gstGlobalPrm.szDebugLevel, sizeof(gstGlobalPrm.szDebugLevel), DATATYPE_STRING, "9"                 },
  { "wrk-dir"    , gstGlobalPrm.szWrkDir    , sizeof(gstGlobalPrm.szWrkDir)    , DATATYPE_STRING, "./"                },
  { "fonts-dir"  , gstGlobalPrm.szFontsDir  , sizeof(gstGlobalPrm.szFontsDir)  , DATATYPE_STRING, "./fonts"           },
  { "conf-dir"   , gstGlobalPrm.szConfDir   , sizeof(gstGlobalPrm.szConfDir)   , DATATYPE_STRING, "./conf"            },
  { NULL         , NULL                     , 0                                , 0              , NULL                }
};

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

int bInitGlobals(void) {
  char szConfFile[512] = "";
  char szGameDatPath[512] = "";

  memset(szConfFile   , 0x00, sizeof(szConfFile));
  memset(szGameDatPath, 0x00, sizeof(szGameDatPath));

  snprintf(szConfFile   , sizeof(szConfFile), "%s%cccg.xml", gstGlobalPrm.szConfDir, DIR_SEPARATOR);

  giLevel = 1;
  gstGame.iLevel = giLevel;
  gstGame.iStatus = STATUS_WELCOME;
  gstGame.iLastState = STATE_NONE;
  gstGame.iState = STATE_WELCOME_BEGIN;

  snprintf(gszDebugLevel, sizeof(gszDebugLevel), "%c", DEBUG_LVL_DETAILS);
  gbSDL_Mode = FALSE;
  gkpszProgramName = NULL;

  if ( !bLoadXmlFromFile(szConfFile, astCCGXml) ) {
    snprintf(szConfFile   , sizeof(szConfFile), "%s%cccg.conf", gstCmdLine.szConfDir, DIR_SEPARATOR);
    if ( !bReadConfFile(szConfFile, astConfFile) ) {
      fprintf(stderr, "Falha ao carregar parametros globais!\n");
      return 0;
    }
  }

  if ( !bStrIsEmpty(gstCmdLine.szTrace) ) {
    snprintf(gstGlobalPrm.szTrace, sizeof(gstGlobalPrm.szTrace), "%s", gstCmdLine.szTrace);
  }

  if ( !bStrIsEmpty(gstCmdLine.szDebugLevel) ) {
    snprintf(gstGlobalPrm.szDebugLevel, sizeof(gstGlobalPrm.szDebugLevel), "%s", gstCmdLine.szDebugLevel);
  }

  snprintf(szGameDatPath, sizeof(szGameDatPath), "%s%cGAME.dat", gstGlobalPrm.szWrkDir, DIR_SEPARATOR);
  snprintf(gszDebugLevel, sizeof(gszDebugLevel), "%s", gstGlobalPrm.szDebugLevel);

  if ( iDIR_IsDir(szGameDatPath) == 0 ) {
    iGameLoad();
  }

  return 1;
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

int iSDL_OpenShop(SDL_Renderer *pSDL_Renderer, PSTRUCT_PLAYER pstPlayer, PSTRUCT_DECK pstDeck);

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
  char* pszRootDir = NULL;

  memset(&gstGlobalPrm, 0x00, sizeof(gstGlobalPrm));
  memset(&gstGame     , 0x00, sizeof(gstGame     ));

  vSetProgramName(argv);

  pszRootDir = getenv("CCG_ROOT_DIR");

  if ( bStrIsEmpty(pszRootDir) ) {
    pszRootDir = ".";
  }

  snprintf(gstCmdLine.szConfDir, sizeof(gstCmdLine.szConfDir), "%s%cconf", pszRootDir, DIR_SEPARATOR);

  /** Reading cmdline options like using SDL2 or not, debug level ... */
  if ( !bParseCommandLine(argc, argv, astCmdOpt) ) {
    vPrintUsage(argv, astCmdOpt);
    return -1;
  }

  if ( !bStrIsEmpty(gstCmdLine.szConfDir) ) {
    snprintf(gstGlobalPrm.szConfDir, sizeof(gstGlobalPrm.szConfDir), "%s", gstCmdLine.szConfDir);
  }

  if ( !bInitGlobals() ) {
    return -1;
  }

  if ( astCmdOpt[0].bSet ) {
    vPrintUsage(argv, astCmdOpt);
    return 0;
  }

  if ( astCmdOpt[1].bSet ) {
    vShowVersion();
    return 0;
  }

  if ( astCmdOpt[4].bSet ) {
    gbSDL_Mode = 1;
  }

  vInitLogs(gstGlobalPrm.szTrace, gstGlobalPrm.szDebugLevel);
     
  #ifdef USE_SDL2
    if ( gbSDL_Mode ) {
      char szScreenXmlPath[512] = "";
      memset(szScreenXmlPath, 0x00, sizeof(szScreenXmlPath));
      snprintf(szScreenXmlPath, sizeof(szScreenXmlPath), "%s%cscreen.xml", gstGlobalPrm.szConfDir, DIR_SEPARATOR);
      if ( !bLoadScreenXml(szScreenXmlPath) ) {
        fprintf(stderr, "Falha ao carregar o arquivo %s!", szScreenXmlPath);
        return -1;
      }
      vSDL_SetupMain(&pSDL_Rnd, &pSDL_Wndw);
    }
    else
      vShowInitDialog();
  #else
    vShowInitDialog();
  #endif

  #ifdef USE_SDL2
    vSDL_WelcomeInit();
    if ( iSDL_OpenWelcome(pSDL_Rnd) == FINISH_PROGRAM ) {
      if ( gbSDL_Mode ) vSDL_MainQuit();
      return 0;
    }
  #endif

  if ( !gbLoadGameFromFile ) {
    vInitBasicDeck(&stDeck);
    iDrawMultipleCard(INIT_HAND_CARDS, &stDeck);
    vInitPlayer(&stDeck, !gbSDL_Mode);
    vInitMonstersForLevel(astMonsters, giLevel, &iMonsterCount);
    vInitDialog();
  }
  else {
    vTraceVarArgsFn("memcpy(&stDeck, gstGame.stGameContext.stPlayer.astPlayerCards, sizeof(STRUCT_DECK));");
    memcpy(&stDeck, gstGame.stGameContext.stPlayer.astPlayerCards, sizeof(STRUCT_DECK));
    vTraceVarArgsFn("memcpy(&gstPlayer, &gstGame.stGameContext.stPlayer, sizeof(STRUCT_PLAYER));");
    memcpy(&gstPlayer, &gstGame.stGameContext.stPlayer, sizeof(STRUCT_PLAYER));
    vTraceVarArgsFn("memcpy(&astMonsters, &gstGame.stGameContext.astMonster, sizeof(STRUCT_MONSTER) * gstGame.stGameContext.iCtMonster);");
    memcpy(&astMonsters, &gstGame.stGameContext.astMonster, sizeof(STRUCT_MONSTER) * gstGame.stGameContext.iCtMonster);
    vTraceVarArgsFn("iMonsterCount = gstGame.stGameContext.iCtMonster;");
    iMonsterCount = gstGame.stGameContext.iCtMonster;
    vTraceVarArgsFn("FINISH");
  }
 
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
