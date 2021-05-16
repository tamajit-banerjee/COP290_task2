#include "game.h"

#define PLAYER_WIDTH_SRC 48
#define PLAYER_HEIGHT_SRC 72
#define RENDER_PLAYER_DELAY 10

Player::Player(){
    name = "Not Entered yet!";
    xpos = 0, ypos = 0;
    old_xpos = 0, old_ypos = 0;
    score = 0;
    time = 500;
    right = 0, left = 0, up = 0, down = 0;
    width = 32; height = 48;
    playerId = 1;
    renderCycle = 1;
    player_no = 1;
}


Player::Player(const Player &p){
    name = p.name;
    xpos = p.xpos, ypos = p.ypos;
    score = p.score;
    time = p.time;
    right = 0, left = 0, up = 0, down = 0;
    width = p.width, height = p.height;
    playerId = p.playerId;
    renderCycle = p.renderCycle;
    player_no = p.player_no;
}

void Player::encode(int x[]){
    x[0] = xpos;
    x[1] = ypos;
    x[2] = score;
    x[3] = time;
}

void Player::decode(int y[]){
    old_xpos = xpos;
    old_ypos = ypos;
    xpos = y[0];
    ypos = y[1];
    score = y[2];
    time = y[3];
}

void Player::setPosCenter(int i, int j){
    xpos = i*CELL_SIZE + CELL_SIZE/2 - width/2;
    ypos = j*CELL_SIZE + CELL_SIZE/2 - height/2;
}

void Player::draw(SDL_Renderer *renderer, TTF_Font *font){
    SDL_Rect destR;
    destR.h = height;
    destR.w = width;
    destR.x = xpos;
    destR.y = ypos;

    SDL_Rect srcR;
    srcR.h = PLAYER_HEIGHT_SRC;
    srcR.w = PLAYER_WIDTH_SRC;
    
    if(xpos > old_xpos)
        srcR.y = 2*PLAYER_HEIGHT_SRC;
    else if(xpos < old_xpos)
        srcR.y = 1*PLAYER_HEIGHT_SRC;
    else if(ypos < old_ypos)
        srcR.y = 3*PLAYER_HEIGHT_SRC;
    else if(ypos > old_ypos)
        srcR.y = 0;
    else{
        srcR.y = 0;
        renderCycle = 0;
    }
    srcR.x = (3*playerId + int(renderCycle/RENDER_PLAYER_DELAY)) * PLAYER_WIDTH_SRC;

    // SDL_RenderCopy(renderer, Tex,  srcR, &destR);
    if(SDL_RenderCopyEx(renderer, Tex,  &srcR, &destR, 0.0, NULL, SDL_FLIP_NONE) < 0){
        std::cout<<"Players not rendered properly\n";
        std::cout<<SDL_GetError()<<"\n";
        exit(EXIT_FAILURE);
    }
    renderCycle = (renderCycle+1)%(3*RENDER_PLAYER_DELAY) ;
    disp_text(renderer, "P" , font, xpos + width, ypos+5);
    if(player_no == 1)
        disp_text(renderer,  "1", font, xpos + width, ypos+20);
    else
        disp_text(renderer,  "2", font, xpos + width, ypos+20);
    
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
    disp_text(renderer, "Time: ", font, xpos, ypos);
    std::string temp_str = std::to_string(time);
    char* char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, xpos+50, ypos);
}
std::pair<int,int> Player::move(int s){
    old_xpos = xpos, old_ypos = ypos;
    int new_x = xpos, new_y = ypos;
    if(!freeze and !final_freeze){
        if(right)
            new_x+=s;
        if(left)
            new_x-=s;
        if(up)
            new_y-=s;
        if(down)
            new_y+=s;
    }
    return std::make_pair(new_x, new_y);
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

std::pair<int, int> Player::getMazeCoordinates(SDL_Rect &r){
    int i = 0;
    int j = 0;
    while(i<MAZEROWS){
        while(j<MAZECOLS){
            if(xpos>=r.w * j)
                j+=1;
            if(ypos>=r.h * i)
                i+=1;
            if(xpos<r.w * j && ypos<r.h * i)
                return std::make_pair(i-1, j-1);
        }
    }
    return std::make_pair(-1, -1);
}

void Game::renderPeriscope(){
    SDL_Rect dstR;
    dstR.x = 0; dstR.y = 0;
    dstR.w = 2*SCREEN_WIDTH; dstR.h = 2*SCREEN_HEIGHT;
    if(isServer){
        dstR.x = sPlayer.xpos + sPlayer.width/2 - dstR.w/2;
        dstR.y = sPlayer.ypos + sPlayer.height/2 - dstR.h/2;
    }
    else{
        dstR.x = cPlayer.xpos + cPlayer.width/2 - dstR.w/2;
        dstR.y = cPlayer.ypos + cPlayer.height/2 - dstR.h/2;
    }
    if(SDL_RenderCopyEx(renderer, periTex,  NULL, &dstR, 0.0, NULL, SDL_FLIP_NONE) < 0){
        std::cout<<"Periscope view not rendered properly\n";
        std::cout<<SDL_GetError()<<"\n";
        exit(EXIT_FAILURE);
    }
}

