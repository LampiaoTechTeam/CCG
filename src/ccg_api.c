#include <ccg_api.h>

void vCCG_GetVariable(char* pszRsl, long int lRslSize, char* pszVar) {
  printf("EVALUATE {%s}\n", pszVar);
  fgets(pszRsl, lRslSize, stdin);
  fflush(stdout);
}

void vCCG_SetVariable(char* pszVar, char* pszValue) {
  printf("%s=%s\n", pszVar, pszValue);
  fflush(stdout);
}

int iCCG_GetStatus(void) {
  char szRsl[32] = "";
  memset(szRsl, 0x00, sizeof(szRsl));
  vCCG_GetVariable(szRsl, sizeof(szRsl), "STATUS");
  return atoi(szRsl);
}

int iCCG_GetState(void) {
  char szRsl[32] = "";
  memset(szRsl, 0x00, sizeof(szRsl));
  vCCG_GetVariable(szRsl, sizeof(szRsl), "STATE");
  return atoi(szRsl);
}

void vCCG_SetStatus(int iStatus) {
  char szValue[32] = "";
  memset(szValue, 0x00, sizeof(szValue));
  snprintf(szValue, sizeof(szValue), "%d", iStatus);
  vCCG_SetVariable("STATUS", szValue);
}

void vCCG_SetState(int iState) {
  char szValue[32] = "";
  memset(szValue, 0x00, sizeof(szValue));
  snprintf(szValue, sizeof(szValue), "%d", iState);
  vCCG_SetVariable("STATE", szValue);
}

int iCCG_GetLayout(void) {
  char szRsl[32] = "";
  memset(szRsl, 0x00, sizeof(szRsl));
  vCCG_GetVariable(szRsl, sizeof(szRsl), "LAYOUT");
  return atoi(szRsl);
}

void vCCG_SetLayout(int iLayout) {
  char szValue[32] = "";
  memset(szValue, 0x00, sizeof(szValue));
  snprintf(szValue, sizeof(szValue), "%d", iLayout);
  vCCG_SetVariable("LAYOUT", szValue);
}

void vCCG_Mensagem(char* pszMsg) {
  printf("MENSAGEM %s\n", pszMsg);
  fflush(stdout);
}
