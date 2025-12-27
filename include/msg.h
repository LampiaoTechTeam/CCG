/**
 * msg.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *
 * Description: Messages of the game
 */

#ifndef _MSG_H_
#define _MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_MSGS 64

#define MSG(iID) pszGetMsgById(iID)

typedef struct STRUCT_MSG {
  int iID;
  char szText[1024];
} STRUCT_MSG, *PSTRUCT_MSG;

extern STRUCT_MSG gastMsg[MAX_MSGS];

char* pszGetMsgById(int iID);
int bLoadMsgXml(void);

#ifdef __cplusplus
}
#endif

#endif

