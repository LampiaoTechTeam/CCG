/**
 * cmdline.c
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in Dec 2025
 *
 * Description: Command line parser with getopt_long
 */

#include <cmdline.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys_interface.h>
#include <libgen.h>

void vPrintUsage(char* argv[], STRUCT_COMMANDLINE_OPTIONS astCmdOpt[]) {
  int ii = 0;
  printf(
    "Usage: %s --argument=<parameter>\n", basename(argv[0])
  );
  for ( ii = 0; astCmdOpt[ii].name; ii++ ) {
    if ( astCmdOpt[ii].has_arg == no_argument ) {
      printf("  -%c --%s\n", astCmdOpt[ii].val, astCmdOpt[ii].name);
    }
    else {
      printf("  -%c%s --%s=%s\n",
             astCmdOpt[ii].val, astCmdOpt[ii].pszArgumentExample,
             astCmdOpt[ii].name, astCmdOpt[ii].pszArgumentExample
      );
    }
    printf("    %s\n", astCmdOpt[ii].pszHelp);
  }
}

int bParseCommandLine(int argc, char* argv[], STRUCT_COMMANDLINE_OPTIONS astCmdOpt[]) {
  char chCh = 0;
  int iOptIdx = 0;
  struct option *astOptions;
  int iOptions = 0;
  int ii = 0;

  opterr = 0;

  for ( ii = 0; astCmdOpt[ii].name; ii++ ) iOptions++;

  astOptions = (struct option*) calloc(iOptions, sizeof(struct option));
  if ( !astOptions ) return 0;

  for ( ii = 0; astCmdOpt[ii].name; ii++ ) {
    astOptions[ii].name = astCmdOpt[ii].name;
    astOptions[ii].has_arg = astCmdOpt[ii].has_arg;
    astOptions[ii].flag = NULL;
    astOptions[ii].val = astCmdOpt[ii].val;
    if ( astCmdOpt[ii].has_arg == required_argument ) {
      switch (  astCmdOpt[ii].eType ) {
        case CMDLINETYPE_BOOL:
        case CMDLINETYPE_INT: {
          *(int*)astCmdOpt[ii].pData = atoi(astCmdOpt[ii].pszDefaultData);
          break;
        }
        case CMDLINETYPE_CHAR: {
          *(char*)astCmdOpt[ii].pData = astCmdOpt[ii].pszDefaultData[0];
          break;
        }
        case CMDLINETYPE_DOUBLE: {
          *(double*)astCmdOpt[ii].pData = atof(astCmdOpt[ii].pszDefaultData);
          break;
        }
        case CMDLINETYPE_STRING: {
          snprintf((char*)astCmdOpt[ii].pData, astCmdOpt[ii].iDataSize, "%s", astCmdOpt[ii].pszDefaultData);
          break;
        }
        case CMDLINETYPE_NULL:
        default: {
          break;
        }
      }
    }
  }

  while ( (chCh = getopt_long(argc, argv, gpszOptStr, astOptions, &iOptIdx)) != -1 ) {
    if ( chCh == '?' ) {
      free(astOptions);
      astOptions = NULL;
      return 0;
    }
    for ( ii = 0; astCmdOpt[ii].name; ii++ ) {
      if ( chCh == astCmdOpt[ii].val ) {
        iOptIdx = ii;
        break;
      }
    }
    if ( astCmdOpt[iOptIdx].bSet ) continue;
    astCmdOpt[iOptIdx].bSet = 1;
    if ( astCmdOpt[iOptIdx].has_arg == no_argument ) continue;
    if ( astCmdOpt[iOptIdx].has_arg == required_argument ) {
      if ( bStrIsEmpty(optarg) ) {
        if ( bStrIsEmpty(astCmdOpt[iOptIdx].pszDefaultData) ) {
          free(astOptions);
          astOptions = NULL;
          return 0;
        }
        optarg = astCmdOpt[iOptIdx].pszDefaultData;
      }
    }
    switch ( astCmdOpt[iOptIdx].eType ) {
      case CMDLINETYPE_BOOL:
      case CMDLINETYPE_INT: {
        *(int*)astCmdOpt[iOptIdx].pData = atoi(optarg);
        break;
      }
      case CMDLINETYPE_CHAR: {
        *(char*)astCmdOpt[iOptIdx].pData = optarg[0];
        break;
      }
      case CMDLINETYPE_DOUBLE: {
        *(double*)astCmdOpt[iOptIdx].pData = atof(optarg);
        break;
      }
      case CMDLINETYPE_STRING: {
        snprintf((char*)astCmdOpt[iOptIdx].pData, astCmdOpt[iOptIdx].iDataSize, "%s", optarg);
        break;
      }
      case CMDLINETYPE_NULL:
      default: {
        break;
      }
    }
  }

  free(astOptions);
  astOptions = NULL;

  return 1;
}
