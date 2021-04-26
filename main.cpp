#include "Header.h"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"
#include "game.h"

int main(){

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font *font;
    font = TTF_OpenFont("resources/m5x7.ttf", 24);
    window = SDL_CreateWindow(
            "game",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            0);

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    char name[100];
    ask_for_name(renderer, font, name);

    Game *game = new Game();
    game->sPlayer.name = name;
    game->init(renderer, font);
    game->isServer = true;
    for (int level = 1; level<2; level++){

        while (game->running()) {
            
            game->handleEvents();
            game->update();
            game->render();

            if(game->sPlayer.time<=0){
                game->isRunning = false;
            }
        }
        game->clean();
    }

    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
