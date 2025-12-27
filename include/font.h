/**
 * font.h
 *
 * Written by Renato Fermi <repizza@gmail.com>
 * 
 * SDL2 TTF Handling 
 * 
 */
#ifndef _FONT_H_
#define _FONT_H_
  #ifdef USE_SDL2
    #define PATH_TO_TTF "fonts"
    #define TTF_FONT_TITLE "FiraCode.ttf"

    TTF_Font *ttfSDL_InitFont(void);

  #endif
#endif /* _FONT_H_ */
