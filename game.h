#pragma once

#define CELL_SIZE 64
#define MAZECOLS 10
#define MAZEROWS 10

#define SCREEN_WIDTH (MAZECOLS * CELL_SIZE)
#define SCREEN_HEIGHT (MAZECOLS * CELL_SIZE)

#define COIN_SIZE (CELL_SIZE / 2)
#define COIN_SCORE 10

#define TIME_SIZE (COIN_SIZE)
#define TIME_INCREASE 100

#define SPEED 2
#define LEVELS 4

#define MONSTERS 4
#define MONSTERS_DIR_CHANGE 30
#define MONSTER_SIZE 131
#define MONSTER_DELAY 8

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
	int xpos, ypos, old_xpos, old_ypos;
	int right, left, up, down;
	int width, height;
	void setPosCenter(int i, int j);

	SDL_Texture *Tex;
	int playerId;
	int renderCycle;

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
	int old_xpos, old_ypos;
	int id;
	int right, left, up, down;
	int width, height;
	int renderCycle;

	SDL_Texture *Tex;

	int changeDirectionCounter;

	Monster();

	void draw(SDL_Renderer *renderer, TTF_Font *font);
	void setPosCenter(int i, int j);

	void move(int s);
	std::pair<int, int> getMazeCoordinates(SDL_Rect & r);
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
	void levelStart(int level);
	void levelEnd();

	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Event event;

	bool isRunning;
	bool isLevelRunning;
	int level;

	Player sPlayer, cPlayer;
	bool isServer;

	Monster monsters[MONSTERS];
	void initMonsters();
	void checkMonsterCollisions(Player &p);
	void handleMonsterCollisions();
	void updateMonsters();

	void renderMaze();
    bool ok(int x, int y);
    void dfs(int x, int y);
    void maze_gen();
    int find_set(int v);
    void make_set(int v);
    bool union_sets(int a, int b);

	MazeCell maze[MAZEROWS][MAZECOLS];
	SDL_Texture *mazeTex;
	void mazeInit();
	bool checkWallCollisions(int x, int y, int w, int h);

	SDL_Texture *coinTex;
	int coinCycle, timeCycle;
	void placeCoins();
	void checkCoinTimeEat();
	void updateCoinTime(Player & p, MazeCell & m);

	SDL_Texture *timeTex;
	void placeTimes();

	void loadTexture(char *textName, char *path);

	Game(){}
	~Game(){}

	int counter;
};
