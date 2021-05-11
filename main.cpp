#include "Header.h"
#include "client.hpp"
#include "server.hpp"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"
#include "game.h"

int main(){
    // char menu = 's';
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
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
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
    
    Game *game = new Game();
    game->init(renderer, font);

    // server_or_client(renderer, &menu, font);
    // if (menu == 'c') {
    //     run_client(renderer,font, game);
    // }
    // if(menu == 's') {
    //     run_server(renderer,font, game);
    // }
    
    if(run_server(renderer,font, game) == -1){
        run_client(renderer,font, game);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
