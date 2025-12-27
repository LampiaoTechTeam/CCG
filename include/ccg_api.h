#ifndef _CCG_API_H_
#define _CCG_API_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <game.h>

void vCCG_GetVariable(char* pszRsl, long int lRslSize, char* pszVar);
void vCCG_SetVariable(char* pszVar, char* pszValue);
int iCCG_GetStatus(void);
int iCCG_GetState(void);
void vCCG_SetStatus(int iStatus);
void vCCG_SetState(int iState);
int iCCG_GetLayout(void);
void vCCG_SetLayout(int iLayout);
void vCCG_Mensagem(char* pszMsg);

#endif

