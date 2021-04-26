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
            maze[int(cnt/mazeRows)][cnt%mazeRows].update(13);
            maze[int((cnt+1)/mazeRows)][(cnt+1)%mazeRows].update(11);
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

    SDL_Rect sdestR, cdestR;
    sdestR.h = 25;
    sdestR.w = 25;
    sdestR.x = sPlayer.xpos;
    sdestR.y = sPlayer.ypos;
    SDL_RenderCopy(renderer, sPlayer.Tex,  NULL, &sdestR);
    disp_text(renderer, sPlayer.name , font, sPlayer.xpos, sPlayer.ypos-20);
    cdestR.h = 25;
    cdestR.w = 25;
    cdestR.x = cPlayer.xpos;
    cdestR.y = cPlayer.ypos;
    SDL_RenderCopy(renderer, cPlayer.Tex,  NULL, &cdestR);
    disp_text(renderer, cPlayer.name , font, cPlayer.xpos, cPlayer.ypos-20);


    disp_text(renderer, sPlayer.name , font, 300, 20);
    disp_text(renderer, "score: " , font, 400, 20);
    std::string temp_str = std::to_string(sPlayer.score);
    char* char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, 450, 20);
    disp_text(renderer, "time: " , font, 500, 20);
    temp_str = std::to_string(sPlayer.time);
    char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, 550, 20);

    disp_text(renderer, cPlayer.name , font, 300, 40);
    disp_text(renderer, "score: " , font, 400, 40);
    temp_str = std::to_string(cPlayer.score);
    char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, 450, 40);
    disp_text(renderer, "time: " , font, 500, 40);
    temp_str = std::to_string(cPlayer.time);
    char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, 550, 40);


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