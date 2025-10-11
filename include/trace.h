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
  #include <card_game.h>

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

  typedef enum ENUM_DEBUG{
    DEBUG_LVL_NONE        = 0,
    DEBUG_LVL_FNCALL         ,
    DEBUG_LVL_SYSCALL     = 4,
    DEBUG_LVL_DETAILS     = 7,
    DEBUG_LVL_MORE_DETAILS   ,
    DEBUG_LVL_ALL
  }ENUM_DEBUG;

  #define TOKEN_MISMATCH -1
  #define ROOT_PATH_FROM_BIN ".."
  #define UNUSED(X) (void) X

  #define DEBUG_MSGS      giDebugLevel >=  DEBUG_LVL_FNCALL
  #define DEBUG_MORE_MSGS giDebugLevel >=  DEBUG_LVL_DETAILS
  #define DEBUG_DIALOG    giDebugLevel >=  DEBUG_LVL_MORE_DETAILS
  #define DEBUG_ALL       giDebugLevel >=  DEBUG_LVL_ALL

  #define vTraceVarArgsFn(FORMAT, ...) _vTraceVarArgsFn(__FILE__, __LINE__, __func__, FORMAT, ##__VA_ARGS__)
  #define vTraceMsgDialog(FORMAT, ...) _vTraceMsgDialog(FORMAT, ##__VA_ARGS__)
  #define vTraceBegin() vTraceVarArgsFn(" -- Begin -- ")
  #define vTraceEnd()   vTraceVarArgsFn(" --  End  -- ")

/******************************************************************************
 *                                                                            *
 *                     Global variables and constants                         *
 *                                                                            *
 ******************************************************************************/

  extern int  giDebugLevel;
  extern char gszTraceFile[2048];
  extern char gszConfFile[_MAX_PATH];
  extern char *szTokenName[];

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
  void vInitLogs( void );
  void _vTraceVarArgsFn(char *pszModuleName, const int kiLine, const char *kpszFunctionName, const char *kpszFmt, ...);
  void vTraceMainLoopEnd();
  void vTraceMainLoopInit();
   
#endif /* _TRACE_H */
