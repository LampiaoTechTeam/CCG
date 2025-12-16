#include <stdio.h>
#include <stdlib.h>
#include <debuff.h>
#include <trace.h>
#include <terminal_utils.h>
#include <deck.h>
#include <monster.h>
#include <player.h>
#include <game.h>

#ifdef LINUX
  #include <unistd.h>
#endif

int iGameSave(void) {
  FILE* fpGameDat = NULL;
  char szGameDatPath[512] = "";
  memset(szGameDatPath, 0x00, sizeof(szGameDatPath));
  snprintf(szGameDatPath, sizeof(szGameDatPath), "%s%cGAME.dat", gstGlobalPrm.szWrkDir, DIR_SEPARATOR);
  if ( (fpGameDat = fopen(szGameDatPath, "wb")) == NULL ) {
    return 0;
  }
  fwrite(&gstGame, sizeof(gstGame), 1, fpGameDat);
  fflush(fpGameDat);
  #ifdef LINUX
  fsync(fileno(fpGameDat));
  #endif
  fclose(fpGameDat);
  fpGameDat = NULL;
  return 1;
}

int iGameLoad(void) {
  FILE* fpGameDat = NULL;
  char szGameDatPath[512] = "";
  memset(szGameDatPath, 0x00, sizeof(szGameDatPath));
  snprintf(szGameDatPath, sizeof(szGameDatPath), "%s%cGAME.dat", gstGlobalPrm.szWrkDir, DIR_SEPARATOR);
  if ( (fpGameDat = fopen(szGameDatPath, "rb")) == NULL ) {
    return 0;
  }
  if (fread(&gstGame, sizeof(gstGame), 1, fpGameDat) != 1) {
    fclose(fpGameDat);
    return 0;
  }
  fclose(fpGameDat);
  fpGameDat = NULL;
  return 1;
}
