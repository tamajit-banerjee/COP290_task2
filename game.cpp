#include "Header.h"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"

#include <chrono>
#include <thread>

#include "game.h"

void Game::init(SDL_Renderer *arg_renderer, TTF_Font *arg_font )
{

	renderer = arg_renderer;
    font = arg_font;

    isRunning = true;
    isLevelRunning = true;

    loadTexture("player", "resources/players_combined.bmp");
    loadTexture("monster", "resources/monster.bmp");
    loadTexture("maze", "resources/maze.bmp");
    loadTexture("coin", "resources/coins.bmp");
    loadTexture("time", "resources/time.bmp");

    mazeInit();
    
    counter = 0;

    sPlayer.xpos = 144;
    sPlayer.ypos = 16;
    cPlayer.xpos = 16;
    cPlayer.ypos = 16;
    sPlayer.time = 700;

    sPlayer.playerId = 1;
    cPlayer.playerId = 4;

    for(int i = 0 ; i<MONSTERS; i++){
        monsters[i].id = i;
        monsters[i].xpos = rand()%SCREEN_WIDTH/2;
        monsters[i].ypos = rand()%SCREEN_WIDTH/2;
    }
        
}

void Game::handleEvents()
{
	
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE)
            isRunning = false;
        if(isServer)
            sPlayer.handleKeyDown(event.key.keysym.sym);
        else
            cPlayer.handleKeyDown(event.key.keysym.sym);
        break;
    case SDL_KEYUP:
        if(isServer)
            sPlayer.handleKeyUp(event.key.keysym.sym);
        else
            cPlayer.handleKeyUp(event.key.keysym.sym);
        break;
	default:
		break;
	}
}

void Game::update(){

    for(int i = 0 ; i<MONSTERS; i++){
        int j = (counter) % 100;
        monsters[i].left = (j < 50 ) ? 0 : 1;
        monsters[i].right = (j >= 50) ? 0 : 1;
        monsters[i].up = (j < 50) ? 0 : 1;
        monsters[i].down = (j >= 50) ? 0 : 1;
        monsters[i].move(SPEED);
    }

    counter++;

    checkMonsterCollisions(sPlayer);
    checkMonsterCollisions(cPlayer);

    sPlayer.freeze_counter++;
    cPlayer.freeze_counter++;
    
    if(sPlayer.freeze){
        if (sPlayer.freeze_counter >= FREEZE_LIMIT)
            sPlayer.freeze = false;
    }
    cPlayer.freeze_counter++;
    if(cPlayer.freeze){
        if (cPlayer.freeze_counter >= FREEZE_LIMIT)
            cPlayer.freeze = false;
    }


    std::pair<int, int> s_p = sPlayer.move(SPEED);
    std::pair<int, int> c_p = cPlayer.move(SPEED);
    if(!checkWallCollisions(s_p.first, s_p.second, sPlayer.width, sPlayer.height)){
        sPlayer.xpos = s_p.first;
        sPlayer.ypos = s_p.second;
    }

    if(!checkWallCollisions(c_p.first, c_p.second, cPlayer.width, cPlayer.height)){
        cPlayer.xpos = c_p.first;
        cPlayer.ypos = c_p.second;
    }

    checkCoinTimeEat();

    if(sPlayer.time>0)
        sPlayer.time -= 1;
    if(cPlayer.time>0)
        cPlayer.time -= 1;

    coinId++;
    if(coinId==80){
        coinId = 0;
    }
    updateCoins();

    // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
}

void Game::render(){
    SDL_RenderClear(renderer);

    renderMaze();

    sPlayer.draw(renderer, font);
    cPlayer.draw(renderer, font);


    sPlayer.dispName(renderer, font, 300, 20);
    sPlayer.dispScore(renderer, font, 400, 20);
    sPlayer.dispTime(renderer, font, 500, 20);

    cPlayer.dispName(renderer, font, 300, 40);
    cPlayer.dispScore(renderer, font, 400, 40);
    cPlayer.dispTime(renderer, font, 500, 40);

    for(int i = 0 ; i<MONSTERS; i++){
        monsters[i].draw(renderer, font);
    }

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    disp_text(renderer, "Results", font, 300, 140);
    
    sPlayer.dispName(renderer, font, 250, 200);
    sPlayer.dispScore(renderer, font, 350, 200);

    cPlayer.dispName(renderer, font, 250, 250);
    cPlayer.dispScore(renderer, font, 350, 250);

    SDL_RenderPresent(renderer);
}





void Game::loadTexture(char *textName, char *path){
    SDL_Surface* tmpSurface;
    if(strcmp(textName, "player") == 0){
        tmpSurface = SDL_LoadBMP(path);
        cPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        sPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    if(strcmp(textName, "monster") == 0){
        tmpSurface = SDL_LoadBMP(path);
        for(int i = 0; i<MONSTERS; i++){
            monsters[i].Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        }
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "maze") == 0){
        tmpSurface = SDL_LoadBMP(path);
        mazeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "coin") == 0){
        tmpSurface = SDL_LoadBMP(path);
        coinTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "time") == 0){
        tmpSurface = SDL_LoadBMP(path);
        timeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }

}
