#include <ccg_api.h>
#include <strings.h>

typedef struct STRUCT_MY_COMMAND_LINE {
  char szTrace[256];
  char szDebug[32];
  char szCommand[32];
  char szLayout[32];
} STRUCT_MY_COMMAND_LINE;

STRUCT_MY_COMMAND_LINE gstCmdLine;

STRUCT_GAME gstGame;

int iLimparVariaveis(void) {
  gstGame.iStatus = iCCG_GetStatus();
  gstGame.iState = iCCG_GetState();

  if ( gstGame.iStatus == STATUS_RUN &&
        (gstGame.iState == STATE_YOU_WIN ||
         gstGame.iState == STATE_YOU_LOSE) ) {
    char szGameFilePath[512] = "";
    memset(szGameFilePath, 0x00, sizeof(szGameFilePath));
    remove(szGameFilePath);
  }

  return 0;
}

int iExibirMensagem(void) {
  int iRsl = 0;
  int iLayout = iCCG_GetLayout();

  gstGame.iStatus = iCCG_GetStatus();
  gstGame.iState = iCCG_GetState();

  /* Nada a fazer */
  if ( gstGame.iStatus == STATUS_PAUSE ) {
    return iRsl;
  }

  if ( iLayout == LAYOUT_GAME_OVER ) {
    vCCG_Mensagem("Voce perdeu");
  }

  return iRsl;
}

int main(int iArgc, char** ppszArgv) {
  int iRsl = 0;

       if ( !strcasecmp(gstCmdLine.szCommand, "limpar-variaveis") ) iRsl = iLimparVariaveis();
  else if ( !strcasecmp(gstCmdLine.szCommand, "exibir-mensagem" ) ) iRsl = iExibirMensagem ();
  else {
    printf("Error: %s --command=%s not found!\n", ppszArgv[0], gstCmdLine.szCommand);
    iRsl = -1;
  }
  return iRsl;
}

