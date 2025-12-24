#include <debuff.h>
#include <deck.h>
#include <monster.h>
#ifdef USE_SDL2
  #include <SDL2/SDL.h>
#endif
#include <player.h>
#include <terminal_utils.h>
#include <input.h>
#include <trace.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#endif

/**
 * @brief Remove espaços no início e no fim da string.
 * @param pszStr - string a ser tratada
 */
void vTrimSpaces(char *pszStr) {
  char *pszStart = pszStr;
  char *pszEnd;

  // Remove espaços no início
  while (*pszStart == ' ') {
    pszStart++;
  }
 
  // Move string para o início se necessário
  if (pszStart != pszStr) {
    memmove(pszStr, pszStart, strlen(pszStart) + 1);
  }

  // Remove espaços no final
  pszEnd = pszStr + strlen(pszStr) - 1;
  while (pszEnd >= pszStr && *pszEnd == ' ') {
    *pszEnd = '\0';
    pszEnd--;
  }
}

void vShowInitDialog() {
  vClearScreen();
  vPrintHighlitedLine("=== Bem-vindo ao Card Game ===", INSERT_NEW_LINE);
  vPrintLine("Pressione qualquer tecla para comecar...", INSERT_NEW_LINE);
  iPortableGetchar();
}

/**
 * @brief Captura um único caractere do teclado, sem eco e sem buffer
 * @return int - Código ASCII do caractere lido
 */
int iPortableGetchar() {
#ifdef _WIN32
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD record;
  DWORD read;

  while (1) {
    ReadConsoleInput(hStdin, &record, 1, &read);

    if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
      WORD vkCode = record.Event.KeyEvent.wVirtualKeyCode;

      // Debug para Virtual Key Code
      // vPrintLine("[DEBUG - WIN] VK = %d\n", vkCode);
      fflush(stdout);

      switch (vkCode) {
        case VK_UP:    return -1;
        case VK_DOWN:  return -2;
        case VK_RIGHT: return -3;
        case VK_LEFT:  return -4;
        case VK_RETURN: return '\n';
        default:
          return record.Event.KeyEvent.uChar.AsciiChar;
      }
    }
  }
#else
  struct termios stOldt, stNewt;
  int iCh, iSecond = -1, iThird = -1;

  tcgetattr(STDIN_FILENO, &stOldt);
  stNewt = stOldt;
  stNewt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &stNewt);

  iCh = getchar();

  if (iCh == 27) { // ESC
    iSecond = getchar();
    if (iSecond == '[') {
      iThird = getchar();

      // vPrintLine("[DEBUG - LIN] Sequence: ESC (%d), [ (%d), Code (%d)\n", iCh, iSecond, iThird);
      tcsetattr(STDIN_FILENO, TCSANOW, &stOldt);

      switch (iThird) {
        case 'A': return -1; // ↑
        case 'B': return -2; // ↓
        case 'C': return -3; // →
        case 'D': return -4; // ←
        default:
          return 0;
      }
    }
  } else {
    // vPrintLine("[DEBUG - LIN] Key = %d (%c)\n", iCh, (iCh >= 32 && iCh <= 126) ? iCh : '?');
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &stOldt);
  return iCh;
#endif
}


/**
 * @brief Pausa a execução por um número de segundos (cross-platform)
 * @param iSeconds - tempo em segundos
 */
void vSleepSeconds(int iSeconds) {
  
  vTraceVarArgsFn("Sleeping %d seconds", iSeconds);
#ifdef _WIN32
  Sleep(iSeconds * 1000);
#else
  sleep(iSeconds);
#endif
}

/**
 * @brief Limpa o terminal (cross-platform)
 */
void vClearTerminal() {
#ifdef _WIN32
  system("cls");
#else
  int iRet = system("clear");
  (void)iRet;
#endif
}

/**
 * @brief Limpa buffer de entrada do teclado
 */
void vFlushInput() {
#ifdef _WIN32
  HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
  FlushConsoleInputBuffer(hIn);
#else
  int iC;
  while ((iC = getchar()) != '\n' && iC != EOF) {
  }
#endif
}

/**
 * @brief Converte uma string para minúsculas
 * @param pszBuf - string a ser convertida
 */
void vToLower(char *pszBuf) {
  char *p;
  for (p = pszBuf; *p; ++p)
    if (*p >= 'A' && *p <= 'Z')
      *p += 32; /* 'A'(65)+32 = 'a'(97) */
}

/**
 * @brief Verifica se a string contém apenas espaços
 * @param pszStr - string a ser verificada
 * @return bool - TRUE se for só espaços, FALSE caso contrário
 */
int bIsOnlySpaces(const char *pszStr) {
  while (*pszStr) {
    if (!((unsigned char)*pszStr == ' '))
      return FALSE;
    pszStr++;
  }
  return TRUE;
}

void vReadPlayerName(char *pszName, int iNameSz){
  do {
    vClearTerminal();
    vPrintLine("\n Escolha seu nome :", INSERT_NEW_LINE);
    vPrintLine("Nome: ", NO_NEW_LINE);

    memset(pszName, 0, iNameSz);
    if (fgets(pszName, iNameSz, stdin)) {
      if (strchr(pszName, '\n') == NULL)
        vFlushInput();
    }
  } while ( bStrIsEmpty(pszName) );
  
  strtok(pszName, "\n");

  return;
}
void vReadCardName(char *pszName, int iNameSz){

  memset(pszName, 0, iNameSz);
  if (fgets(pszName, iNameSz, stdin)) {
    if (strchr(pszName, '\n') == NULL)
      vFlushInput();
  }  
  if ( !bStrIsEmpty(pszName) ) strtok(pszName, "\n");

  return;
}

int iReadOptions() {
  return fgetc(stdin);
}
/**
 * @brief Captura input do usuário caractere a caractere, com redesenho contínuo
 * @return char* - ponteiro para a string digitada (precisa ser liberado com
 * free)
 */
char *cCatchInput() {
  char *pszBuffer = (char *)malloc(256);
  int iBufferLen = 0;
  memset(pszBuffer, 0, 256);

  while (TRUE) {
    /** Redesenha linha de input */
    vGotoInputPosition();
    vClearLineFromCursor();
    fflush(stdout);

    /** Captura um único caractere */
    int iCh = iPortableGetchar();

    if (iCh == '\n' || iCh == '\r') { /** Enter */
      if (iBufferLen > 0 && !bIsOnlySpaces(pszBuffer))
        break; /** Input válido */
      vGotoFeedbackPosition();
      vPrintLine("Você deve digitar uma palavra!", INSERT_NEW_LINE);
      fflush(stdout);
      vSleepSeconds(1);
      continue;
    } else if (iCh == 127 || iCh == 8) { /** Backspace */
      if (iBufferLen > 0) {
        iBufferLen--;
        pszBuffer[iBufferLen] = '\0';
      }
    } else if (isprint(iCh) &&
               iBufferLen < 256 - 1) { /** Caractere imprimível */
      pszBuffer[iBufferLen++] = (char)iCh;
      pszBuffer[iBufferLen] = '\0';
    }
  }

  /** Converte para minúsculo e remove acentos */
  vToLower(pszBuffer);

  return pszBuffer;
}
