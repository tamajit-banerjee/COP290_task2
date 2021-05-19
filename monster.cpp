#include "game.h"


Monster::Monster(){
    xpos = 0;
    ypos = 0;
    right = 0;
    left = 0;
    up = 0;
    down = 0;
    id = 0;
    width = 60;
    height = 60;
    changeDirectionCounter = 0;
    renderCycle = 1;
}

void Monster::setPosCenter(int i, int j){
    xpos = i*CELL_SIZE + CELL_SIZE/2 - width/2;
    ypos = j*CELL_SIZE + CELL_SIZE/2 - height/2;
}

void Game::initMonsters(){
    monsters[0].setPosCenter(0 + rand()%5, 0 + rand()%5);
    monsters[0].dest = rand()%(MAZECOLS*MAZEROWS);
    monsters[0].chase_which_player = 1;
    monsters[0].mode_chase = true;
    monsters[0].chase_time = 0;
    monsters[0].not_chase_time = 0;
    monsters[1].setPosCenter(5 + rand()%5, 0 + rand()%5);
    monsters[1].dest = rand()%(MAZECOLS*MAZEROWS);
    monsters[1].chase_which_player = 0;
    monsters[1].mode_chase = false;
    monsters[1].chase_time = 0;
    monsters[1].not_chase_time = 0;
    monsters[2].setPosCenter(0 + rand()%5, 5 + rand()%5);
    monsters[2].dest = rand()%(MAZECOLS*MAZEROWS);
    monsters[2].chase_which_player = 1;
    monsters[2].mode_chase = false;
    monsters[2].chase_time = 0;
    monsters[2].not_chase_time = 0;
    monsters[3].setPosCenter(5 + rand()%5, 5 + rand()%5);
    monsters[3].dest = rand()%(MAZECOLS*MAZEROWS);
    monsters[3].chase_which_player = 0;
    monsters[3].mode_chase = true;
    monsters[3].chase_time = 0;
    monsters[3].not_chase_time = 0;
}

void Monster::move(int s){
    old_xpos = xpos, old_ypos = ypos;
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

void Monster::draw(SDL_Renderer *renderer, TTF_Font *font){

    SDL_Rect destR;
    destR.h = height;
    destR.w = width;
    destR.x = xpos;
    destR.y = ypos + SCORE_DISPLAY_HEIGHT;

    SDL_Rect srcR;
    srcR.h = MONSTER_SIZE;
    srcR.w = MONSTER_SIZE;
    
    if(xpos > old_xpos)
        srcR.y = 2*MONSTER_SIZE;
    else if(xpos < old_xpos)
        srcR.y = 1*MONSTER_SIZE;
    else if(ypos < old_ypos)
        srcR.y = 3*MONSTER_SIZE;
    else if(ypos > old_ypos)
        srcR.y = 0;
    else{
        srcR.y = 0;
        renderCycle = 0;
    }
    srcR.x = int(renderCycle/MONSTER_DELAY) * MONSTER_SIZE;
    renderCycle = (renderCycle+1)%(3*MONSTER_DELAY) ;

    // SDL_RenderCopy(renderer, Tex,  srcR, &destR);
    if(SDL_RenderCopyEx(renderer, Tex,  &srcR, &destR, 0.0, NULL, SDL_FLIP_NONE) < 0){
        std::cout<<"Monster not rendered properly\n";
        std::cout<<SDL_GetError()<<"\n";
        exit(EXIT_FAILURE);
    }
}



bool iscolliding(Player p, Monster m){
    if (p.xpos >= m.xpos + m.width || m.xpos >= p.xpos + p.width)
        return false;
    if (p.ypos >= m.ypos + m.height || m.ypos >= p.ypos + p.height)
        return false;

    return true;
    
}

void Game::checkMonsterCollisions_Player(Player &p, bool playerIsServer){
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
            if(!p.freeze && ((isServer && playerIsServer) || (!isServer && !playerIsServer)))
                sounds.play("freeze");
            p.freeze = true;
        }
    }
}

bool Game::checkMonsterCollisions_Bullet(Bullet &b){
    for(int i = 0; i< MONSTERS; i++){
        // std::cout<<p.xpos<<p.ypos<<p.width<<p.height<<'\n';
        // std::cout<<monsters[i].xpos<<monsters[i].ypos<<monsters[i].width<<monsters[i].height<<'\n';
        SDL_Rect destR , srcR;
        destR = b.position;
        srcR.h = monsters[i].height;
        srcR.w = monsters[i].width;
        srcR.x = monsters[i].xpos;
        srcR.y = monsters[i].ypos;
        SDL_Rect *d = &destR;
        SDL_Rect *b = &srcR;
        if(SDL_HasIntersection(d,b)){
            return true;
        }
    }
    return false;
}



void Game::handleMonsterCollisions(){
    checkMonsterCollisions_Player(sPlayer, true);
    checkMonsterCollisions_Player(cPlayer, true);

    sPlayer.freeze_counter++;
    cPlayer.freeze_counter++;
    
    if(sPlayer.freeze){
        if (sPlayer.freeze_counter >= FREEZE_LIMIT)
            sPlayer.freeze = false;
    }
    cPlayer.freeze_counter++;
    if(cPlayer.freeze){
        if (cPlayer.freeze_counter >= FREEZE_LIMIT)
            cPlayer.freeze = false;
    }
}


std::pair<int, int> Monster::getMazeCoordinates(SDL_Rect &r){
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

bool checkIfWall(int i, MazeCell & m){
    int factor = 1;
    while(i>0){
        factor = 2*factor;
        i--;
    }
    return int(m.id/factor)%2 != 0;
}

bool colliding(Monster & m, MazeCell & maze){
    if(m.left)
        return maze.id % 2 != 0;
    if(m.right)
        return maze.id/2 % 2 != 0;
    if(m.up)
        return maze.id/4 % 2 != 0;
    if(m.down)
        return maze.id/8 % 2 != 0;
    return false;
}
void resetDirections(Monster & m){
    m.left = 0;
    m.right = 0;
    m.up = 0;
    m.down = 0;
}

bool centre(Monster & m ){
    return (m.xpos + m.width/2 - CELL_SIZE/2)%CELL_SIZE == 0 && (m.ypos + m.height/2 - CELL_SIZE/2)%CELL_SIZE == 0 ;
}
    


bool Game::checkoneMonsterCollisions(Player &p, Monster &m){
        SDL_Rect destR , srcR;
        destR.h = p.height;
        destR.w = p.width;
        destR.x = p.xpos;
        destR.y = p.ypos;
        srcR.h = m.height;
        srcR.w = m.width;
        srcR.x = m.xpos;
        srcR.y = m.ypos;
        SDL_Rect *d = &destR;
        SDL_Rect *b = &srcR;
        if(SDL_HasIntersection(d,b)){
            return true;
        }else{
            return false;
        }
}

void Game::updateMonsters(){
    for(int i = 0 ; i<MONSTERS; i++){
       // bool tochange = false;
        

      //   std::cout<<monsters[i].mode_chase<<" "<<monsters[i].chase_time<<" "<<monsters[i].not_chase_time<<"\n";

        if(centre(monsters[i])){
            SDL_Rect rect;
            
            rect.x = 0; rect.y = 0;
            rect.w = CELL_SIZE; rect.h = CELL_SIZE;
            std::pair<int, int> i_j = monsters[i].getMazeCoordinates(rect);
            resetDirections(monsters[i]);
            if(maze[i_j.first][i_j.second].id == 15)
                continue;
            
        if(monsters[i].mode_chase){
               ++monsters[i].chase_time;
                if( monsters[i].chase_which_player ==  0 ){
                    std::pair<int, int> pos = sPlayer.getMazeCoordinates(rect);
                    int s = pos.first * MAZECOLS + pos.second;
                    monsters[i].dest = s;
                    if(checkoneMonsterCollisions(sPlayer,monsters[i]) || monsters[i].chase_time ==  CHASE_TIME || sPlayer.freeze ){
                        monsters[i].mode_chase = false;
                        monsters[i].not_chase_time = 0;
                    }

                }else{
                    std::pair<int, int> pos = cPlayer.getMazeCoordinates(rect);
                    int s = pos.first * MAZECOLS + pos.second;
                    monsters[i].dest = s;
                    if(checkoneMonsterCollisions(cPlayer,monsters[i]) || monsters[i].chase_time ==  CHASE_TIME || cPlayer.freeze ){
                        monsters[i].mode_chase = false;
                        monsters[i].not_chase_time = 0;
                    }
                }
        }
        
        if(!monsters[i].mode_chase){

        while(monsters[i].dest == i_j.first*MAZECOLS + i_j.second){
            monsters[i].dest = rand()%(MAZEROWS*MAZECOLS);
        }
            
            ++monsters[i].not_chase_time;
            if(monsters[i].not_chase_time >=  NON_CHASE_TIME/level && i!= 1 && i!= 2){

                if(monsters[i].chase_which_player == 0 && !sPlayer.freeze){
                    monsters[i].mode_chase = true;
                    monsters[i].chase_time = 0 ;
                }
                
                if(monsters[i].chase_which_player == 1 && !cPlayer.freeze){
                    monsters[i].mode_chase = true;
                    monsters[i].chase_time = 0 ;
                }

            }
        }
            
            int random = maze[i_j.first][i_j.second].to_go[monsters[i].dest];
            switch(random){
                case 0:
                    monsters[i].left = 1; break;
                case 1:
                    monsters[i].right = 1; break;
                case 2:
                    monsters[i].up = 1; break;
                case 3:
                    monsters[i].down = 1; break;
            }
        }
//        if(monsters[i].changeDirectionCounter == 0){
//            tochange = true;
//        }
//        else{
//            std::pair<int, int> i_j = monsters[i].getMazeCoordinates(maze[0][0].dstR);
//            if(colliding(monsters[i], maze[i_j.first][i_j.second])){
//                tochange = true;
//            }
//        }
//        if(tochange){
//            monsters[i].changeDirectionCounter = MONSTERS_DIR_CHANGE - std::rand()%(MONSTERS_DIR_CHANGE / 2);
//
//            if((monsters[i].xpos + monsters[i].width/2 - CELL_SIZE/2)%CELL_SIZE == 0 && (monsters[i].ypos + monsters[i].height/2 - CELL_SIZE/2)%CELL_SIZE == 0){
//                std::pair<int, int> i_j = monsters[i].getMazeCoordinates(maze[0][0].dstR);
//                resetDirections(monsters[i]);
//                if(maze[i_j.first][i_j.second].id == 15)
//                    continue;
//
//                int random = std::rand()%4;
//                while(checkIfWall(random, maze[i_j.first][i_j.second])){
//                    random = std::rand()%4;
//                }
//                switch(random){
//                    case 0:
//                        monsters[i].left = 1; break;
//                    case 1:
//                        monsters[i].right = 1; break;
//                    case 2:
//                        monsters[i].up = 1; break;
//                    case 3:
//                        monsters[i].down = 1; break;
//                }
//            }
//        }
//        else{
//            monsters[i].changeDirectionCounter -- ;
//        }
        monsters[i].move(SPEED);
    }
}

