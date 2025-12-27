/**
 * screen.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in Dec 2025
 *
 * Description: Screen module
 */

#ifndef _SCREEN_H_
#define _SCREEN_H_

#ifdef USE_SDL2

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

#define MAX_SCREEN_ITEMS 64
#define MAX_ELEMENTS     64
#define MAX_LAYOUTS      64

typedef struct STRUCT_SCREEN_RECT {
  int x;
  int y;
  int w;
  int h;
} STRUCT_SCREEN_RECT;

typedef struct STRUCT_SCREEN_COLOR {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} STRUCT_SCREEN_COLOR, *PSTRUCT_SCREEN_COLOR;

typedef struct STRUCT_SCREEN_ITEM {
  int iID;
  int iType;
  char szFont[32];
  int iFontSize;
  char szName[32];
  char szText[32];
  STRUCT_SCREEN_RECT stRect;
  STRUCT_SCREEN_COLOR stFgColor;
  STRUCT_SCREEN_COLOR stBgColor;
} STRUCT_SCREEN_ITEM, *PSTRUCT_SCREEN_ITEM;

typedef struct STRUCT_ELEMENT {
  int iID;
  char szName[32];
  char szText[32];
  int iType; /* menu, button, etc */
  STRUCT_SCREEN_RECT stRect;
  STRUCT_SCREEN_COLOR stFgColor;
  STRUCT_SCREEN_COLOR stBgColor;
  char szFont[32];
  int iFontSize;
  int iCtItems;
  int iSelectedItemIdx;
  STRUCT_SCREEN_ITEM astItem[MAX_SCREEN_ITEMS];
} STRUCT_ELEMENT, *PSTRUCT_ELEMENT;

typedef struct STRUCT_LAYOUT {
  int iID;
  char szTitle[32];
  int iType; /* login, save, etc */
  char szFont[32];
  int iFontSize;
  int iCtElements;
  STRUCT_ELEMENT astElement[MAX_ELEMENTS];
} STRUCT_LAYOUT, *PSTRUCT_LAYOUT;

typedef struct STRUCT_SCREEN {
  int iWindowWidth;
  int iWindowHeight;
  char szFont[32];
  int iFontSize;
  char szFontDir[256];
  int iCurrentLayout;
  int iCtLayouts;
  STRUCT_LAYOUT astLayout[MAX_LAYOUTS];
} STRUCT_SCREEN, *PSTRUCT_SCREEN;

extern STRUCT_SCREEN gstScreen;

int bLoadScreenXml(char* pszXmlPath);

int iSCREEN_SetLayout(int iID);
int iSCREEN_GetLayout(void);

PSTRUCT_LAYOUT pstSCREEN_GetCurrentLayout(void);

PSTRUCT_ELEMENT pstSCREEN_GetElementByType(int iType);
PSTRUCT_ELEMENT pstSCREEN_GetElementByName(const char* pszName);
PSTRUCT_ELEMENT pstSCREEN_GetElementById(int iID);

PSTRUCT_SCREEN_ITEM pstSCREEN_GetItemByType(int iElmIdx, int iType);
PSTRUCT_SCREEN_ITEM pstSCREEN_GetItemByName(int iElmIdx, const char* pszName);

void vTraceElement(PSTRUCT_ELEMENT pstElement);

int bInsertElementInLayout(PSTRUCT_ELEMENT pstElement);
int bInsertItemInElement(int iElmId, PSTRUCT_SCREEN_ITEM pstItem);

#ifdef __cplusplus
}
#endif

#endif

#endif

