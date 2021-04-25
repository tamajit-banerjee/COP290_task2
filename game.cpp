#include "Header.h"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"

#include "Game.h"

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

    SDL_Surface* mazeTmpSurface = SDL_LoadBMP("resources/maze.bmp");
    mazeTex = SDL_CreateTextureFromSurface(renderer, mazeTmpSurface);
    SDL_FreeSurface(mazeTmpSurface);
    mazeInit();
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
    cnt++;
    
    
}

void Game::render(){
    SDL_RenderClear(renderer);

    renderMaze();

    char* s = "Sever Player: ";
    disp_text(renderer, s , font, 100, 200);
    disp_text(renderer, sPlayer.name , font, 250, 200);
    char* c = "client Player: ";
    disp_text(renderer, c , font, 100, 250);
    disp_text(renderer, cPlayer.name , font, 250, 250);

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

void Game::renderMaze(){
    int cell_width = 64;
    int cell_height = 64;
    for(int i =0; i<mazeRows; i++){
        for(int j = 0; j<mazeCols; j++){
            SDL_Rect dstR;
            dstR.w = cell_width;
            dstR.h = cell_height;
            dstR.x = dstR.w * i;
            dstR.y = dstR.h * j;
            if(SDL_RenderCopyEx(renderer, mazeTex,  &maze[i][j].srcR, &dstR, 0.0, NULL, SDL_FLIP_NONE) < 0){
                std::cout<<"Maze not rendered properly\n";
                std::cout<<SDL_GetError()<<"\n";
                exit(EXIT_FAILURE);
            }
        }
    }
}

void Game::mazeInit(){
    for(int i =0; i<mazeRows; i++){
        for(int j = 0; j<mazeCols; j++){
            maze[i][j].update((i+j)%15);
        }
    }
}