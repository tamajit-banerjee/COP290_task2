#pragma once

#define CELL_SIZE 64
#define MAZECOLS 10
#define MAZEROWS 10

#define SCREEN_WIDTH (MAZECOLS * CELL_SIZE)
#define SCREEN_HEIGHT (MAZECOLS * CELL_SIZE)

#define COIN_SIZE (CELL_SIZE / 2)
#define COIN_SCORE 10

#define SPEED 2
#define LEVELS 4
#define MONSTERS 4
#define SEPARATOR '|'
#define FREEZE_LIMIT 20
#define WALL_RATIO 8



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

	std::pair<int, int> move(int s);
	void handleKeyDown(int key);
	void handleKeyUp(int key);
	std::pair<int, int> getMazeCoordinates(SDL_Rect & r);
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
	void checkMonsterCollisions(Player &p);

	void renderMaze();
    bool ok(int x, int y);
    void dfs(int x, int y);
    void maze_gen();
	MazeCell maze[MAZEROWS][MAZECOLS];
	SDL_Texture *mazeTex;
	void mazeInit();
	bool checkWallCollisions(int x, int y, int w, int h);

	SDL_Texture *coinTex;
	int coinId;
	void placeCoins();
	void updateCoins();
	void checkCoinEat();

	SDL_Texture *timeTex;
	void placeTimes();
	void updateTimes();

	void loadTexture(char *textName, char *path);

	Game(){}
	~Game(){}

	int counter;
};
