#include "game.h"

Player::Player(){
    name = "Not Entered yet!";
    xpos = 0, ypos = 0;
    old_xpos = 0, old_ypos = 0;
    score = 0;
    time = 500;
    right = 0, left = 0, up = 0, down = 0;
    width = PLAYER_WIDTH; height = PLAYER_HEIGHT;
    playerId = 1;
    renderCycle = 1;
    player_no = 1;
    attack = false;
    attack_counter = 0;
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
    attack_counter=p.attack_counter;
    attack = false;
    attack_dir = p.attack_dir;
}

void Player::encode(int x[]){
    x[0] = xpos;
    x[1] = ypos;
    x[2] = score;
    x[3] = time;
    x[4] = playerId;
}

void Player::decode(int y[]){
    old_xpos = xpos;
    old_ypos = ypos;
    xpos = y[0];
    ypos = y[1];
    score = y[2];
    time = y[3];
    playerId = y[4];
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
        case  SDLK_a:
            attack = true;
            attack_dir = 0;
            attack_counter =0;
            break;
        case  SDLK_d:
            attack = true;
            attack_dir = 1;
            attack_counter =0;
            break;
        case  SDLK_w:
            attack = true;
            attack_dir = 2;
            attack_counter =0;
            break;
        case  SDLK_s:
            attack = true;
            attack_dir = 3;
            attack_counter =0;
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
        case  SDLK_a:
            attack = false;
            break;
        case  SDLK_d:
            attack = false;
            break;
        case  SDLK_w:
            attack = false;
            break;
        case  SDLK_s:
            attack = false;
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

void Game::askPlayerAvatar(){
    char id[1];
    memset(id, ' ', 1);
    SDL_Event e;
    int position = 0;
    int ok = false;
    while (!ok) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if ((e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9)) {
                    if (position > 0) {
                        position = 0;
                    }
                    id[position] = e.key.keysym.sym;
                    position++;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    position--;
                    if (position < 0) {
                        position = 0;
                    }
                    id[position] = ' ';
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    id[position] = 0;
                    ok = true;
                }
            }
        }

        SDL_Rect srcR[8];
        for(int i = 0; i<8; i++){
            srcR[i].w = PLAYER_WIDTH_SRC; srcR[i].h = PLAYER_HEIGHT_SRC;
            srcR[i].y = 0;
            srcR[i].x = (i*3 + 1) * PLAYER_WIDTH_SRC;
        }

        SDL_Rect dstR[8];
        for(int i = 0; i<8; i++){
            dstR[i].w = PLAYER_WIDTH; dstR[i].h = PLAYER_HEIGHT;
            dstR[i].y = SCREEN_HEIGHT/2+20;
            dstR[i].x = 10 + SCREEN_WIDTH/2 + (i-4)*(PLAYER_WIDTH + 10);
        }


        usleep(200);
        SDL_RenderClear(renderer);
        if(isServer)
            disp_text_center(renderer, "Player 1" , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2)-45);
        else    
            disp_text_center(renderer, "Player 2" , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2)-45);
        char c[] = "Please chose Your Avatar";
        disp_text_center(renderer, c , font, int(SCREEN_WIDTH/2)+10, int(SCREEN_HEIGHT/2)-20);
        disp_text_center(renderer, id , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2)+120);

        for(int i = 0; i<8; i++){
            if(SDL_RenderCopyEx(renderer, sPlayer.Tex,  &srcR[i], &dstR[i], 0.0, NULL, SDL_FLIP_NONE) < 0){
                std::cout<<"Avatars not rendered properly\n";
                std::cout<<SDL_GetError()<<"\n";
                exit(EXIT_FAILURE);
            }
            char * digits[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
            char * p_id = digits[i]; 
            
            disp_text_center(renderer, p_id , font, dstR[i].x+dstR[i].w/2 , int(SCREEN_HEIGHT/2)+80);
        }

        SDL_RenderPresent(renderer);
    }
    if(isServer)
        sPlayer.playerId = std::stoi(id);
    else    
        cPlayer.playerId = std::stoi(id);
}