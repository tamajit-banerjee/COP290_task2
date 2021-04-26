#include "menu.hpp"

void ask_for_name(SDL_Renderer *renderer, TTF_Font *font, char* name){
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
        char c[] = "Enter your name ";
        disp_text(renderer, c , font, 240, 200);
        disp_text(renderer, name ,  font, 240, 230);
        SDL_RenderPresent(renderer);
    }
}
