#include "game.h"

Player::Player(){
    name = "Not Entered yet!";
    xpos = 0;
    ypos = 0;
    score = 0;
    time = 500;
    right = 0;
    left = 0;
    up = 0;
    down = 0;
}

Player::Player(const Player &p){
    name = p.name;
    xpos = p.xpos;
    ypos = p.ypos;
    score = p.score;
    time = p.time;
    right = 0;
    left = 0;
    up = 0;
    down = 0;
}

void Player::encode(int x[]){
    x[0] = xpos;
    x[1] = ypos;
    x[2] = score;
    x[3] = time;
    //x = xpos*1000 + ypos;
}

void Player::decode(int y[]){
//    xpos = y/1000;
//    ypos = y%1000;
    xpos = y[0];
    ypos = y[1];
    score = y[2];
    time = y[3];
}

void Player::draw(SDL_Renderer *renderer, TTF_Font *font){
    SDL_Rect destR;
    width = 25;
    height = 25;
    destR.h = height;
    destR.w = width;
    destR.x = xpos;
    destR.y = ypos;
    SDL_RenderCopy(renderer, Tex,  NULL, &destR);
    disp_text(renderer, name , font, xpos, ypos-20);
}

void Player::dispName(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, name, font, xpos, ypos);
}
void Player::dispScore(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, "Score: ", font, xpos, ypos);
    std::string temp_str = std::to_string(score);
    char* char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, xpos+50, ypos);
}
void Player::dispTime(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, "Score: ", font, xpos, ypos);
    std::string temp_str = std::to_string(time);
    char* char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, xpos+50, ypos);
}

void Player::move(int s){
    if(!freeze){
        if(right)
            xpos+=s;
        if(left)
            xpos-=s;
        if(up)
            ypos-=s;
        if(down)
            ypos+=s;
    }
}

void Player::handleKeyDown(int key){
    switch(key){
        case SDLK_LEFT:
            left = 1;
            break;
        case SDLK_RIGHT:
            right = 1;
            break;
        case SDLK_UP:
            up = 1;
            break;
        case SDLK_DOWN:
            down = 1;
            break;
        default:
            break;
    }
}
void Player::handleKeyUp(int key){
    switch(key){
        case SDLK_LEFT:
            left = 0;
            break;
        case SDLK_RIGHT:
            right = 0;
            break;
        case SDLK_UP:
            up = 0;
            break;
        case SDLK_DOWN:
            down = 0;
            break;
        default:
            break;
    }
}
