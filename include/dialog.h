#ifndef DIALOG_H
#define DIALOG_H

#include <card_game.h>

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

/* --- Utilidade --- */
int iDlgGetTopIndex(void);
int iDlgGetCount(void);
PSTRUCT_DIALOG pstDlgGetAt(int iIndex);

/* Estado global (uma lista de diálogo) */
extern STRUCT_DIALOG_LIST gstDlgList;

#endif
