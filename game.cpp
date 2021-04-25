#include "Header.h"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"

#include "Game.h"

Game::Game()
{}

Game::~Game()
{}

void Game::init(SDL_Renderer *arg_renderer, TTF_Font *arg_font )
{

	renderer = arg_renderer;
    font = arg_font;
    if (renderer)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        
    }

    isRunning = true;

    SDL_Surface* tmpSurface = SDL_LoadBMP("resources/player.bmp");
    cPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    sPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
}

void Game::handleEvents()
{
	
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update(){
    
    sPlayer.xpos++;
}

void Game::render(){
    SDL_RenderClear(renderer);
    char* s = "Sever Player: ";
    disp_text(renderer, s , font, 100, 200);
    disp_text(renderer, sPlayer.name , font, 250, 200);
    char* c = "client Player: ";
    disp_text(renderer, c , font, 100, 250);
    disp_text(renderer, cPlayer.name , font, 250, 250);

    SDL_Rect destR;
    destR.h = 128;
    destR.w = 128;
    destR.x = sPlayer.xpos;
    destR.y = sPlayer.ypos;
    SDL_RenderCopy(renderer, sPlayer.Tex,  NULL, &destR);

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
	// SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}


void Game::play(int level){
    while (running()) {
        handleEvents();
        update();
        render();
    }
    clean();
}

