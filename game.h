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
		srcR.w = 32;
		srcR.h = 32;
	}
	MazeCell(int arg_id){
		update(arg_id);
		srcR.w = 32;
		srcR.h = 32;
	}

	void update(int arg_id){
		id = arg_id;
		
		srcR.x = srcR.w*(id%10);
		srcR.y = srcR.h*int(id/10);
	}

	void removeWall(char *direction){
		if(strcmp(direction, "top") == 0){
			switch(id){
				case 0:
					update(11);break;
				case 2:
					update(15);break;
				case 4:
					update(10);break;
				case 6:
					update(1);break;
				case 9:
					update(3);break;
				case 12:
					update(7);break;
				case 13:
					update(5);break;
				case 14:
					update(8);break;
				default:
					std::cout<<"Cell with id "<<id<<" has no top wall\n";
					exit(EXIT_FAILURE);
					break;
			}
		}
		else if(strcmp(direction, "bottom") == 0){
			switch(id){
				case 0:
					update(13);break;
				case 4:
					update(2);break;
				case 7:
					update(1);break;
				case 8:
					update(3);break;
				case 10:
					update(15);break;
				case 11:
					update(5);break;
				case 12:
					update(6);break;
				case 14:
					update(9);break;
				default:
					std::cout<<"Cell with id "<<id<<" has no bottom wall\n";
					exit(EXIT_FAILURE);
					break;
			}
		}
		else if(strcmp(direction, "right") == 0){
			switch(id){
				case 0:
					update(12);break;
				case 3:
					update(15);break;
				case 5:
					update(1);break;
				case 8:
					update(10);break;
				case 9:
					update(2);break;
				case 11:
					update(7);break;
				case 13:
					update(6);break;
				case 14:
					update(4);break;
				default:
					std::cout<<"Cell with id "<<id<<" has no right wall\n";
					exit(EXIT_FAILURE);
					break;
			}
		}
		else if(strcmp(direction, "left") == 0){
			switch(id){
				case 0:
					update(14);break;
				case 1:
					update(15);break;
				case 5:
					update(3);break;
				case 6:
					update(2);break;
				case 7:
					update(10);break;
				case 11:
					update(8);break;
				case 12:
					update(4);break;
				case 13:
					update(9);break;
				default:
					std::cout<<"Cell with id "<<id<<" has no left wall\n";
					exit(EXIT_FAILURE);
					break;
			}
		}
		else{
			std::cout<<"Invalid use of removeWall(). Please use top, bottom, left or right as argument!\n";
			exit(EXIT_FAILURE);
		}
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
	bool isLevelRunning;
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

};