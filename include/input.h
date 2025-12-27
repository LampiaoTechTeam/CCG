#ifndef INPUT_H
  #define INPUT_H

  #define ACTION_PLAY       'p'
 
  int iPortableGetchar();
  void vShowInitDialog();
  void vSleepSeconds(int iSeconds);
  void vClearTerminal();
  void vFlushInput();
  char *cCatchInput();
  void vReadPlayerName(char *pszName, int iNameSz);
  int iReadOptions();
  void vReadCardName(char *pszName, int iNameSz);
  
#endif
