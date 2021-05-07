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
    width = 20;
    height = 20;
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



bool iscolliding(Player p, Monster m){
    if (p.xpos >= m.xpos + m.width || m.xpos >= p.xpos + p.width)
        return false;
    if (p.ypos >= m.ypos + m.height || m.ypos >= p.ypos + p.height)
        return false;

    return true;
    
}

void Game::checkMonsterCollisions(Player &p){
    for(int i = 0; i< MONSTERS; i++){
        // std::cout<<p.xpos<<p.ypos<<p.width<<p.height<<'\n';
        // std::cout<<monsters[i].xpos<<monsters[i].ypos<<monsters[i].width<<monsters[i].height<<'\n';
        SDL_Rect destR , srcR;
        destR.h = p.height;
        destR.w = p.width;
        destR.x = p.xpos;
        destR.y = p.ypos;
        srcR.h = monsters[i].height;
        srcR.w = monsters[i].width;
        srcR.x = monsters[i].xpos;
        srcR.y = monsters[i].ypos;
        SDL_Rect *d = &destR;
        SDL_Rect *b = &srcR;
        if(SDL_HasIntersection(d,b)){
            p.freeze_counter = 0;
            p.freeze = true;
        }
    }
}
