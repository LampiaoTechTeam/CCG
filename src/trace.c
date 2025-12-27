/**
 * trace.c
 *
 * Written by Renato Fermi <repiazza@gmail.com>
 *
 * Trace functions and global variables
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys_interface.h>
#include <card_game.h>

#include <trace.h>

#if defined(LINUX) || defined(APPLE)
  #include <sys/time.h>
  #include <sys/types.h>
  #include <stdarg.h>
#else
  #include <windows.h>
  #include <process.h>
#endif

char gszRootPathFromBin[_MAX_PATH];
char gszTraceFile[2048];
char gszTraceFileDialog[2048];
char gszDebugLevel[32];
char gszConfFile[_MAX_PATH];
int  giNoNL = FALSE;
int gbTraceOnTerminal = FALSE;

#ifdef _WIN32

int gettimeofday(struct timeval *tp, void *tzp) {
  FILETIME ft;
  ULARGE_INTEGER li;
  unsigned __int64 t;
  
  if ( tzp == NULL ){}

  GetSystemTimeAsFileTime(&ft);
  li.LowPart = ft.dwLowDateTime;
  li.HighPart = ft.dwHighDateTime;
  t = li.QuadPart - 116444736000000000ULL; // converte de 1601 para 1970
  tp->tv_sec = (long)(t / 10000000ULL);
  tp->tv_usec = (long)((t % 10000000ULL) / 10);
  return 0;
}
#endif

void vTraceMsgNoNL(char *szMsg) {
  giNoNL = TRUE;
  vTraceMsg(szMsg);
  giNoNL = FALSE;
} /* vTraceMsgNoNL */

void vTraceMsg(char *szMsg) {
  FILE *pfLog;
  char szDateTimeNow_us[128];
  struct tm *st_tm_Now;
  struct timeval tv;
  time_t lTime;

  time(&lTime);
  st_tm_Now = localtime(&lTime);
  gettimeofday(&tv, NULL);

  memset(szDateTimeNow_us, 0, sizeof(szDateTimeNow_us));

  snprintf(szDateTimeNow_us, sizeof(szDateTimeNow_us),
           "[%02d/%02d/%02d %02d:%02d:%02d.%03ld] ", (int)st_tm_Now->tm_mday,
           (int)st_tm_Now->tm_mon + 1, (int)st_tm_Now->tm_year + 1900,
           (int)st_tm_Now->tm_hour, (int)st_tm_Now->tm_min,
           (int)st_tm_Now->tm_sec, (long)tv.tv_usec / 1000);

  if ((pfLog = fopen(gszTraceFile, "a+")) == NULL){
    return;
  }
  if (giNoNL == TRUE) {
    fprintf(pfLog, "%s", szMsg);
    if ( gbTraceOnTerminal ) fprintf(stdout, "%s", szMsg);
  }
  else {
    fprintf(pfLog, "%s%s\n", szDateTimeNow_us, szMsg);
    if ( gbTraceOnTerminal ) fprintf(stdout, "%s%s\n", szDateTimeNow_us, szMsg);
  }

  fclose(pfLog);
  pfLog = NULL;
} /* vTraceMsg */

void _vTraceMsgDialog(char *szMsg, ...) {
  FILE *pfLog;
  va_list args;
  if ( DEBUG_DIALOG ){
    va_start(args, szMsg);
    if ((pfLog = fopen(gszTraceFileDialog, "a+")) == NULL){
      return;
    }
    vfprintf(pfLog, szMsg, args);
    if ( gbTraceOnTerminal ) vfprintf(stdout, szMsg, args);

    va_end(args);
    fclose(pfLog);
    pfLog = NULL;
  }
} /* vTraceMsg */

void vTracePid(char *szMsg, int iMsgLen) {
  char *pszMyMsg = NULL;
  int iNewMsgLen = iMsgLen + 16;
  int iPid;

  iPid = getpid();

  if ((pszMyMsg = (char *)malloc(iNewMsgLen)) == NULL)
    return;

  memset(pszMyMsg, 0x00, iNewMsgLen);
  sprintf(pszMyMsg, "%d %s", iPid, szMsg);

  vTraceMsg(pszMyMsg);

  free(pszMyMsg);
  pszMyMsg = NULL;
} /* vTracePid */

void vSetRootPathFromCwd(){
  char szTestPath[_MAX_PATH + _MAX_PATH];
  memset(szTestPath, 0, sizeof(szTestPath));
  memset(gszRootPathFromBin, 0, sizeof(gszRootPathFromBin));
  sprintf(gszRootPathFromBin, "%s", ROOT_PATH_FROM_BIN);
  sprintf(szTestPath,"%s/bin", gszRootPathFromBin);
  if ( !iDIR_IsDir(szTestPath) )
    sprintf(gszRootPathFromBin, "%s", ".");
    
}

void _vTraceVarArgsFn(char *pszModuleName, const int kiLine, const char *kpszFunctionName, const char *kpszFmt, ...) {
  va_list args;
  FILE *pfLog = NULL;
  char szPath[_MAX_PATH + _MAX_PATH + 8];
  char szName[_MAX_PATH];
  char szExt[_MAX_PATH];
  char szFullTitle[_MAX_PATH+16];
  char szDbg[2048];
  struct tm *st_tm_Now;
  struct timeval tv;
  time_t lTime;

  memset(szDbg, 0x00, sizeof(szDbg));
  memset(szPath, 0x00, sizeof(szPath));
  memset(szName, 0x00, sizeof(szName));
  memset(szExt, 0x00, sizeof(szExt));

  time(&lTime);
  st_tm_Now = localtime(&lTime);
  gettimeofday(&tv, NULL);

  if ((pfLog = fopen(gszTraceFile, "a+")) == NULL) {
    return;
  }

  va_start(args, kpszFmt);

  iDIR_SplitFilename(pszModuleName, szPath, szName, szExt);
  sprintf(szFullTitle, "<%.9s%s:%d>", szName, szExt, kiLine);
  snprintf(szDbg, sizeof(szDbg), "[%02d/%02d/%02d %02d:%02d:%02d.%03ld]%-16.16s(%-12.12s): ",
           (int)st_tm_Now->tm_mday, (int)st_tm_Now->tm_mon + 1,
           (int)st_tm_Now->tm_year - 100, (int)st_tm_Now->tm_hour,
           (int)st_tm_Now->tm_min, (int)st_tm_Now->tm_sec,
           (long)tv.tv_usec / 1000, szFullTitle, kpszFunctionName);

  strcat(szDbg, kpszFmt);
  
  strcat(szDbg, "\n");
  vfprintf(pfLog, szDbg, args);
  if ( gbTraceOnTerminal ) vfprintf(stdout, szDbg, args);

  va_end(args);

  fclose(pfLog);
  pfLog = NULL;
} /* _vTraceVarArgsFn */


void vSetLogFileTitle(void) {
  memset(gszTraceFile, 0, sizeof(gszTraceFile));
  sprintf(gszTraceFile, "%s.log", gkpszProgramName);
  memset(gszTraceFileDialog, 0, sizeof(gszTraceFileDialog));
  sprintf(gszTraceFileDialog, "%s_dialog.log", gkpszProgramName);
} /* vSetLogFile */

void vInitLogs(char* pszTrace, const char* pszDebugLevel) {
  char szPath[_MAX_PATH + 8];
  char szPath2[_MAX_PATH + 8];
  char szName[_MAX_PATH];
  char szExt[_MAX_PATH];

  memset(szPath, 0x00, sizeof(szPath));
  memset(szName, 0x00, sizeof(szName));
  memset(szExt, 0x00, sizeof(szExt));

  
  
  vSetRootPathFromCwd();
  if ( bStrIsEmpty(pszTrace) ) {
    vSetLogFileTitle();
    iDIR_SplitFilename(gszTraceFile, szPath, szName, szExt);
    snprintf(szPath, sizeof(szPath), "%s/log", gszRootPathFromBin);
    sprintf(gszTraceFile, "%s/%s%s",szPath,szName,szExt);
  }
  else {
    iDIR_SplitFilename(pszTrace, szPath, szName, szExt);
    snprintf(gszTraceFile, sizeof(gszTraceFile), "%s", pszTrace);
  }
  if ( !iDIR_IsDir(szPath) ) {
    if ( !iDIR_MkDir(szPath) ) {
      sprintf(gszRootPathFromBin, "%s", ROOT_PATH_FROM_BIN);
      snprintf(szPath, sizeof(szPath), "%s/log", gszRootPathFromBin);
      sprintf(gszTraceFile, "%s/%s%s",szPath,szName,szExt);
      
      if ( !iDIR_IsDir(szPath) ) {
        if ( !iDIR_MkDir(szPath) ) {
          fprintf(stderr, "E: Impossible create dir [%s]!\n"
          "%s\n",
          szPath, strerror(errno));
          exit(EXIT_FAILURE);
        }
      }
    }
  }
  if ( pszDebugLevel ) snprintf(gszDebugLevel, sizeof(gszDebugLevel), "%s", pszDebugLevel);

  iDIR_SplitFilename(gszTraceFileDialog, szPath2, szName, szExt);
  sprintf(gszTraceFileDialog, "%s/%s%s",szPath,szName,szExt);
} /* vInitLogs */

void vTraceMainLoopInit(){
  vTraceVarArgsFn("===========================================");
  vTraceVarArgsFn("=====    ***   Init Main LOOP  ***    =====");
  vTraceVarArgsFn("=====    ***     Mode=%s       ***    =====", gbSDL_Mode ? "SDL" : "CONSOLE");
  vTraceVarArgsFn("=====                                 =====");
}

void vTraceMainLoopEnd(){
  vTraceVarArgsFn("=====                                 =====");
  vTraceVarArgsFn("=====    ***   End Main LOOP   ***    =====");
  vTraceVarArgsFn("=====    ***     Mode=%s       ***    =====", gbSDL_Mode ? "SDL" : "CONSOLE");
  vTraceVarArgsFn("===========================================");
}
