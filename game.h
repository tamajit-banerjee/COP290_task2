#pragma once

#define mazeCols 10
#define mazeRows 8
#define speed 2
#define separator '|'

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

	Player();
	Player(const Player &p);

	void encode(char * s, int size);
	void decode(char * s);

	void draw(SDL_Renderer *renderer, TTF_Font *font);
	void dispName(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos);
	void dispScore(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos);
	void dispTime(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos);
};

class MazeCell{
public:
	bool hascoin;
	bool hastime;
	int id; 
	// id denotes which type of cell it is. refer to walls_mapping.png for reference
	SDL_Rect srcR, dstR;
	MazeCell();
	MazeCell(int arg_id);

	void update(int arg_id);
	void removeWall(char *direction);
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

	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Event event;

	bool isRunning;
	bool isLevelRunning;
	int level;

	Player sPlayer, cPlayer;
	bool isServer;

	void renderMaze();
	MazeCell maze[mazeRows][mazeCols];
	SDL_Texture *mazeTex;
	void mazeInit();

	SDL_Texture *coinTex;
	int coinId;
	void placeCoins();
	void updateCoins();

	SDL_Texture *timeTex;
	void placeTimes();
	void updateTimes();

	void loadTexture(char *textName, char *path);

	Game(){}
	~Game(){}

	int counter;
};