#ifndef server_hpp
#define server_hpp

#include "Header.h"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"
#include "game.h"
#define  PORT "9987";
void run_server(SDL_Renderer *renderer,TTF_Font *font , Game *game);

#endif /* server_hpp */
