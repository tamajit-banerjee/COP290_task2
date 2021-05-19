#ifndef menu_hpp
#define menu_hpp

#include <unistd.h>
#include "game.h"

void server_or_client(SDL_Renderer *renderer, char *menu, TTF_Font *font);
void ask_for_ip(SDL_Renderer *renderer, TTF_Font *font, char* ip);
int ask_for_name(SDL_Renderer *renderer, TTF_Font *font, char* name, bool server = true);

#endif /* menu_hpp */
