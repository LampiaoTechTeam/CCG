#ifndef _CMDLINE_H_
#define _CMDLINE_H_

typedef enum ENUM_CMDLINE_DATA_TYPE {
  CMDLINETYPE_BOOL,
  CMDLINETYPE_INT,
  CMDLINETYPE_CHAR,
  CMDLINETYPE_STRING,
  CMDLINETYPE_DOUBLE,
  CMDLINETYPE_NULL
} ENUM_CMDLINE_DATA_TYPE, *PENUM_CMDLINE_DATA_TYPE;

typedef struct STRUCT_COMMANDLINE_OPTIONS {
  const char* name; // long
  int val; // short
  int has_arg;
  ENUM_CMDLINE_DATA_TYPE eType;
  char* pszArgumentExample;
  int bSet;
  char* pszDefaultData;
  void* pData;
  int iDataSize;
  char* pszHelp;
} STRUCT_COMMANDLINE_OPTIONS, *PSTRUCT_COMMANDLINE_OPTIONS;

extern char* gpszOptStr;

void vPrintUsage(char* argv[], STRUCT_COMMANDLINE_OPTIONS astCmdOpt[]);
int bParseCommandLine(int argc, char* argv[], STRUCT_COMMANDLINE_OPTIONS astCmdOpt[]);

#endif
