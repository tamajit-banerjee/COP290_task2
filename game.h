#pragma once

#define mazeCols 10
#define mazeRows 8
#define speed 2
#define LEVELS 4
#define MONSTERS 4
#define separator '|'
#define FREEZE_LIMIT 1000
#define COLLISION_AREA 1000

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
	int right, left, up, down;
	int width, height;

	SDL_Texture *Tex;

	bool freeze;
	int freeze_counter;

	Player();
	Player(const Player &p);

    void encode(int x[]);
    void decode(int y[]);

	void draw(SDL_Renderer *renderer, TTF_Font *font);
	void dispName(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos);
	void dispScore(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos);
	void dispTime(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos);

	void move(int s);
	void handleKeyDown(int key);
	void handleKeyUp(int key);
};

class Monster{
public:
	int xpos, ypos;
	int id;
	int right, left, up, down;
	int width, height;

	SDL_Texture *Tex;

	Monster();

	void draw(SDL_Renderer *renderer, TTF_Font *font);

	void move(int s);
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
	void removeWall(int direction);
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

	Monster monsters[MONSTERS];
	void checkMonsterCollisions(Player p);

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
