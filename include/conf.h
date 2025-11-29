/**
 * conf.h
 * 
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 * 
 * Description: module to read .conf files
 * 
 * Date: 24/10/2024
 */

#ifndef _CONF_H_
#define _CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum ENUM_PRIMITIVE_DATA_TYPES {
  DATATYPE_BOOL,
  DATATYPE_CHAR,
  DATATYPE_INT,
  DATATYPE_FLOAT,
  DATATYPE_DOUBLE,
  DATATYPE_STRING,
  DATATYPE_NULL
} ENUM_PRIMITIVE_DATA_TYPES, *PENUM_PRIMITIVE_DATA_TYPES;

typedef struct STRUCT_CONF_FILE {
  char *pszVariableName;
  void *pData;
  int iDataSize;
  int iDataType;
  char *pszDefaultValue;
} STRUCT_CONF_FILE, *PSTRUCT_CONF_FILE;

/**
 * @brief Set all values of pszDefaultValue in pData
 * 
 * @param astConfFile is the conf file array
 */
void vSetDefaultSettings(PSTRUCT_CONF_FILE astConfFile);

/**
 * @brief Read the .conf file and put the values in pData
 * 
 * @param kpszFileName is the name of .conf file
 * @param astConfFile is the conf file array
 * 
 * @return true or false
 */
int bReadConfFile(const char *kpszFileName, PSTRUCT_CONF_FILE astConfFile);

#ifdef __cplusplus
}
#endif

#endif
