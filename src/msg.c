/**
 * msg.c
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *
 * Description: Messages of the game
 */

#include <strings.h>
#include <msg.h>
#include <xml.h>
#include <sys_interface.h>
#include <card_game.h>
#include <consts.h>

STRUCT_MSG stWrkMsg;
int giCtMsg = 0;
STRUCT_MSG gastMsg[MAX_MSGS];

int icbackMsgXml(xmlNodePtr pstNode, void* pData __attribute((unused)));
STRUCT_XML astMsgXml[] = {
  { "MSG_LIST", XMLTYPE_PROC  , 0                      , NULL           , icbackMsgXml },
  { "MSG"     , XMLTYPE_PROC  , 0                      , NULL           , icbackMsgXml },
  { "ID"      , XMLTYPE_INT   , 0                      , &stWrkMsg.iID  , NULL         },
  { "TEXT"    , XMLTYPE_STRING, sizeof(stWrkMsg.szText), stWrkMsg.szText, NULL         },
  { NULL      , XMLTYPE_NULL  , 0                      , NULL           , NULL         }
};
int icbackMsgXml(xmlNodePtr pstNode, void* pData __attribute__((unused))) {
  if ( !strcasecmp((char*)pstNode->name, "MSG_LIST") ) {
    memset(&stWrkMsg, 0x00, sizeof(stWrkMsg));
    iParseXmlFields(pstNode, astMsgXml);
  }
  if ( !strcasecmp((char*)pstNode->name, "MSG") ) {
    memset(&stWrkMsg, 0x00, sizeof(stWrkMsg));
    if ( giCtMsg >= MAX_MSGS ) return 1;
    iParseXmlFields(pstNode, astMsgXml);
    gastMsg[giCtMsg] = stWrkMsg;
    giCtMsg++;
  }
  return 1;
}

char* pszGetMsgById(int iID) {
  int ii = 0;
  static char* pszMsg = "";
  if ( DEBUG_XML_MORE_MSGS ) vTraceBegin();
  if ( iID <= 0 || iID > MAX_MSGS ) return pszMsg;
  for ( ii = 0; giCtMsg; ii++ ) {
    if ( iID == gastMsg[ii].iID ) {
      pszMsg = gastMsg[ii].szText;
      break;
    }
  }
  if ( DEBUG_XML_MORE_MSGS ) vTraceVarArgsFn("MSG = %s", pszMsg);
  return pszMsg;
}

int bLoadMsgXml(void) {
  int bRsl = FALSE;
  char szMsgFilePath[512] = "";
  memset(szMsgFilePath, 0x00, sizeof(szMsgFilePath));
  if ( DEBUG_XML_MORE_MSGS ) vTraceBegin();
  snprintf(szMsgFilePath, sizeof(szMsgFilePath), "%s%cmsg.xml", gstGlobalPrm.szConfDir, DIR_SEPARATOR);
  bRsl = bLoadXmlFromFile(szMsgFilePath, astMsgXml);
  if ( DEBUG_XML_MORE_MSGS ) vTraceVarArgsFn("end - bRsl = [%s]", bRsl ? "TRUE" : "FALSE");
  return bRsl;
}

