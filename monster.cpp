#include "game.h"

Monster::Monster(){
    xpos = 0;
    ypos = 0;
    right = 0;
    left = 0;
    up = 0;
    down = 0;
    id = 0;
    width = 30;
    height = 30;
    changeDirectionCounter = 0;
}

void Monster::setPosCenter(int i, int j){
    xpos = i*CELL_SIZE + CELL_SIZE/2 - width/2;
    ypos = j*CELL_SIZE + CELL_SIZE/2 - height/2;
}

void Game::initMonsters(){
    monsters[0].setPosCenter(4, 4);
    monsters[1].setPosCenter(4, 5);
    monsters[2].setPosCenter(5, 4);
    monsters[3].setPosCenter(5, 5);
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

void Monster::draw(SDL_Renderer *renderer, TTF_Font *font){
    SDL_Rect destR;
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

void Game::handleMonsterCollisions(){
    checkMonsterCollisions(sPlayer);
    checkMonsterCollisions(cPlayer);

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

void resetDirections(Monster & m){
    m.left = 0;
    m.right = 0;
    m.up = 0;
    m.down = 0;
}
void Game::updateMonsters(){
    
    for(int i = 0 ; i<MONSTERS; i++){
        if((monsters[i].xpos + monsters[i].width/2 - CELL_SIZE/2)%CELL_SIZE == 0 && (monsters[i].ypos + monsters[i].height/2 - CELL_SIZE/2)%CELL_SIZE == 0){
            std::pair<int, int> i_j = monsters[i].getMazeCoordinates(maze[0][0].dstR);
            resetDirections(monsters[i]);
            if(maze[i_j.first][i_j.second].id == 15)
                continue;
            
            int random = std::rand()%4;
            while(checkIfWall(random, maze[i_j.first][i_j.second])){
                random = std::rand()%4;
            }
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
        monsters[i].move(SPEED);
    }
}

