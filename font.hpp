#ifndef font_hpp
#define font_hpp

#include "Header.h"


void disp_text(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y);
void disp_text_center(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y);

#endif /* font_hpp */
