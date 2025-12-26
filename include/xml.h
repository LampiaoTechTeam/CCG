/**
 * xml.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *
 * XML parse using libxml2
 */

#ifndef _XML_H_
#define _XML_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libxml/parser.h>
#include <trace.h>

/**
 * @enum ENUM_XML_TYPES
 * @brief XML data types
 */
typedef enum ENUM_XML_TYPES {
  XMLTYPE_PROC,
  XMLTYPE_STRING,
  XMLTYPE_INT,
  XMLTYPE_BOOL,
  XMLTYPE_DOUBLE,
  XMLTYPE_NULL
} ENUM_XML_TYPES, *PENUM_XML_TYPES;

/**
 * @struct STRUCT_XML
 * @brief Structure that represents a XML
 */
typedef struct STRUCT_XML {
  char* pszTag;                                    /**< TAG name                          */
  ENUM_XML_TYPES eType;                            /**< XML data type                     */
  size_t zDataSize;                                /**< sizeof pData                      */
  void* pData;                                     /**< Variable used to save tag content */
  int (*icbackXml)(xmlNode* pstNode, void* pData); /**< Callback used to parse a XML node */
} STRUCT_XML, *PSTRUCT_XML;

/**
 * @brief Parse XML fields
 *
 * @param pstNode XML node
 * @param astXml XML array
 *
 * @return TRUE
 */
int iParseXmlFields(xmlNodePtr pstNode, STRUCT_XML astXml[]);

/**
 * @brief Load XML from buffer
 *
 * @param pszXml XML contend
 * @param astXml XML array
 *
 * @return int
 */
int bLoadXml(char* pszXml, STRUCT_XML astXml[]);

/**
 * @brief Load XML from file
 *
 * @param pszFilePath XML file path
 * @param astXml XML array
 * @return int
 */
int bLoadXmlFromFile(char *pszFilePath, STRUCT_XML astXml[]);

#ifdef __cplusplus
}
#endif

#endif

