#ifdef USE_SDL2
  #include <SDL2/SDL.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dialog.h>
#include <debuff.h>
#include <deck.h>
#include <monster.h>
#include <player.h>
#include <sys_interface.h>
#include <card_game.h>
#include <trace.h>
#include <time.h>

#ifdef USE_SDL2
  #include <SDL2/SDL_ttf.h>
  #include <sdl_api.h>
#endif

/* estado global */
STRUCT_DIALOG_LIST gstDlgList;

/* ---------- Helpers internos ---------- */
/* dialog.c */
static inline int iDlgGetMaxTop(int iVisibleCount) {
  int iMaxTop;
  if (iVisibleCount < 1)
    iVisibleCount = 1;
  iMaxTop = gstDlgList.iCount - iVisibleCount;
  if (iMaxTop < 0)
    iMaxTop = 0;
  return iMaxTop;
}
/* helpers locais */
static int iClamp(int iVal, int iMin, int iMax) {
  if (iVal < iMin) return iMin;
  if (iVal > iMax) return iMax;
  return iVal;
}
void vDlgScrollLines(int iDelta, int iVisibleCount) {
  gstDlgList.iTopIndex = iClamp(
    gstDlgList.iTopIndex + iDelta,
    0,
    iDlgGetMaxTop(iVisibleCount)
  );
}

static PSTRUCT_DIALOG pstDlgNodeCreate(const char *pszMsgIn, int iMsgLen, int iLevel) {
  PSTRUCT_DIALOG pstNode;
  int iAllocLen;
  time_t lNow;
  struct tm *pstTM;

  pstNode = (PSTRUCT_DIALOG)malloc(sizeof(STRUCT_DIALOG));
  if (pstNode == NULL)
    return NULL;

  pstNode->pstPrev = NULL;
  pstNode->pstNext = NULL;
  pstNode->iLevel = iLevel;

  if (iMsgLen < 0)
    iMsgLen = (int)strlen(pszMsgIn);
  iAllocLen = iMsgLen + 1;
  pstNode->pszMsg = (char*)malloc((size_t)iAllocLen);
  if (pstNode->pszMsg == NULL) {
    free(pstNode);
    return NULL;
  }
  memcpy(pstNode->pszMsg, pszMsgIn, (size_t)iMsgLen);
  pstNode->pszMsg[iMsgLen] = '\0';

  lNow = time(NULL);
  pstTM = localtime(&lNow);
  if (pstTM != NULL) {
    /* exemplo: 2025-10-17 13:04:22 */
    snprintf(pstNode->szDT, sizeof(pstNode->szDT),
             "%04d-%02d-%02d %02d:%02d:%02d",
             pstTM->tm_year + 1900,
             pstTM->tm_mon + 1,
             pstTM->tm_mday,
             pstTM->tm_hour,
             pstTM->tm_min,
             pstTM->tm_sec);
  } else {
    strcpy(pstNode->szDT, "0000-00-00 00:00:00");
  }

  return pstNode;
}

static void vDlgNodeDestroy(PSTRUCT_DIALOG pstNode) {
  if (pstNode == NULL)
    return;
  if (pstNode->pszMsg != NULL)
    free(pstNode->pszMsg);
  free(pstNode);
}
/* ---------- API pública ---------- */

void vInitDialog(void) {
  vFreeDialog();
  gstDlgList.iPinned = 0;
}

int iAddMsgToDialog(char *pszMsg, int iMsgLen) {
  PSTRUCT_DIALOG pstNode;

  if (pszMsg == NULL)
    return -1;

  pstNode = pstDlgNodeCreate(pszMsg, iMsgLen, 0);
  if (pstNode == NULL)
    return -2;

  if (gstDlgList.pstTail == NULL) {
    gstDlgList.pstHead = pstNode;
    gstDlgList.pstTail = pstNode;
  } else {
    pstNode->pstPrev = gstDlgList.pstTail;
    gstDlgList.pstTail->pstNext = pstNode;
    gstDlgList.pstTail = pstNode;
  }

  gstDlgList.iCount++;
  #ifdef USE_SDL2
    if (gbSDL_Mode){
      int iWinH = 800;
      int iDlgY;
      int iDlgH;
      int iVisible;

      /* calcula o mesmo tamanho de área que o renderer usa */
      iDlgY = 50 + 500 + 2;
      iDlgH = iWinH - iDlgY - 2;
      if (iDlgH < 0)
        iDlgH = 0;

      /* mesmo cálculo que o draw usa: 8 px topo + 8 px base, 18 px por linha */
      iVisible = (iDlgH - 16) / 18;
      if (iVisible < 1)
        iVisible = 1;

      iDlgMaybeFollowTail(iVisible);
    }
  #endif
  return 0;
}

void vFreeDialog(void) {
  PSTRUCT_DIALOG pstCur;
  PSTRUCT_DIALOG pstNext;

  pstCur = gstDlgList.pstHead;
  while (pstCur != NULL) {
    pstNext = pstCur->pstNext;
    vDlgNodeDestroy(pstCur);
    pstCur = pstNext;
  }

  gstDlgList.pstHead = NULL;
  gstDlgList.pstTail = NULL;
  gstDlgList.iCount = 0;
  gstDlgList.iTopIndex = 0;
}

void vTraceDialog(int bLogDT) {
  PSTRUCT_DIALOG pstCur;
  int iIdx;

  pstCur = gstDlgList.pstHead;
  iIdx = 0;

  while (pstCur != NULL) {
    char *pszMsg;
    int iLen = strlen(pstCur->szDT) + strlen(pstCur->pszMsg) + 32;
    if ( (pszMsg = (char *)malloc(iLen)) == NULL ) 
      return ;
    
    memset(pszMsg, 0, iLen);
    if (bLogDT != 0)
      sprintf(pszMsg, "[%03d][%s] %s\n", iIdx, pstCur->szDT, pstCur->pszMsg);
    else
      sprintf(pszMsg, "[%03d] %s\n", iIdx, pstCur->pszMsg);

    #ifdef USE_SDL2
      if (!gbSDL_Mode)
        vTraceMsgDialog(pszMsg);
    #else
      vTraceMsgDialog(pszMsg);
    #endif
    free(pszMsg);
    pstCur = pstCur->pstNext;
    iIdx++;
  }
}

void vClearPin(void) {
  gstDlgList.iPinned = 0;
}

void vSetPin(void) {
  gstDlgList.iPinned = 1;
}

void vScrollToBottomInternal(int iVisibleCount) {
  int iMaxTop;
  if (iVisibleCount < 1)
    iVisibleCount = 1;
  iMaxTop = gstDlgList.iCount - iVisibleCount;
  if (iMaxTop < 0)
    iMaxTop = 0;
  gstDlgList.iTopIndex = iMaxTop;
}

/* ---------- Navegação/scroll ---------- */
PSTRUCT_DIALOG pstDlgGetAt(int iIndex) {
  PSTRUCT_DIALOG pstCur;
  int ii;

  if (iIndex < 0 || iIndex >= gstDlgList.iCount)
    return NULL;

  if (iIndex <= (gstDlgList.iCount / 2)) {
    pstCur = gstDlgList.pstHead;
    ii = 0;
    while (pstCur != NULL && ii < iIndex) {
      pstCur = pstCur->pstNext;
      ii++;
    }
    return pstCur;
  } else {
    pstCur = gstDlgList.pstTail;
    ii = gstDlgList.iCount - 1;
    while (pstCur != NULL && ii > iIndex) {
      pstCur = pstCur->pstPrev;
      ii--;
    }
    return pstCur;
  }
}

int iDlgGetTopIndex(void) {
  return gstDlgList.iTopIndex;
}

int iDlgGetCount(void) {
  return gstDlgList.iCount;
}


void vDlgPageUp(int iVisibleCount) {
  vDlgScrollLines(-iVisibleCount, iVisibleCount);
}

void vDlgPageDown(int iVisibleCount) {
  vDlgScrollLines(iVisibleCount, iVisibleCount);
}

void vDlgScrollToBottom(int iVisibleCount) {
  int iMaxTop;

  if (iVisibleCount < 1)
    iVisibleCount = 1;

  iMaxTop = gstDlgList.iCount - iVisibleCount;
  if (iMaxTop < 0)
    iMaxTop = 0;

  gstDlgList.iTopIndex = iMaxTop;
}
