#include "game.h"

Monster::Monster(){
    xpos = 0;
    ypos = 0;
    right = 0;
    left = 0;
    up = 0;
    down = 0;
    id = 0;
}


void Monster::draw(SDL_Renderer *renderer, TTF_Font *font){
    SDL_Rect destR;
    width = 10*(id+2);
    height = 10*(id+2);
    destR.h = height;
    destR.w = width;
    destR.x = xpos;
    destR.y = ypos;
    // SDL_RenderCopy(renderer, Tex,  NULL, &destR);
    if(SDL_RenderCopy(renderer, Tex,  NULL, &destR) < 0){
        std::cout<<"Monster not rendered properly\n";
        std::cout<<SDL_GetError()<<"\n";
        exit(EXIT_FAILURE);
    }
}

void Monster::move(int s){
    
    if(right)
        xpos+=s;
    if(left)
        xpos-=s;
    if(up)
        ypos-=s;
    if(down)
        ypos+=s;
    // std::cout<<"xpos: "<<xpos<<" ypos: "<<ypos<<'\n';
}

