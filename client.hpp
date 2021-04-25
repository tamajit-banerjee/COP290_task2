#ifndef client_hpp
#define client_hpp

#include "Header.h"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"
#include "game.h"
#define  PORT "9987";

void run_client(SDL_Renderer *renderer,TTF_Font *font , Game *game);

#endif
