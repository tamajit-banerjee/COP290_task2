#pragma once

#define mazeCols 8
#define mazeRows 10

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

class MazeCell{
public:
	bool hascoin;
	bool hastime;

	int id; 
	// id denotes which type of cell it is. refer to walls_mapping.png for reference
	SDL_Rect srcR, dstR;
	MazeCell(){
		MazeCell(0);
	}
	MazeCell(int arg_id){
		update(arg_id);
	}

	void update(int arg_id){
		id = arg_id;
		
		srcR.w = 32;
		srcR.h = 32;
		srcR.x = srcR.w*(id%10);
		srcR.y = srcR.h*int(id/10);
	}
};

class Game
{
public:
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

	void renderMaze();
	MazeCell maze[mazeRows][mazeCols];
	SDL_Texture *mazeTex;
	void mazeInit();

	Game(){
	}

	~Game(){
	}

private:

	int cnt = 0;
	SDL_Window *window;
};