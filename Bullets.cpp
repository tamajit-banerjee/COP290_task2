#include "game.h"

#define BULLET_WIDTH 8
#define BULLET_HEIGHT 8

Bullet:: Bullet(int x, int y, int direction ) {
    position.x = x;
    position.y = y;
    position.w = BULLET_WIDTH;
    position.h = BULLET_HEIGHT;    
    dir = direction;
}

std::pair<int,int> Bullet::move(int s){
    int new_x = position.x, new_y = position.y;
    switch (dir)
    {
    case 0 : 
        new_x -= s;
        break;
    case 1 : 
        new_x += s;
        break;
    case 2 : 
        new_y -= s;
        break;
    case 3 : 
        new_y += s;
        break;
    default:
        break;
    }
    return std::make_pair(new_x, new_y);
}


void Game:: updateBullets(Player &p){
    std::vector<Bullet> new_bullets;
    for(int i=0;i<p.bullets.size();i++){
       std::pair<int, int> pa =  p.bullets[i].move(SPEED*4);
    if(!checkWallCollisions(pa.first, pa.second, p.bullets[i].position.w, p.bullets[i].position.h)){
        p.bullets[i].position.x = pa.first; p.bullets[i].position.y = pa.second;
        new_bullets.push_back(p.bullets[i]);
    }
    }
    p.bullets =new_bullets;
}

void Game:: Bullet_hit_Player(){
    std::vector<Bullet> new_bullets;
    for(int i=0;i<sPlayer.bullets.size();i++){
         SDL_Rect destR;
        destR.h = cPlayer.height;
        destR.w = cPlayer.width;
        destR.x = cPlayer.xpos;
        destR.y = cPlayer.ypos;
        SDL_Rect *d = &destR;
        if(SDL_HasIntersection(d,&sPlayer.bullets[i].position)){
            cPlayer.freeze_counter = 0;
            cPlayer.freeze = true;
        }else{
            new_bullets.push_back(sPlayer.bullets[i]);
        }

    }
    sPlayer.bullets = new_bullets;
    new_bullets.clear();
    for(int i=0;i<cPlayer.bullets.size();i++){
        SDL_Rect destR;
        destR.h = sPlayer.height;
        destR.w = sPlayer.width;
        destR.x = sPlayer.xpos;
        destR.y = sPlayer.ypos;
        SDL_Rect *d = &destR;
        if(SDL_HasIntersection(d,&cPlayer.bullets[i].position)){
            sPlayer.freeze_counter = 0;
            sPlayer.freeze = true;
        }else{
            new_bullets.push_back(cPlayer.bullets[i]);
        }
    }
    cPlayer.bullets = new_bullets;
}

void Game::render_bullets(){
    for(int i=0;i<sPlayer.bullets.size();i++){
        sPlayer.bullets[i].draw(renderer,bullet);
    }
    for(int i=0;i<cPlayer.bullets.size();i++){
        cPlayer.bullets[i].draw(renderer,bullet);
    }
}


void Bullet::draw(SDL_Renderer *renderer, SDL_Texture *bullet ){

    if(SDL_RenderCopyEx(renderer, bullet,  NULL , &position, 0.0, NULL, SDL_FLIP_NONE) < 0){
        std::cout<<"Bullet not rendered properly\n";
        std::cout<<SDL_GetError()<<"\n";
        exit(EXIT_FAILURE);
    }
}