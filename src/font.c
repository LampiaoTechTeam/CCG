/**
 * font.c
 *
 * Written by Renato Fermi <repizza@gmail.com>
 *
 * SDL2 TTF Handling
 *
 */

#ifdef USE_SDL2
#include <stdio.h>
#include <sys_interface.h>
#include <SDL2/SDL_ttf.h>
#include <font.h>

TTF_Font *ttfSDL_InitFont(void) {
  char szFontPath[_MAX_PATH] = "";
  memset(szFontPath, 0x00, sizeof(szFontPath));

  if (TTF_Init()) return NULL;
  sprintf(szFontPath, "%s/%s", PATH_TO_TTF, TTF_FONT_TITLE);
  return TTF_OpenFont(szFontPath, 16);
}

#endif

