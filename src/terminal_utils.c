#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <sys_interface.h>
#include <card_game.h>
#include <debuff.h>
#include <deck.h>
#include <monster.h>
#ifdef USE_SDL2
  #include <SDL2/SDL.h>
#endif
#include <player.h>
#include <dialog.h>
#include <input.h>
#include <trace.h>
#include <terminal_utils.h>

char *pszTerminalColors[] ={
  "\033",
  "\034",
  "\035",
  "\036",
  "\037",
  "\090",
  "\091",
  "\092",
  "\093",
  "\094",
  "\095",
  "\096",
  "\097",
  NULL
};

void vPrintLine(char *pszLine, int bNewLine) {
  #ifdef USE_SDL2
    if ( gbSDL_Mode ) vTraceVarArgsFn(pszLine);
    else {
      printf("%s", pszLine);
      if ( bNewLine )
        printf("\n");
    }
  #else
    printf("%s", pszLine);
    if ( bNewLine )
      printf("\n");
  #endif
  
  if ( bNewLine ) {
    char *pszWrkMsg;
    pszWrkMsg = (char *) malloc(strlen(pszLine) + 8);
    memset(pszWrkMsg,0,strlen(pszLine)+8);
    strcat(pszWrkMsg, pszLine);
    #ifndef USE_SDL2
    strcat(pszWrkMsg, "\n");
    #endif 
    iAddMsgToDialog(pszWrkMsg, strlen(pszWrkMsg));
    free(pszWrkMsg);
    return;
  }
  
  iAddMsgToDialog(pszLine, strlen(pszLine));
}

void vPrintHighlitedLine(char *pszLine, int bNewLine) {
  #ifdef USE_SDL2
    if ( gbSDL_Mode ) vTraceVarArgsFn(pszLine);
    else {
      printf("\x1b[7m%s\x1b[0m", pszLine);
      if ( bNewLine )
        printf("\n");
    }
  #else
  printf("\x1b[7m%s\x1b[0m", pszLine);
  if ( bNewLine )
    printf("\n");
  #endif
    
  if ( bNewLine ) {
    char *pszWrkMsg;
    pszWrkMsg = (char *) malloc(strlen(pszLine) + 8);
    memset(pszWrkMsg,0,strlen(pszLine)+8);
    strcat(pszWrkMsg, pszLine);
    #ifndef USE_SDL2
      strcat(pszWrkMsg, "\n");
    #endif 
    iAddMsgToDialog(pszWrkMsg, strlen(pszWrkMsg));
    free(pszWrkMsg);
    return;
  }

  iAddMsgToDialog(pszLine, strlen(pszLine));
}

void vSetCursorPosition(int iRow, int iCol) {
  printf("\033[%d;%dH", iRow, iCol);
  fflush(stdout);
}

void vClearScreen() {
  vClearTerminal();
    printf("\033[2J\033[1;1H");  /** Limpa a tela e volta para (1,1) */
  fflush(stdout);
}


void vGotoInputPosition() {
  vSetCursorPosition(11, 1);  /** Input sempre na linha 11 */
}

void vGotoFeedbackPosition() {
  vSetCursorPosition(13, 1);  /** Feedback sempre na linha 23 */
}
void vGotoFeedbackErrorPosition() {
  vSetCursorPosition(13, 1);  /** Feedback de erro sempre na linha 13 */
}

void vGotoInputTextPosition() {
  vSetCursorPosition(11, 21); /** Linha 11, coluna após "Digite sua palavra: " */
}

void vPrintColored(const char *pszText, int iColor) {
  iAddMsgToDialog((char*)pszText, strlen((char*)pszText));
#ifdef USE_SDL2
  if ( !gbSDL_Mode )
    printf("\033[%dm%s\033[0m", iColor, pszText);
#else
  printf("\033[%dm%s\033[0m", iColor, pszText);
#endif
}

void vClearLineFromCursor() {
  printf("\033[K");
  fflush(stdout);
}

