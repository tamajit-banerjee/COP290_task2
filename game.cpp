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

    loadTexture("player", "resources/player.bmp");
    loadTexture("maze", "resources/maze.bmp");

    mazeInit();
    
    cnt = 0;

    sPlayer.xpos = 80;
    sPlayer.ypos = 240;
    cPlayer.xpos = 500;
    cPlayer.ypos = 240;
    sPlayer.time = 700;
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
                    cPlayer.xpos -= speed;
                    break;
                case SDLK_RIGHT:
                    cPlayer.xpos += speed;
                    break;
                case SDLK_UP:
                    cPlayer.ypos -= speed;
                    break;
                case SDLK_DOWN:
                    cPlayer.ypos += speed;
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
            maze[int(cnt/mazeRows)][cnt%mazeRows].removeWall("right");
            maze[int((cnt+1)/mazeRows)][(cnt+1)%mazeRows].removeWall("left");
        }
        
    }
    if(sPlayer.time>0)
        sPlayer.time -= 1;
    if(cPlayer.time>0)
        cPlayer.time -= 1;
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

void Game::renderMaze(){
    int cell_width = 64;
    int cell_height = 64;
    for(int i =0; i<mazeRows; i++){
        for(int j = 0; j<mazeCols; j++){
            SDL_Rect dstR;
            dstR.w = cell_width;
            dstR.h = cell_height;
            dstR.x = dstR.w * j;
            dstR.y = dstR.h * i;
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

void Game::loadTexture(char *textName, char *path){
    SDL_Surface* tmpSurface;
    if(strcmp(textName, "player") == 0){
        tmpSurface = SDL_LoadBMP(path);
        cPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        sPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "maze") == 0){
        tmpSurface = SDL_LoadBMP(path);
        mazeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }

}