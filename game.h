#pragma once

#define mazeCols 8
#define mazeRows 10
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

	Player(){
		name = "Not Entered yet!";
		xpos = 0;
		ypos = 0;
		score = 0;
		time = 500;
	}

	Player(const Player &p){
		name = p.name;
		xpos = p.xpos;
		ypos = p.ypos;
		score = p.score;
		time = p.time;
	}

	void encode(char * s, int size){
		int i = 0;
		while(i<sizeof(name)/sizeof(name[0])){
			s[i] = name[i];
			i++;
		}
		s[i] = separator; i++;
		int j = 0;
		while(j<(std::to_string(xpos).length())){
			s[i] = std::to_string(xpos)[j];
			i++; j++;
		}
		j = 0;
		s[i] = separator; i++;
		while(j<(std::to_string(ypos).length())){
			s[i] = std::to_string(ypos)[j];
			i++; j++;
		}
		j = 0;
		s[i] = separator; i++;
		while(j<(std::to_string(score).length())){
			s[i] = std::to_string(score)[j];
			i++; j++;
		}
		j = 0;
		s[i] = separator; i++;
		while(j<(std::to_string(time).length())){
			s[i] = std::to_string(time)[j];
			i++; j++;
		}
		while(i<size){
			s[i] = ' ';
			i++;
		}
	}

	void decode(char * s){
		int i = 0;
		char nametmp[100];
		while(s[i]!=separator){
			nametmp[i] = s[i];
			i++;
		}
		name = nametmp;
		int j = 0;
		char xpostmp[100];
		while(s[i]!=separator){
			xpostmp[j] = s[i];
			i++; j++;
		}
		xpos = atoi(xpostmp);
		j = 0;
		char ypostmp[100];
		while(s[i]!=separator){
			ypostmp[j] = s[i];
			i++; j++;
		}
		ypos = atoi(ypostmp);
		j = 0;
		char scoretmp[100];
		while(s[i]!=separator){
			scoretmp[j] = s[i];
			i++; j++;
		}
		score = atoi(scoretmp);
		j = 0;
		char timetmp[100];
		while(s[i]!=separator){
			timetmp[j] = s[i];
			i++; j++;
		}
		time = atoi(timetmp);
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
	bool isServer;

	void renderMaze();
	MazeCell maze[mazeRows][mazeCols];
	SDL_Texture *mazeTex;
	void mazeInit();

	Game(){
		sPlayer.xpos = 80;
		sPlayer.ypos = 240;

		cPlayer.xpos = 500;
		cPlayer.ypos = 240;
	}

	~Game(){
	}

private:

	int cnt = 0;
	SDL_Window *window;
};