#include "font.hpp"

void disp_text(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y) {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect pos;
    SDL_Color color = {255, 255, 255};
    surface = TTF_RenderText_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    pos.w = surface->w;
    pos.h = surface->h;
    pos.x = x;
    pos.y = y + SCORE_DISPLAY_HEIGHT;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &pos);
}

void disp_text_center(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y){
    int l = std::strlen(text);
    disp_text(renderer, text, font, x - 9*l/2, y);
}