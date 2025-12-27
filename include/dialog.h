#ifndef DIALOG_H
#define DIALOG_H

/* Nó de diálogo (duplamente encadeado) */
typedef struct STRUCT_DIALOG {
  int iLevel;
  char *pszMsg;               /* mensagem alocada */
  char szDT[64];              /* timestamp opcional */
  struct STRUCT_DIALOG *pstPrev;
  struct STRUCT_DIALOG *pstNext;
} STRUCT_DIALOG, *PSTRUCT_DIALOG;

/* Lista de diálogos + estado de rolagem */
typedef struct STRUCT_DIALOG_LIST {
  PSTRUCT_DIALOG pstHead;
  PSTRUCT_DIALOG pstTail;
  int iCount;                 /* total de linhas */
  int iTopIndex;              /* primeira linha visível (rolagem) */
  int iPinned;                /* 0 = follow automático; 1 = posição “pinada” por scroll manual */
} STRUCT_DIALOG_LIST, *PSTRUCT_DIALOG_LIST;

/* --- API básica (compatível com sua ideia original) --- */
void vInitDialog(void);
int iAddMsgToDialog(char *pszMsg, int iMsgLen);  /* push back */
void vFreeDialog(void);
void vTraceDialog(int bLogDT);

/* --- Scroll --- */
void vDlgScrollLines(int iDelta, int iVisibleCount);
void vDlgPageUp(int iVisibleCount);
void vDlgPageDown(int iVisibleCount);
void vDlgScrollToBottom(int iVisibleCount);
void vScrollToBottomInternal(int iVisibleCount);

/* --- Utilidade --- */
int iDlgGetTopIndex(void);
int iDlgGetCount(void);
void vClearPin(void);
void vSetPin(void);
PSTRUCT_DIALOG pstDlgGetAt(int iIndex);

/* Estado global (uma lista de diálogo) */
extern STRUCT_DIALOG_LIST gstDlgList;
#ifdef USE_SDL2
  typedef struct STRUCT_SDL_DIALOG_LAYOUT {
    int iX;
    int iY;
    int iW;
    int iH;
    int iVisibleLines;
    int iLineHeight;
    SDL_Rect stBtnUp;
    SDL_Rect stBtnDown;
} STRUCT_SDL_DIALOG_LAYOUT, *PSTRUCT_SDL_DIALOG_LAYOUT;
#endif

#endif
