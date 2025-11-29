/**
 * conf.c
 * 
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 * 
 * Description: module to read .conf files
 * 
 * Date: 24/10/2024
 */

#include "conf.h"

void vSetDefaultSettings(PSTRUCT_CONF_FILE astConfFile) {
  int ii = 0;
  for ( ii = 0; astConfFile[ii].pszVariableName; ii++ ) {
    switch ( astConfFile[ii].iDataType ) {
      case DATATYPE_BOOL: {
        if ( !strcmp(astConfFile[ii].pszDefaultValue, "false") ||
              !strcmp(astConfFile[ii].pszDefaultValue, "FALSE") ||
              !strcmp(astConfFile[ii].pszDefaultValue, "0") ) {
          *(int *) astConfFile[ii].pData = 0;
        }
        
        if ( !strcmp(astConfFile[ii].pszDefaultValue, "true")  ||
              !strcmp(astConfFile[ii].pszDefaultValue, "TRUE")  ||
              !strcmp(astConfFile[ii].pszDefaultValue, "1") ) {
          *(int *) astConfFile[ii].pData = 1;
        }
        break;
      }
      case DATATYPE_CHAR: {
        *(char *) astConfFile[ii].pData = astConfFile[ii].pszDefaultValue[0];
        break;
      }
      case DATATYPE_INT: {
        *(int *) astConfFile[ii].pData = atoi(astConfFile[ii].pszDefaultValue);
        break;
      }
      case DATATYPE_FLOAT: {
        *(float *) astConfFile[ii].pData = atof(astConfFile[ii].pszDefaultValue);
        break;
      }
      case DATATYPE_DOUBLE: {
        *(double *) astConfFile[ii].pData = atof(astConfFile[ii].pszDefaultValue);
        break;
      }
      case DATATYPE_STRING:
      default: {
        snprintf(astConfFile[ii].pData, astConfFile[ii].iDataSize, "%s", astConfFile[ii].pszDefaultValue);
        break;
      }
    }
  }
}

int bReadConfFile(const char *kpszFileName, PSTRUCT_CONF_FILE astConfFile) {
  FILE *fpFile = NULL;
  char szLine[1024] = "";
  int ii = 0;
  
  memset(szLine, 0x00, sizeof(szLine));
  
  if ( (fpFile = fopen(kpszFileName, "r")) == NULL ) {
    vSetDefaultSettings(astConfFile);
    return 1;
  }
  
  while ( fgets(szLine, sizeof(szLine), fpFile) ) {
    char *pTok = NULL;
    
    if ( szLine[0] == '#' ) continue;
    
    pTok = strtok(szLine, "=# \"");
    
    for ( ii = 0; astConfFile[ii].pszVariableName; ii++ ) {
      if ( !strcmp(pTok, astConfFile[ii].pszVariableName) ) {
        pTok = strtok(NULL, "=# \"\n");
        
        if ( pTok == NULL ) {
          pTok = astConfFile[ii].pszDefaultValue;
        }
        
        switch ( astConfFile[ii].iDataType ) {
          case DATATYPE_BOOL: {
            if      ( !strcmp(pTok, "false") || !strcmp(pTok, "FALSE") || !strcmp(pTok, "0") ) *(int *) astConfFile[ii].pData = 0;
            else if ( !strcmp(pTok, "true")  || !strcmp(pTok, "TRUE")  || !strcmp(pTok, "1") ) *(int *) astConfFile[ii].pData = 1;
            else    return 0;
            break;
          }
          case DATATYPE_CHAR: {
            *(char *) astConfFile[ii].pData = pTok[0];
            break;
          }
          case DATATYPE_INT: {
            *(int *) astConfFile[ii].pData = atoi(pTok);
            break;
          }
          case DATATYPE_FLOAT: {
            *(float *) astConfFile[ii].pData = atof(pTok);
            break;
          }
          case DATATYPE_DOUBLE: {
            *(double *) astConfFile[ii].pData = atof(pTok);
            break;
          }
          case DATATYPE_STRING:
          default: {
            snprintf(astConfFile[ii].pData, astConfFile[ii].iDataSize, "%s", pTok);
            break;
          }
        }
      }
    }
  }
  
  fclose(fpFile);
  fpFile = NULL;
  
  return 1;
}
