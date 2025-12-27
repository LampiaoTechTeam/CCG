/**
 * xml.c
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *
 * XML parse using libxml2
 */

#include "xml.h"

/**
 * @var giXmlSizeof
 * @brief sizeof XML buffer
 */
static int giXmlSizeof = 0;

int iParseXmlFields(xmlNodePtr pstNode, STRUCT_XML astXml[]) {
  xmlNodePtr pstCurrentXmlNode = NULL;
  int ii = 0;
  for ( pstCurrentXmlNode = pstNode->children; pstCurrentXmlNode; pstCurrentXmlNode = pstCurrentXmlNode->next ) {
    for ( ii = 0; astXml[ii].pszTag; ii++ ) {
      if ( DEBUG_XML_MORE_MSGS ) vTraceVarArgsFn("pstCurrentXmlNode->type [%d]", (int) pstCurrentXmlNode->type);
      if ( pstCurrentXmlNode->type != XML_ELEMENT_NODE ) continue;
      if ( DEBUG_XML_MORE_MSGS ) vTraceVarArgsFn("pstCurrentXmlNode->name [%s]", pstCurrentXmlNode->name);
      if ( !xmlStrcasecmp(pstCurrentXmlNode->name, (xmlChar*)astXml[ii].pszTag) ) {
        xmlChar* pszContent = xmlNodeGetContent(pstCurrentXmlNode);
        if ( bStrIsEmpty((char*)pszContent) ) continue;
        vTraceVarArgsFn("pszContent [%s]", pszContent);
        switch ( astXml[ii].eType ) {
          case XMLTYPE_BOOL: {
            *(int*)astXml[ii].pData = atoi((char*)pszContent);
            break;
          }
          case XMLTYPE_INT: {
            *(int*)astXml[ii].pData = atoi((char*)pszContent);
            break;
          }
          case XMLTYPE_DOUBLE: {
            *(double*)astXml[ii].pData = atof((char*)pszContent);
            break;
          }
          case XMLTYPE_STRING: {
            if ( astXml[ii].zDataSize > 0 ) {
              snprintf((char*)astXml[ii].pData, astXml[ii].zDataSize, "%s", (char*)pszContent);
            }
            break;
          }
          case XMLTYPE_PROC: {
            if ( astXml[ii].icbackXml ) {
              if ( DEBUG_XML_ALL ) vTraceVarArgsFn("Callback start...");
              astXml[ii].icbackXml(pstCurrentXmlNode, astXml);
            }
            break;
          }
          default: break;
        }
        xmlFree(pszContent);
        pszContent = NULL;
      }
    }
  }
  return 1;
}

int bLoadXml(char* pszXml, STRUCT_XML astXml[]) {
  int bRsl = 0;
  xmlDocPtr pstXmlDoc = NULL;
  xmlNodePtr pstXmlRoot = NULL;
  pstXmlDoc = xmlReadMemory(pszXml, giXmlSizeof, NULL, NULL, 0);
  if ( !pstXmlDoc ) {
    fprintf(stderr, "Error in XML parser!\n");
    return 0;
  }
  pstXmlRoot = xmlDocGetRootElement(pstXmlDoc);
  bRsl = iParseXmlFields(pstXmlRoot, astXml);
  xmlFreeDoc(pstXmlDoc);
  xmlCleanupParser();
  return bRsl;
}

int bLoadXmlFromFile(char* pszFilePath, STRUCT_XML astXml[]) {
  char* pszXml = NULL;
  FILE* fpXml = NULL;
  int bRsl = 0;
  if ( DEBUG_XML_MSGS ) vTraceBegin();
  giXmlSizeof = 0;
  if ( DEBUG_XML_MORE_MSGS ) vTraceVarArgsFn("Open the file [%s]", pszFilePath);
  if ( (fpXml = fopen(pszFilePath, "r")) == NULL ) {
    if ( DEBUG_XML_MSGS ) vTraceVarArgsFn("end FALSE");
    return 0;
  }
  fseek(fpXml, 0, SEEK_END);
  giXmlSizeof = ftell(fpXml);
  if ( DEBUG_XML_MORE_MSGS ) vTraceVarArgsFn("XML size [%ld]", giXmlSizeof);
  fseek(fpXml, 0, SEEK_SET);
  pszXml = (char*)calloc(1, sizeof(char) * giXmlSizeof);
  if ( !pszXml ) {
    fclose(fpXml);
    fpXml = NULL;
    return 0;
  }
  fread(pszXml, giXmlSizeof, 1, fpXml);
  fclose(fpXml);
  fpXml = NULL;
  if ( DEBUG_XML_ALL ) vTraceVarArgsFn("XML CONTENT: [%s]", pszXml);
  bRsl = bLoadXml(pszXml, astXml);
  free(pszXml);
  pszXml = NULL;
  if ( DEBUG_XML_MSGS ) vTraceVarArgsFn("bRsl [%s]", bRsl ? "TRUE" : "FALSE");
  if ( DEBUG_XML_MSGS ) vTraceEnd();
  return bRsl;
}

