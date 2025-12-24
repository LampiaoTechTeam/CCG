#ifndef TERMINAL_UTILS_H
  #define TERMINAL_UTILS_H
  // #include <card_game.h>
  #define NO_NEW_LINE 0
  #define INSERT_NEW_LINE 1

  typedef enum E_TERMINAL_COLORS{
    TERMINAL_COLOR_BLACK = 30,
    TERMINAL_COLOR_RED      ,
    TERMINAL_COLOR_GREEN    ,
    TERMINAL_COLOR_YELLOW   ,
    TERMINAL_COLOR_BLUE     ,
    TERMINAL_COLOR_MAGENTA  ,
    TERMINAL_COLOR_CYAN     ,
    TERMINAL_COLOR_WHITE    ,
    TERMINAL_COLOR_BBLACK = 90,
    TERMINAL_COLOR_BRED     ,
    TERMINAL_COLOR_BGREEN   ,
    TERMINAL_COLOR_BYELLOW  ,
    TERMINAL_COLOR_BBLUE    ,
    TERMINAL_COLOR_BMAGENTA ,
    TERMINAL_COLOR_BCYAN    ,
    TERMINAL_COLOR_BWHITE 
  } E_TERMINAL_COLORS;

  /**
   * @brief Move o cursor para uma posição específica no terminal.
   * @param iRow Linha (1-based)
   * @param iCol Coluna (1-based)
   */
  void vSetCursorPosition(int iRow, int iCol);

  /**
   * @brief Limpa toda a tela e move o cursor para o início.
   */
  void vClearScreen();

  /**
   * @brief Posição fixa para exibir o input do jogador.
   */
  void vGotoInputPosition();

  /**
   * @brief Posição fixa para exibir mensagens de feedback.
   */
  void vGotoFeedbackPosition();
  /**
   * @brief Posição fixa do input (onde começa a digitação, após o label).
   */
  void vGotoInputTextPosition();
  /**
   * @brief Posição fixa para exibir mensagens de erro feedback.
   */
  void vGotoFeedbackErrorPosition();

  /**
   * @brief Imprime um texto colorido.
   * @param pszText Texto a imprimir
   * @param iColor Código ANSI da cor (ex: 31 = vermelho)
   */
  void vPrintColored(const char *pszText, int iColor);
  /**
   * @brief Limpa do cursor até o final da linha atual.
   */
  void vClearLineFromCursor();
  void vPrintLine(char *pszLine, int bNewLine);
  void vPrintHighlitedLine(char *pszLine, int bNewLine);

#endif
