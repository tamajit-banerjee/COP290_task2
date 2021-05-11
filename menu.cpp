#include "menu.hpp"
#include "game.h"


void server_or_client(SDL_Renderer *renderer, char *menu, TTF_Font *font){
    SDL_Event e;
    int pressed = false;
    while (!pressed) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_c) {
                    *menu = 'c';
                    pressed = true;
                } else if (e.key.keysym.sym == SDLK_s) {
                    *menu = 's';
                    pressed = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(renderer);
        char c[] = "[s]erver or [c]lient?";
        disp_text(renderer, c , font, int(SCREEN_WIDTH/2) - 70, int(SCREEN_HEIGHT/2));
        SDL_RenderPresent(renderer);
    }
}


void ask_for_ip(SDL_Renderer *renderer, TTF_Font *font, char* ip) {
    memset(ip, ' ', 15);
    SDL_Event e;
    int position = 0;
    int ok = false;
    while (!ok) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if ((e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) || e.key.keysym.sym == SDLK_PERIOD) {
                    if (position > 14) {
                        position = 14;
                    }
                    ip[position] = e.key.keysym.sym;
                    position++;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    position--;
                    if (position < 0) {
                        position = 0;
                    }
                    ip[position] = ' ';
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    ip[position] = 0;
                    ok = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(renderer);
        char c[] = "ip adress";
        disp_text(renderer, c , font, int(SCREEN_WIDTH/2) - 70, int(SCREEN_HEIGHT/2));
        disp_text(renderer, ip ,  font, int(SCREEN_WIDTH/2) - 70, int(SCREEN_HEIGHT/2));
        SDL_RenderPresent(renderer);
    }
}


void ask_for_name(SDL_Renderer *renderer, TTF_Font *font, char* name, bool server){
    memset(name, ' ', 15);
    SDL_Event e;
    int position = 0;
    int ok = false;
    while (!ok) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if ((e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z) || e.key.keysym.sym == SDLK_PERIOD) {
                    if (position > 14) {
                        position = 14;
                    }
                    name[position] = e.key.keysym.sym;
                    position++;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    position--;
                    if (position < 0) {
                        position = 0;
                    }
                    name[position] = ' ';
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    name[position] = 0;
                    ok = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(renderer);
        if(server)
            // disp_text(renderer, "Player 1" , font, int(SCREEN_WIDTH/2) - 60, int(SCREEN_HEIGHT/2)-45);
            disp_text_center(renderer, "Player 1" , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2)-45);
        else    
            // disp_text(renderer, "Player 2" , font, int(SCREEN_WIDTH/2) - 60, int(SCREEN_HEIGHT/2)-45);
            disp_text_center(renderer, "Player 2" , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2)-45);
        char c[] = "Enter your name ";
        // disp_text(renderer, c , font, int(SCREEN_WIDTH/2) - 100, int(SCREEN_HEIGHT/2)-20);
        disp_text_center(renderer, c , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2)-20);
        // disp_text(renderer, name ,  font, int(SCREEN_WIDTH/2) - 100, int(SCREEN_HEIGHT/2)+20);
        disp_text_center(renderer, name , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2)+20);
        SDL_RenderPresent(renderer);
    }
}
