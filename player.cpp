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

void Player::encode(char * s, int size){
    int i = 0;
    while(i<sizeof(name)/sizeof(name[0])){
        s[i] = name[i];
        i++;
    }
    s[i] = separator; i++;
    int j = 0;
    while(j<(std::to_string(xpos).length())){
        s[i] = std::to_string(xpos)[j];
        i++; j++;
    }
    j = 0;
    s[i] = separator; i++;
    while(j<(std::to_string(ypos).length())){
        s[i] = std::to_string(ypos)[j];
        i++; j++;
    }
    j = 0;
    s[i] = separator; i++;
    while(j<(std::to_string(score).length())){
        s[i] = std::to_string(score)[j];
        i++; j++;
    }
    j = 0;
    s[i] = separator; i++;
    while(j<(std::to_string(time).length())){
        s[i] = std::to_string(time)[j];
        i++; j++;
    }
    s[i] = separator; 
    i++;
    while(i<size){
        s[i] = ' ';
        i++;
    }
    
}

void Player::decode(char * s){
    int i = 0;
    char nametmp[100];
    while(s[i]!=separator){
        nametmp[i] = s[i];
        i++;
    }
    name = nametmp;

    i++;
    std::string xpostmp = ""; 
    while(s[i]!=separator){
        xpostmp += s[i];
        i++; 
    }
    xpos = std::stoi(xpostmp);

    i++;
    std::string ypostmp = ""; 
    while(s[i]!=separator){
        ypostmp += s[i];
        i++; 
    }
    ypos = std::stoi(ypostmp);

    i++;
    std::string scoretmp = ""; 
    while(s[i]!=separator){
        scoretmp += s[i];
        i++; 
    }
    score = std::stoi(scoretmp);

    i++;
    std::string timetmp = "";
    while(s[i]!=separator){
        timetmp += s[i];
        i++; 
    }
    time = std::stoi(timetmp);
}

void Player::draw(SDL_Renderer *renderer, TTF_Font *font){
    SDL_Rect destR;
    destR.h = 25;
    destR.w = 25;
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
    if(right)
        xpos+=s;
    if(left)
        xpos-=s;
    if(up)
        ypos-=s;
    if(down)
        ypos+=s;
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