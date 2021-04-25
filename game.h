#pragma once

#include "Header.h"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"

class Player{
public:
	char* name;
	int score;
	int time;
	int xpos, ypos;

	SDL_Texture *Tex;

	Player(){
		name = "Not Entered yet!";
		xpos = 0;
		ypos = 0;
	}
};


class Game
{
public:
	Game();
	~Game();

	void init(SDL_Renderer *arg_renderer, TTF_Font *arg_font);

	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();
	void play(int level);

	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Event event;
	bool isRunning;
	int level;

	Player sPlayer, cPlayer;

private:

	int cnt = 0;
	SDL_Window *window;
};