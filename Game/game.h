#pragma once

#include "../Helper/Header.h"
#include "../Helper/constants.h"
#include "../Helper/font.hpp"
#include "../Helper/menu.hpp"
#include "../Helper/sounds.h"

#include <chrono>
#include <thread>

class Bullet {
	public:
    SDL_Rect position;
    int dir;
	Bullet(int x, int y , int dir);
	std::pair<int,int> move(int s);
	void draw(SDL_Renderer *renderer, SDL_Texture *bullet );
};


class Player{
private:
	int time;
public:
	char* name;
	int player_no;
	int score;
	//int time;
	int xpos, ypos, old_xpos, old_ypos;
	int right, left, up, down;
	int width, height;
	bool attack;
	int attack_dir;
	int attack_counter;
	void setPosCenter(int i, int j);
	std::vector<Bullet> bullets;

	SDL_Texture *Tex;
	SDL_Texture *freezeTex;
	int playerId;
	int renderCycle;

	bool freeze;
    bool final_freeze;
	int freeze_counter;

	Player();
	Player(const Player &p);

	void init();

    void encode(int x[]);
    void decode(int y[]);
	void set_time(int t);
	int get_time();

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

	bool mode_chase;
	int dest;
	int chase_which_player;
	int chase_time;
	int not_chase_time;

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
	int to_go[MAZECOLS*MAZEROWS];
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
	Game(){}
	~Game(){}

	void init(SDL_Renderer *arg_renderer, TTF_Font *arg_font);
	void handleEvents();
	bool toQuit();
	void update();
	void render();
	void levelStart(int level, int seedi);
	void levelEnd();
	bool running() { return isRunning; }

	Player sPlayer, cPlayer;
	int askPlayerAvatar();

	bool isServer;
	bool isRunning, isLevelRunning;
	

private:

	int level;
	int seedi;

	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Event event;
	Sounds sounds;
	
	void updateTime();
	SDL_Texture *periTex;
	void renderPeriscope();
	void handlePlayerActivities(Player &p);

	void render_bullets();
	SDL_Texture *bullet;
	void Bullet_hit_Player();
	void updateBullets(Player &p);

	Monster monsters[MONSTERS];
	void initMonsters();
	void checkMonsterCollisions_Player(Player &p, bool playerIsServer);
	bool checkMonsterCollisions_Bullet(Bullet &b);
	bool checkoneMonsterCollisions(Player &p, Monster &m);
	void handleMonsterCollisions();
	void updateMonsters();

	MazeCell maze[MAZEROWS][MAZECOLS];
	SDL_Texture *mazeTex;
	void mazeInit();
	bool checkWallCollisions(int x, int y, int w, int h);
	void renderMaze();
	void maze_dist_update();
	void random_wall_removal();
    bool ok(int x, int y);
    void dfs(int x, int y);
    void maze_gen();
    int find_set(int v);
    void make_set(int v);
    bool union_sets(int a, int b);

	SDL_Texture *coinTex;
	void placeCoins();
	
	SDL_Texture *timeTex;
	void placeTimes();

	int coinCycle, timeCycle;
	void checkCoinTimeEat();
	void updateCoinTime(Player & p, MazeCell & m, bool playerIsServer = false);

	void loadTexture(char *textName, char *path);

	int gameTime;
};
