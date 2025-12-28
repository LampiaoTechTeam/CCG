/**
 * screen.c
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in Dec 2025
 *
 * Description: Screen module
 */

#ifdef USE_SDL2

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys_interface.h>
#include <xml.h>
#include <screen.h>
#include <card_game.h>
#include <consts.h>

STRUCT_SCREEN_COLOR stWrkColor;
STRUCT_SCREEN_RECT stWrkRect;
STRUCT_SCREEN_ITEM stWrkItem;
STRUCT_ELEMENT stWrkElement;
STRUCT_LAYOUT stWrkLayout;
STRUCT_SCREEN gstScreen;

STRUCT_XML astColorXml[] = {
  { "R" , XMLTYPE_INT , 0, &stWrkColor.r, NULL },
  { "G" , XMLTYPE_INT , 0, &stWrkColor.g, NULL },
  { "B" , XMLTYPE_INT , 0, &stWrkColor.b, NULL },
  { "A" , XMLTYPE_INT , 0, &stWrkColor.a, NULL },
  { NULL, XMLTYPE_NULL, 0, NULL, NULL }
};

STRUCT_XML astRectXml[] = {
  { "H" , XMLTYPE_INT , 0, &stWrkRect.h, NULL },
  { "W" , XMLTYPE_INT , 0, &stWrkRect.w, NULL },
  { "X" , XMLTYPE_INT , 0, &stWrkRect.x, NULL },
  { "Y" , XMLTYPE_INT , 0, &stWrkRect.y, NULL },
  { NULL, XMLTYPE_NULL, 0, NULL, NULL }
};

int icbackItemXml(xmlNodePtr pstNode, void* pData __attribute__((unused)));
STRUCT_XML astItemXml[] = {
  { "ITEM"     , XMLTYPE_PROC  , 0                       , NULL                , icbackItemXml },
  { "ID"       , XMLTYPE_INT   , 0                       , &stWrkItem.iID      , NULL          },
  { "FONT"     , XMLTYPE_STRING, sizeof(stWrkItem.szFont), stWrkItem.szFont    , NULL          },
  { "FONT_SIZE", XMLTYPE_INT   , 0                       , &stWrkItem.iFontSize, NULL          },
  { "NAME"     , XMLTYPE_STRING, sizeof(stWrkItem.szName), stWrkItem.szName    , NULL          },
  { "TEXT"     , XMLTYPE_STRING, sizeof(stWrkItem.szText), stWrkItem.szText    , NULL          },
  { "RECT"     , XMLTYPE_PROC  , 0                       , NULL                , icbackItemXml },
  { "FG_COLOR" , XMLTYPE_PROC  , 0                       , NULL                , icbackItemXml },
  { "BG_COLOR" , XMLTYPE_PROC  , 0                       , NULL                , icbackItemXml },
  { "ACTION"   , XMLTYPE_INT   , 0                       , &stWrkItem.iAction  , NULL          },
  { NULL       , XMLTYPE_NULL  , 0                       , NULL                , NULL          }
};
int icbackItemXml(xmlNodePtr pstNode, void* pData __attribute__((unused))) {
  if ( !strcasecmp((char*)pstNode->name, "ITEM") ) {
    memset(&stWrkItem, 0x00, sizeof(stWrkItem));
    if ( stWrkElement.iCtItems >= MAX_SCREEN_ITEMS ) return 0;
    iParseXmlFields(pstNode, astItemXml);
    stWrkElement.astItem[stWrkElement.iCtItems] = stWrkItem;
    stWrkElement.iCtItems++;
  }

  if ( !strcasecmp((char*)pstNode->name, "RECT") ) {
     memset(&stWrkRect, 0x00, sizeof(stWrkRect));
     iParseXmlFields(pstNode, astRectXml);
     stWrkItem.stRect = stWrkRect;
  }

  if ( !strcasecmp((char*)pstNode->name, "FG_COLOR") ) {
    memset(&stWrkColor, 0x00, sizeof(stWrkColor));
    iParseXmlFields(pstNode, astColorXml);
    stWrkItem.stFgColor = stWrkColor;
  }

  if ( !strcasecmp((char*)pstNode->name, "BG_COLOR") ) {
    memset(&stWrkColor, 0x00, sizeof(stWrkColor));
    iParseXmlFields(pstNode, astColorXml);
    stWrkItem.stBgColor = stWrkColor;
  }
  return 1;
}

int icbackElementXml(xmlNodePtr pstNode, void* pData __attribute__((unused)));
STRUCT_XML astElementXml[] = {
  { "ELEMENT"     , XMLTYPE_PROC  , 0                          , NULL                   , icbackElementXml },
  { "ID"          , XMLTYPE_INT   , 0                          , &stWrkElement.iID      , NULL             },
  { "TYPE"        , XMLTYPE_INT   , 0                          , &stWrkElement.iType    , NULL             },
  { "NAME"        , XMLTYPE_STRING, sizeof(stWrkElement.szName), stWrkElement.szName    , NULL             },
  { "TEXT"        , XMLTYPE_STRING, sizeof(stWrkElement.szText), stWrkElement.szText    , NULL             },
  { "FONT"        , XMLTYPE_STRING, sizeof(stWrkElement.szFont), stWrkElement.szFont    , NULL             },
  { "FONT_SIZE"   , XMLTYPE_INT   , 0                          , &stWrkElement.iFontSize, NULL             },
  { "RECT"        , XMLTYPE_PROC  , 0                          , NULL                   , icbackElementXml },
  { "FG_COLOR"    , XMLTYPE_PROC  , 0                          , NULL                   , icbackElementXml },
  { "BG_COLOR"    , XMLTYPE_PROC  , 0                          , NULL                   , icbackElementXml },
  { "BORDER_COLOR", XMLTYPE_PROC  , 0                          , NULL                   , icbackElementXml },
  { "ITEM_LIST"   , XMLTYPE_PROC  , 0                          , NULL                   , icbackElementXml },
  { "ACTION"      , XMLTYPE_INT   , 0                          , &stWrkElement.iAction  , NULL             },
  { NULL          , XMLTYPE_NULL  , 0                          , NULL                   , NULL             }
};
int icbackElementXml(xmlNodePtr pstNode, void* pData __attribute__((unused))) {
  if ( !strcasecmp((char*)pstNode->name, "ELEMENT") ) {
    memset(&stWrkElement, 0x00, sizeof(stWrkElement));
    if ( stWrkLayout.iCtElements >= MAX_ELEMENTS ) return 0;
    iParseXmlFields(pstNode, astElementXml);
    stWrkLayout.astElement[stWrkLayout.iCtElements] = stWrkElement;
    stWrkLayout.iCtElements++;
  }

  if ( !strcasecmp((char*)pstNode->name, "RECT") ) {
    memset(&stWrkRect, 0x00, sizeof(stWrkRect));
    iParseXmlFields(pstNode, astRectXml);
    stWrkElement.stRect = stWrkRect;
  }

  if ( !strcasecmp((char*)pstNode->name, "FG_COLOR") ) {
    memset(&stWrkColor, 0x00, sizeof(stWrkColor));
    iParseXmlFields(pstNode, astColorXml);
    stWrkElement.stFgColor = stWrkColor;
  }

  if ( !strcasecmp((char*)pstNode->name, "BG_COLOR") ) {
    memset(&stWrkColor, 0x00, sizeof(stWrkColor));
    iParseXmlFields(pstNode, astColorXml);
    stWrkElement.stBgColor = stWrkColor;
  }

  if ( !strcasecmp((char*)pstNode->name, "BORDER_COLOR") ) {
    memset(&stWrkColor, 0x00, sizeof(stWrkColor));
    iParseXmlFields(pstNode, astColorXml);
    stWrkElement.stBorderColor = stWrkColor;
  }

  if ( !strcasecmp((char*)pstNode->name, "ITEM_LIST") ) {
    memset(&stWrkItem, 0x00, sizeof(stWrkItem));
    iParseXmlFields(pstNode, astItemXml);
  }
  return 1;
}

int icbackLayoutXml(xmlNodePtr pstNode, void* pData __attribute__((unused)));
STRUCT_XML astLayoutXml[] = {
  { "LAYOUT"      , XMLTYPE_PROC  , 0                          , NULL                  , icbackLayoutXml },
  { "ID"          , XMLTYPE_INT   , 0                          , &stWrkLayout.iID      , NULL            },
  { "TYPE"        , XMLTYPE_INT   , 0                          , &stWrkLayout.iType    , NULL            },
  { "TITLE"       , XMLTYPE_STRING, sizeof(stWrkLayout.szTitle), stWrkLayout.szTitle   , NULL            },
  { "FONT"        , XMLTYPE_STRING, sizeof(stWrkLayout.szFont) , stWrkLayout.szFont    , NULL            },
  { "FONT_SIZE"   , XMLTYPE_INT   , 0                          , &stWrkLayout.iFontSize, NULL            },
  { "ELEMENT_LIST", XMLTYPE_PROC  , 0                          , NULL                  , icbackLayoutXml },
  { NULL          , XMLTYPE_NULL  , 0                          , NULL                  , NULL            }
};
int icbackLayoutXml(xmlNodePtr pstNode, void* pData __attribute__((unused))) {
  if ( !strcasecmp((char*)pstNode->name, "ELEMENT_LIST") ) {
    memset(&stWrkElement, 0x00, sizeof(stWrkElement));
    iParseXmlFields(pstNode, astElementXml);
  }
  return 1;
}

int icbackScreenXml(xmlNodePtr pstNode, void* pData __attribute__((unused)));
STRUCT_XML astScreenXml[] = {
  { "SCREEN"       , XMLTYPE_PROC  , 0                          , NULL                    , icbackScreenXml },
  { "WINDOW_WIDTH" , XMLTYPE_INT   , 0                          , &gstScreen.iWindowWidth , NULL            },
  { "WINDOW_HEIGHT", XMLTYPE_INT   , 0                          , &gstScreen.iWindowHeight, NULL            },
  { "FONT"         , XMLTYPE_STRING, sizeof(gstScreen.szFont)   , gstScreen.szFont        , NULL            },
  { "FONT_DIR"     , XMLTYPE_STRING, sizeof(gstScreen.szFontDir), gstScreen.szFontDir     , NULL            },
  { "FONT_SIZE"    , XMLTYPE_INT   , 0                          , &gstScreen.iFontSize    , NULL            },
  { "LAYOUT"       , XMLTYPE_PROC  , 0                          , NULL                    , icbackScreenXml },
  { NULL           , XMLTYPE_NULL  , 0                          , NULL                    , NULL            }
};
int icbackScreenXml(xmlNodePtr pstNode, void* pData __attribute__((unused))) {
  if ( !strcasecmp((char*)pstNode->name, "SCREEN") ) {
    memset(&gstScreen, 0x00, sizeof(gstScreen));
    iParseXmlFields(pstNode, astScreenXml);
  }

  if ( !strcasecmp((char*)pstNode->name, "LAYOUT") ) {
    memset(&stWrkLayout, 0x00, sizeof(stWrkLayout));
    if ( gstScreen.iCtLayouts >= MAX_LAYOUTS ) return 0;
    iParseXmlFields(pstNode, astLayoutXml);
    gstScreen.astLayout[gstScreen.iCtLayouts] = stWrkLayout;
    gstScreen.iCtLayouts++;
  }

  return 1;
}

static int bScreenXmlIsValid(void) {
  return !bStrIsEmpty(gstScreen.szFontDir) && !bStrIsEmpty(gstScreen.szFont) && gstScreen.iFontSize > 0;
}

void vTraceElement(PSTRUCT_ELEMENT pstElement) {
  int kk = 0;
  vTraceVarArgsFn("  Element [%d]", pstElement->iID);
  vTraceVarArgsFn("    Type.....: [%d]", pstElement->iType);
  vTraceVarArgsFn("    Name.....: [%s]", pstElement->szName);
  vTraceVarArgsFn("    Text.....: [%s]", pstElement->szText);
  vTraceVarArgsFn("    Font.....: [%s]", pstElement->szFont);
  vTraceVarArgsFn("    Font size: [%d]", pstElement->iFontSize);
  vTraceVarArgsFn(
    "    Rect: [x=(%d) y=(%d) w=(%d) h=(%d)]",
    pstElement->stRect.x,
    pstElement->stRect.y,
    pstElement->stRect.w,
    pstElement->stRect.h
  );
  for ( kk = 0; kk < pstElement->iCtItems; kk++ ) {
    PSTRUCT_SCREEN_ITEM pstItem = &pstElement->astItem[kk];
    vTraceVarArgsFn("    Item [%d]", pstItem->iID);
    vTraceVarArgsFn("      Type.....: [%d]", pstItem->iType);
    vTraceVarArgsFn("      Name.....: [%s]", pstItem->szName);
    vTraceVarArgsFn("      Text.....: [%s]", pstItem->szText);
    vTraceVarArgsFn("      Font.....: [%s]", pstItem->szFont);
    vTraceVarArgsFn("      Font size: [%d]", pstItem->iFontSize);
    vTraceVarArgsFn(
      "      Rect: [x=(%d) y=(%d) w=(%d) h=(%d)]",
      pstItem->stRect.x,
      pstItem->stRect.y,
      pstItem->stRect.w,
      pstItem->stRect.h
    );
  }
}

static void vTraceScreen(void) {
  int ii = 0;
  int jj = 0;
  vTraceMsg("================================================");
  vTraceMsg("||                   SCREEN                   ||");
  vTraceMsg("================================================");
  vTraceVarArgsFn("Window Width.: [%d]", gstScreen.iWindowWidth);
  vTraceVarArgsFn("Window Height: [%d]", gstScreen.iWindowHeight);
  vTraceVarArgsFn("Font Dir.....: [%s]", gstScreen.szFontDir);
  vTraceVarArgsFn("Font.........: [%s]", gstScreen.szFont);
  vTraceVarArgsFn("Font size....: [%d]", gstScreen.iFontSize);
  vTraceVarArgsFn("Layouts......: [%d]", gstScreen.iCtLayouts);
  for ( ii = 0; ii < gstScreen.iCtLayouts; ii++ ) {
    PSTRUCT_LAYOUT pstLayout = &gstScreen.astLayout[ii];
    vTraceVarArgsFn("Layout [%d]", pstLayout->iID);
    vTraceVarArgsFn("  Type.....: [%d]", pstLayout->iType);
    vTraceVarArgsFn("  Font.....: [%s]", pstLayout->szFont);
    vTraceVarArgsFn("  Font size: [%d]", pstLayout->iFontSize);
    vTraceVarArgsFn("  Elements.: [%d]", pstLayout->iCtElements);
    for ( jj = 0; jj < pstLayout->iCtElements; jj++ ) {
      PSTRUCT_ELEMENT pstElement = &pstLayout->astElement[jj];
      vTraceElement(pstElement);
    }
  }
  vTraceMsg("================================================");
}

int bLoadScreenXml() {
  int ii = 0;
  int jj = 0;
  int kk = 0;
  char szScreenXmlPath[_MAX_PATH+_MAX_PATH] = "";

  if ( DEBUG_SCREEN_MSGS ) vTraceVarArgsFn("begin");

  memset(szScreenXmlPath, 0x00, sizeof(szScreenXmlPath));
  snprintf(szScreenXmlPath, sizeof(szScreenXmlPath), "%s%cscreen.xml", gstGlobalPrm.szConfDir, DIR_SEPARATOR);
  if ( bStrIsEmpty(szScreenXmlPath) ) {
    if ( DEBUG_SCREEN_MSGS ) vTraceVarArgsFn("szScreenXmlPath is empty!");
    return 0;
  }

  if ( !bLoadXmlFromFile(szScreenXmlPath, astScreenXml) ) {
    if ( DEBUG_SCREEN_MSGS ) vTraceVarArgsFn("Falha ao carregar arquivo %s", szScreenXmlPath);
    return 0;
  }

  if ( !bScreenXmlIsValid() ) {
    if ( DEBUG_SCREEN_MSGS ) vTraceVarArgsFn("Configuracoes invalidas no screen.xml!");
    return 0;
  }

  /* Percorremos a estrutura e preenchemos valores vazios com valores padrao */
  for ( ii = 0; ii < gstScreen.iCtLayouts; ii++ ) {
    if ( bStrIsEmpty(gstScreen.astLayout[ii].szFont) ) {
      snprintf(gstScreen.astLayout[ii].szFont, sizeof(gstScreen.astLayout[ii].szFont), "%s", gstScreen.szFont);
      gstScreen.astLayout[ii].iFontSize = gstScreen.iFontSize;
    }
    for ( jj = 0; jj < gstScreen.astLayout[ii].iCtElements; jj++ ) {
      if ( bStrIsEmpty(gstScreen.astLayout[ii].astElement[jj].szFont) ) {
        char szDefaultFont[32] = "";
        memset(szDefaultFont, 0x00, sizeof(szDefaultFont));
        snprintf(szDefaultFont, sizeof(szDefaultFont), "%s", gstScreen.astLayout[ii].szFont);
        snprintf(gstScreen.astLayout[ii].astElement[jj].szFont, sizeof(gstScreen.astLayout[ii].astElement[jj].szFont), "%s", szDefaultFont);
        gstScreen.astLayout[ii].iFontSize = gstScreen.astLayout[ii].iFontSize;
      }
      for ( kk = 0; kk < gstScreen.astLayout[ii].astElement[jj].iCtItems; kk++ ) {
        if ( bStrIsEmpty(gstScreen.astLayout[ii].astElement[jj].astItem[kk].szFont) ) {
          char szDefaultFont[32] = "";
          memset(szDefaultFont, 0x00, sizeof(szDefaultFont));
          snprintf(szDefaultFont, sizeof(szDefaultFont), "%s", gstScreen.astLayout[ii].astElement[jj].szFont);
          snprintf(gstScreen.astLayout[ii].astElement[jj].astItem[kk].szFont, sizeof(gstScreen.astLayout[ii].astElement[jj].astItem[kk].szFont), "%s", szDefaultFont);
          gstScreen.astLayout[ii].astElement[jj].astItem[kk].iFontSize = gstScreen.astLayout[ii].astElement[jj].iFontSize;
        }
      }
    }
  }

  if ( DEBUG_SCREEN_ALL ) vTraceScreen();

  if ( DEBUG_SCREEN_MSGS ) vTraceVarArgsFn("end");

  return 1;
}

int iSCREEN_SetLayout(int iID) {
  gstScreen.iCurrentLayout = iID;
  return 1;
}

int iSCREEN_GetLayout(void) {
  return gstScreen.iCurrentLayout;
}

PSTRUCT_LAYOUT pstSCREEN_GetCurrentLayout(void) {
  int ii = 0;
  for ( ii = 0; gstScreen.iCtLayouts; ii++ ) {
    if ( gstScreen.astLayout[ii].iID == gstScreen.iCurrentLayout ) {
      return &gstScreen.astLayout[ii];
    }
  }
  return NULL;
}

PSTRUCT_ELEMENT pstSCREEN_GetElementByType(int iType) {
  int ii = 0;
  PSTRUCT_LAYOUT pstLayout = pstSCREEN_GetCurrentLayout();
  for ( ii = 0; ii < pstLayout->iCtElements; ii++ ) {
    if ( pstLayout->astElement[ii].iType == iType ) {
      return &pstLayout->astElement[ii];
    }
  }
  return NULL;
}

PSTRUCT_ELEMENT pstSCREEN_GetElementByName(const char* szName) {
  int ii = 0;
  PSTRUCT_LAYOUT pstLayout = pstSCREEN_GetCurrentLayout();
  for ( ii = 0; ii < pstLayout->iCtElements; ii++ ) {
    if ( !strcasecmp(pstLayout->astElement[ii].szName, szName) ) {
      return &pstLayout->astElement[ii];
    }
  }
  return NULL;
}

PSTRUCT_ELEMENT pstSCREEN_GetElementById(int iID) {
  int ii = 0;
  PSTRUCT_LAYOUT pstLayout = pstSCREEN_GetCurrentLayout();
  for ( ii = 0; ii < pstLayout->iCtElements; ii++ ) {
    if ( pstLayout->astElement[ii].iID == iID ) {
      return &pstLayout->astElement[ii];
    }
  }
  return NULL;
}

PSTRUCT_SCREEN_ITEM pstSCREEN_GetItemByType(int iElmIdx, int iType) {
  int ii = 0;
  PSTRUCT_LAYOUT pstLayout = pstSCREEN_GetCurrentLayout();
  for ( ii = 0; ii < pstLayout->astElement[iElmIdx].iCtItems; ii++ ) {
    if ( pstLayout->astElement[iElmIdx].astItem[ii].iType == iType ) {
      return &pstLayout->astElement[iElmIdx].astItem[ii];
    }
  }
  return NULL;
}

PSTRUCT_SCREEN_ITEM pstSCREEN_GetItemByName(int iElmIdx, const char* szName) {
  int ii = 0;
  PSTRUCT_LAYOUT pstLayout = pstSCREEN_GetCurrentLayout();
  for ( ii = 0; ii < pstLayout->astElement[iElmIdx].iCtItems; ii++ ) {
    if ( !strcasecmp(pstLayout->astElement[iElmIdx].astItem[ii].szName, szName) ) {
      return &pstLayout->astElement[iElmIdx].astItem[ii];
    }
  }
  return NULL;
}

int bInsertElementInLayout(PSTRUCT_ELEMENT pstElement) {
  PSTRUCT_LAYOUT pstLayout = pstSCREEN_GetCurrentLayout();
  if ( pstLayout->iCtElements >= MAX_ELEMENTS ) return FALSE;
  pstLayout->iCtElements++;
  pstLayout->astElement[pstLayout->iCtElements] = *pstElement;
  return TRUE;
}

int bInsertItemInElement(int iElmId, PSTRUCT_SCREEN_ITEM pstItem) {
  PSTRUCT_ELEMENT pstElement = pstSCREEN_GetElementById(iElmId);
  if ( pstElement->iCtItems >= MAX_SCREEN_ITEMS ) return FALSE;
  pstElement->iCtItems++;
  pstElement->astItem[pstElement->iCtItems] = *pstItem;
  return TRUE;
}

#endif

