/**
 * trace.h
 * 
 * Written by Renato Fermi <repiazza@gmail.com>
 *
 * Trace functions and global variables
 * 
 */

#ifndef _TRACE_H_
  #define _TRACE_H_

/******************************************************************************
 *                                                                            *
 *                                 Includes                                   *
 *                                                                            *
 ******************************************************************************/ 
  #include <errno.h>
  #include <sys_interface.h>

  #ifdef _WIN32
    /* Winsock2 deve vir antes de windows.h e antes de time.h */
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #include <process.h>
    #include <time.h>
  #else
    #include <unistd.h>
    #include <sys/time.h>
    #include <time.h>
  #endif
  
/******************************************************************************
 *                                                                            *
 *                             Defines and macros                             *
 *                                                                            *
 ******************************************************************************/

  #define DEBUG_LVL_NONE         '0'
  #define DEBUG_LVL_FNCALL       '1'
  #define DEBUG_LVL_SYSCALL      '4'
  #define DEBUG_LVL_DETAILS      '7'
  #define DEBUG_LVL_MORE_DETAILS '8'
  #define DEBUG_LVL_ALL          '9'

  #define TOKEN_MISMATCH -1
  #define ROOT_PATH_FROM_BIN ".."
  #define UNUSED(X) (void) X

  #define DEBUG_MSGS      gszDebugLevel[0] >=  DEBUG_LVL_FNCALL
  #define DEBUG_MORE_MSGS gszDebugLevel[0] >=  DEBUG_LVL_DETAILS
  #define DEBUG_DIALOG    gszDebugLevel[0] >=  DEBUG_LVL_MORE_DETAILS
  #define DEBUG_ALL       gszDebugLevel[0] >=  DEBUG_LVL_ALL

  #define DEBUG_SDL_MSGS      gszDebugLevel[1] >=  DEBUG_LVL_FNCALL
  #define DEBUG_SDL_MORE_MSGS gszDebugLevel[1] >=  DEBUG_LVL_DETAILS
  #define DEBUG_SDL_DIALOG    gszDebugLevel[1] >=  DEBUG_LVL_MORE_DETAILS
  #define DEBUG_SDL_ALL       gszDebugLevel[1] >=  DEBUG_LVL_ALL

  #define DEBUG_XML_MSGS      gszDebugLevel[2] >=  DEBUG_LVL_FNCALL
  #define DEBUG_XML_MORE_MSGS gszDebugLevel[2] >=  DEBUG_LVL_DETAILS
  #define DEBUG_XML_DIALOG    gszDebugLevel[2] >=  DEBUG_LVL_MORE_DETAILS
  #define DEBUG_XML_ALL       gszDebugLevel[2] >=  DEBUG_LVL_ALL

  #define DEBUG_SCREEN_MSGS      gszDebugLevel[3] >=  DEBUG_LVL_FNCALL
  #define DEBUG_SCREEN_MORE_MSGS gszDebugLevel[3] >=  DEBUG_LVL_DETAILS
  #define DEBUG_SCREEN_DIALOG    gszDebugLevel[3] >=  DEBUG_LVL_MORE_DETAILS
  #define DEBUG_SCREEN_ALL       gszDebugLevel[3] >=  DEBUG_LVL_ALL

  #define vTraceVarArgsFn(FORMAT, ...) _vTraceVarArgsFn(__FILE__, __LINE__, __func__, FORMAT, ##__VA_ARGS__)
  #define vTraceMsgDialog(FORMAT, ...) _vTraceMsgDialog(FORMAT, ##__VA_ARGS__)
  #define vTraceBegin() vTraceVarArgsFn(" -- Begin -- ")
  #define vTraceEnd()   vTraceVarArgsFn(" --  End  -- ")

/******************************************************************************
 *                                                                            *
 *                     Global variables and constants                         *
 *                                                                            *
 ******************************************************************************/

  extern char gszDebugLevel[32];
  extern char gszTraceFile[2048];
  extern char gszConfFile[_MAX_PATH];
  extern char *szTokenName[];
  extern char gszRootPathFromBin[_MAX_PATH];
  extern int gbTraceOnTerminal;

/******************************************************************************
 *                                                                            *
 *                                 Prototypes                                 *
 *                                                                            *
 ******************************************************************************/ 

  void vSetLogFileTitle(void);
  void vTraceMsg( char *szMsg );
  void _vTraceMsgDialog( char *szMsg, ... );
  void vTracePid( char *szMsg, int iMsgLen );
  void vTraceMsgNoNL( char *szMsg );
  void vInitLogs(char* pszTrace, const char* pszDebugLevel);
  void _vTraceVarArgsFn(char *pszModuleName, const int kiLine, const char *kpszFunctionName, const char *kpszFmt, ...);
  void vTraceMainLoopEnd();
  void vTraceMainLoopInit();
   
#endif /* _TRACE_H */
