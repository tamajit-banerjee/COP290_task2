#include "Header.h"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"

#include <chrono>
#include <thread>

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
    cnt = 0;
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
        if(isServer){
            switch (event.key.keysym.sym){
                case SDLK_LEFT:
                    sPlayer.xpos -= speed;
                    break;
                case SDLK_RIGHT:
                    sPlayer.xpos += speed;
                    break;
                case SDLK_UP:
                    sPlayer.ypos -= speed;
                    break;
                case SDLK_DOWN:
                    sPlayer.ypos += speed;
                    break;
                default:
                    break;
            }
        }
        else{
            switch (event.key.keysym.sym){
                case SDLK_LEFT:
                    cPlayer.xpos -= 1;
                    break;
                case SDLK_RIGHT:
                    cPlayer.xpos += 1;
                    break;
                case SDLK_UP:
                    cPlayer.ypos -= 1;
                    break;
                case SDLK_DOWN:
                    cPlayer.ypos += 1;
                    break;
                default:
                    break;
            }
        }
	default:
		break;
	}
}

void Game::update(){

    cnt++;
    
    if(cnt<mazeRows*mazeCols){
        if(maze[int(cnt/mazeRows)][cnt%mazeRows].id == 0 && maze[int((cnt+1)/mazeRows)][(cnt+1)%mazeRows].id == 0){
            maze[int(cnt/mazeRows)][cnt%mazeRows].update(13);
            maze[int((cnt+1)/mazeRows)][(cnt+1)%mazeRows].update(11);
        }
        
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    
}

void Game::render(){
    SDL_RenderClear(renderer);

    renderMaze();

    SDL_Rect sdestR, cdestR;
    sdestR.h = 25;
    sdestR.w = 25;
    sdestR.x = sPlayer.xpos;
    sdestR.y = sPlayer.ypos;
    SDL_RenderCopy(renderer, sPlayer.Tex,  NULL, &sdestR);
    cdestR.h = 25;
    cdestR.w = 25;
    cdestR.x = cPlayer.xpos;
    cdestR.y = cPlayer.ypos;
    SDL_RenderCopy(renderer, cPlayer.Tex,  NULL, &cdestR);

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
            maze[i][j].update(0);
        }
    }
}