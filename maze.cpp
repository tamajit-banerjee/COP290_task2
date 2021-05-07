#include "game.h"


MazeCell::MazeCell(){
    MazeCell(0);
    srcR.w = 32;
    srcR.h = 32;
}
MazeCell::MazeCell(int arg_id){
    update(arg_id);
    srcR.w = 32;
    srcR.h = 32;
}

void MazeCell::update(int arg_id){
    id = arg_id;
    
    srcR.x = srcR.w * (id%10);
    srcR.y = srcR.h * (id/10);
    
}

void MazeCell::removeWall(int direction){
    switch(direction){
        case 0:
            removeWall("top");
            break;
        case 1:
            removeWall("bottom");
            break;
        case 2:
            removeWall("right");
            break;
        case 3:
            removeWall("left");
            break;
        default:
            std::cout<<"Invalid use of removeWall()!\n";
            exit(EXIT_FAILURE);
            break;

    }
}

void MazeCell::removeWall(char *direction){
    int factor = -1;

    if(strcmp(direction, "left") == 0)
        factor = 0;
    else if(strcmp(direction, "right") == 0)
        factor = 1;
    else if(strcmp(direction, "top") == 0)
        factor = 2;
    else if(strcmp(direction, "bottom") == 0)
        factor = 3;
    else{
        std::cout<<"Invalid use of removeWall(). Please use top, bottom, left or right as argument!\n";
        exit(EXIT_FAILURE);
    }

    if( (id>>factor) % 2 == 1){
        update(id - (1<<factor) );
    }
    else{
        std::cout<<"Wall to remove does not exist\n";
        exit(EXIT_FAILURE);
    }

}


bool Game::ok(int x, int y){
    if(x>=0 && x<MAZEROWS && y>=0 && y<MAZECOLS){
        return true;
    }else
        return false;
}

void Game::dfs(int x, int y){
    
    std::pair<int,int> dir[] = {std::make_pair(1,0),std::make_pair(-1,0),std::make_pair(0,1),std::make_pair(0,-1)};
    
    // std::cout<<"dfs/n";
    while(true){
        renderMaze();
        int random = std::rand();
        random = random%4;
        if(ok(x+dir[random].first,y+dir[random].second) && maze[x+dir[random].first][y+dir[random].second].id == 15 ){
            switch (random) {
                case 1:
                    maze[x][y].removeWall("top");
                    maze[x+dir[random].first][y+dir[random].second].removeWall("bottom");
                    break;
                case 0:
                    maze[x][y].removeWall("bottom");
                    maze[x+dir[random].first][y+dir[random].second].removeWall("top");
                    break;
                case 2:
                    maze[x][y].removeWall("right");
                    maze[x+dir[random].first][y+dir[random].second].removeWall("left");
                    break;
                case 3:
                    maze[x][y].removeWall("left");
                    maze[x+dir[random].first][y+dir[random].second].removeWall("right");
                    break;
                default:
                    break;
            }
            dfs(x+dir[random].first,y+dir[random].second);
        }
        int cnt = 0;
        for(int i=0;i<4;i++){
            if(ok(x+dir[i].first,y+dir[i].second) && maze[x+dir[i].first][y+dir[i].second].id == 15  ){
                ++cnt;
            }
        }
        if(!cnt)
            break;
    }
    
}

void Game:: maze_gen(){
    
    int x = std::rand()%10;
    int y = std::rand()%10;

    dfs(x,y);
    
    
}




void Game::renderMaze(){
    int cell_width = CELL_SIZE;
    int cell_height = CELL_SIZE;
    int coin_width = COIN_SIZE;
    int coin_height = COIN_SIZE;
    for(int i =0; i<MAZEROWS; i++){
        for(int j = 0; j<MAZECOLS; j++){
            
            maze[i][j].dstR.w = cell_width;
            maze[i][j].dstR.h = cell_height;
            maze[i][j].dstR.x = maze[i][j].dstR.w * j;
            maze[i][j].dstR.y = maze[i][j].dstR.h * i;
            if(SDL_RenderCopyEx(renderer, mazeTex,  &maze[i][j].srcR, &maze[i][j].dstR, 0.0, NULL, SDL_FLIP_NONE) < 0){
                std::cout<<"Maze not rendered properly\n";
                std::cout<<SDL_GetError()<<"\n";
                exit(EXIT_FAILURE);
            }

            SDL_Rect dstR, srcR;
            srcR.w = 160;
            srcR.h = 160;
            srcR.x = srcR.w * ((coinId/10)%8);
            srcR.y = 0;

            dstR.w = coin_width;
            dstR.h = coin_height;
            dstR.x = cell_width * j + (cell_width - coin_width)/2;
            dstR.y = cell_height * i + (cell_height - coin_height)/2;
            
            if(maze[i][j].hascoin){
                if(SDL_RenderCopyEx(renderer, coinTex,  &srcR, &dstR, 0.0, NULL, SDL_FLIP_NONE) < 0){
                    std::cout<<"Coin not rendered properly\n";
                    std::cout<<SDL_GetError()<<"\n";
                    exit(EXIT_FAILURE);
                }
            }

            if(maze[i][j].hastime){
                if(SDL_RenderCopyEx(renderer, timeTex,  NULL, &dstR, 0.0, NULL, SDL_FLIP_NONE) < 0){
                    std::cout<<"Time not rendered properly\n";
                    std::cout<<SDL_GetError()<<"\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}
void Game::updateCoins(){

}
void Game::placeCoins(){
    maze[0][3].hascoin = true;
    maze[1][5].hascoin = true;
    maze[3][7].hascoin = true;
    maze[4][1].hascoin = true;
    maze[5][4].hascoin = true;
}

void updateTimes(){
    
}
void Game::placeTimes(){
    maze[3][3].hastime = true;
    maze[2][1].hastime = true;
    maze[1][0].hastime = true;
    maze[0][4].hastime = true;
    maze[5][2].hastime = true;
}


void Game::mazeInit(){
    for(int i =0; i<MAZEROWS; i++){
        for(int j = 0; j<MAZECOLS; j++){
            maze[i][j].update(15);
            maze[i][j].hascoin = false;
            maze[i][j].hastime = false;
        }
    }
    coinId = 0;
    placeCoins();
    placeTimes();
}



bool iscollidingwall(int x, int y, int w, int h, SDL_Rect maze_rect){
    if (x >= maze_rect.x + maze_rect.w || maze_rect.x >= x + w)
        return false;
    if (y >= maze_rect.y + maze_rect.h || maze_rect.y >= y + h)
        return false;

    return true;
    
}

int pow(int x, int y){
    if(y <= 0)
        return 1;
    else    
        return x * pow(x, y-1);
}

bool Game::checkWallCollisions(int x, int y, int w, int h){
    for(int i =0; i<MAZEROWS; i++){
        for(int j = 0; j<MAZECOLS; j++){
            int id = maze[i][j].id;
            
            SDL_Rect maze_rect[4];
            // left
            maze_rect[0] = maze[i][j].dstR;
            maze_rect[0].w /= WALL_RATIO;
            // right
            maze_rect[1] = maze[i][j].dstR;
            maze_rect[1].w /= WALL_RATIO;
            maze_rect[1].x += maze[i][j].dstR.w - maze_rect[1].w;
            // top
            maze_rect[2] = maze[i][j].dstR;
            maze_rect[2].h /= WALL_RATIO;
            // bottom
            maze_rect[3] = maze[i][j].dstR;
            maze_rect[3].h /= WALL_RATIO;
            maze_rect[3].y += maze[i][j].dstR.h - maze_rect[3].h;
            
            for(int k = 0; k<4; k++){
                if((int(id/pow(2, k))%2 != 0) && iscollidingwall(x, y, w, h, maze_rect[k])){
                    return true;
                }
            }
        }
    }
    return false;
}

bool isOnCoin(int x, int y, int w, int h, SDL_Rect & rect){
    // std::cout<<"width "<<w<<"x: "<<x<<"y: "<<y<<'\n';
    // std::cout<<"r width "<<rect.w<<"r x: "<<rect.x<<"r y: "<<rect.y<<'\n';
    int threshold = COIN_SIZE;
    int count = 0;
    if((x - rect.x + w/2 - int(rect.w/2))*(x - rect.x + w/2 - int(rect.w/2)) < threshold*threshold)
        count ++;
    if((y - rect.y + h/2 - int(rect.h/2))*(y - rect.y + h/2 - int(rect.h/2)) < threshold*threshold)
        count ++;
    if (count == 2)
        return true;
    else    
        return false;
}

bool playerOnCoin(Player & p, MazeCell & m){
    // std::cout<<m.hascoin<<isOnCoin(p.xpos, p.ypos, p.width, p.height, m.dstR);
    if(m.hascoin && isOnCoin(p.xpos, p.ypos, p.width, p.height, m.dstR)){
        m.hascoin = false;
        p.score += COIN_SCORE;
        return true;
    }
    return false;
}

void Game::checkCoinEat(){
    std::pair<int, int> s_co = sPlayer.getMazeCoordinates(maze[0][0].dstR);
    std::pair<int, int> c_co = cPlayer.getMazeCoordinates(maze[0][0].dstR);
    
    int random_i = std::rand() % MAZEROWS;
    int random_j = std::rand() % MAZECOLS;
    if(playerOnCoin(sPlayer, maze[s_co.first][s_co.second])){
        while(maze[random_i][random_j].hascoin == true){
            random_i = std::rand() % MAZEROWS;
            random_j = std::rand() % MAZECOLS;
        }
        maze[random_i][random_j].hascoin = true;
    }

    random_i = std::rand() % MAZEROWS;
    random_j = std::rand() % MAZECOLS;
    if(playerOnCoin(cPlayer, maze[c_co.first][c_co.second])){
        while(maze[random_i][random_j].hascoin == true){
            random_i = std::rand() % MAZEROWS;
            random_j = std::rand() % MAZECOLS;
        }
        maze[random_i%MAZEROWS][random_j%MAZECOLS].hascoin = true;
    }
    // std::cout<<'\n';
}
