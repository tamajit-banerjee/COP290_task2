//
//  menu.hpp
//  Mustam-Game
//
//  Created by Tamajit Banerjee on 25/04/21.
//

#ifndef menu_hpp
#define menu_hpp


#include "Header.h"
#include <unistd.h>
#include "font.hpp"
#include "constans.h"

void server_or_client(SDL_Renderer *renderer, char *menu, TTF_Font *font);
void ask_for_ip(SDL_Renderer *renderer, TTF_Font *font, char* ip);
void ask_for_name(SDL_Renderer *renderer, TTF_Font *font, char* name);

#endif /* menu_hpp */
