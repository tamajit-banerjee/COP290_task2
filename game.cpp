#include "Header.h"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"

#include <chrono>
#include <thread>

#include "game.h"


#define mapSize 38

std::pair<int,int> dir[] = {std::make_pair(1,0),std::make_pair(-1,0),std::make_pair(0,1),std::make_pair(0,-1)};
// 0 - top 1 - bottom 2 - right 3 - left
int map[mapSize][3] = {
    {0, 0, 2},
    {0, 1, 3},
    {0, 1, 2},
    {0, 2, 3},
    {0, 2, 1},
    {1, 2, 0},
    {1, 2, 1},
    {2, 2, 0},
    {2, 2, 1},
    {3, 2, 0},
    {3, 2, 2},
    {3, 3, 3},
    {3, 3, 2},
    {3, 4, 3},
    {3, 4, 2},
    {3, 5, 3},
    {3, 5, 1},
    {4, 5, 0},
    {4, 5, 1},
    {5, 5, 0},
    {5, 5, 2},
    {5, 6, 3},
    {5, 6, 2},
    {5, 7, 3},
    {5, 7, 2},
    {5, 8, 3},
    {5, 8, 1},
    {6, 8, 0},
    {6, 8, 1},
    {7, 8, 0},
    {5, 0, 2},
    {5, 1, 3},
    {5, 1, 2},
    {5, 2, 3},
    {5, 2, 1},
    {6, 2, 0},
    {6, 2, 1},
    {7, 2, 0},

};

void Game::init(SDL_Renderer *arg_renderer, TTF_Font *arg_font )
{

	renderer = arg_renderer;
    font = arg_font;

    isRunning = true;
    isLevelRunning = true;

    loadTexture("player", "resources/player.bmp");
    loadTexture("monster", "resources/monster.bmp");
    loadTexture("maze", "resources/maze.bmp");
    loadTexture("coin", "resources/coins.bmp");
    loadTexture("time", "resources/time.bmp");

    mazeInit();
    
    counter = 0;

    sPlayer.xpos = 144;
    sPlayer.ypos = 16;
    cPlayer.xpos = 16;
    cPlayer.ypos = 16;
    sPlayer.time = 700;

    for(int i = 0 ; i<MONSTERS; i++){
        monsters[i].id = i;
        monsters[i].xpos = (i*i*i) + 320 -100/(1+i);
        monsters[i].ypos = (13*i*i) + 130 -50/(1+i);
    }
        
}

void Game::handleEvents()
{
	
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE)
            isRunning = false;
        if(isServer)
            sPlayer.handleKeyDown(event.key.keysym.sym);
        else
            cPlayer.handleKeyDown(event.key.keysym.sym);
        break;
    case SDL_KEYUP:
        if(isServer)
            sPlayer.handleKeyUp(event.key.keysym.sym);
        else
            cPlayer.handleKeyUp(event.key.keysym.sym);
        break;
	default:
		break;
	}
}

bool Game::ok(int x, int y){
    if(x>=0 && x<10 && y>=0 && y<10){
        return true;
    }else
        return false;
}

void Game::dfs(int x, int y){
    
    std::cout<<"dfs/n";
    while(true){
        renderMaze();
        //sleep(2);
        int random = std::rand();
        random = random%4;
        if(ok(x+dir[random].first,y+dir[random].second) && maze[x+dir[random].first][y+dir[random].second].id == 15 ){
            switch (random) {
                case 0:
                    maze[x][y].removeWall("top");
                    maze[x+dir[random].first][y+dir[random].second].removeWall("bottom");
                    break;
                case 1:
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
void Game::update(){

    
    
//    if(counter<5*mapSize){
//        // if(maze[int(counter/mazeRows)][counter%mazeRows].id == 0 && maze[int((counter+1)/mazeRows)][(counter+1)%mazeRows].id == 0){
//        //     maze[int(counter/mazeRows)][counter%mazeRows].removeWall("right");
//        //     maze[int((counter+1)/mazeRows)][(counter+1)%mazeRows].removeWall("left");
//        // }
//        if(counter%10==0){
//            int i = counter / 10;
//
//            maze[map[2*i][0]][map[2*i][1]].removeWall(map[2*i][2]);
//            maze[map[2*i+1][0]][map[2*i+1][1]].removeWall(map[2*i+1][2]);
//        }
//
//
//    }

    for(int i = 0 ; i<MONSTERS; i++){
        int j = (counter) % 100;
        monsters[i].left = (j <= 7*i*i || monsters[i].xpos < 20) ? 0 : 1;
        monsters[i].right = (j > 7*i*i || monsters[i].xpos > 600) ? 0 : 1;
        monsters[i].up = (j <= 57*i -10 || monsters[i].ypos < 20) ? 0 : 1;
        monsters[i].down = (j > 57*i -10 || monsters[i].ypos > 420) ? 0 : 1;
        monsters[i].move(speed);
    }

    counter++;

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


    std::pair<int, int> s_p = sPlayer.move(speed);
    std::pair<int, int> c_p = cPlayer.move(speed);
    if(!checkWallCollisions(s_p.first, s_p.second, sPlayer.width, sPlayer.height)){
        sPlayer.xpos = s_p.first;
        sPlayer.ypos = s_p.second;
    }

    if(!checkWallCollisions(c_p.first, c_p.second, cPlayer.width, cPlayer.height)){
        cPlayer.xpos = c_p.first;
        cPlayer.ypos = c_p.second;
    }


    if(sPlayer.time>0)
        sPlayer.time -= 1;
    if(cPlayer.time>0)
        cPlayer.time -= 1;

    coinId++;
    if(coinId==80){
        coinId = 0;
    }
    updateCoins();

    // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
}

void Game::render(){
    SDL_RenderClear(renderer);

    renderMaze();

    sPlayer.draw(renderer, font);
    cPlayer.draw(renderer, font);


    sPlayer.dispName(renderer, font, 300, 20);
    sPlayer.dispScore(renderer, font, 400, 20);
    sPlayer.dispTime(renderer, font, 500, 20);

    cPlayer.dispName(renderer, font, 300, 40);
    cPlayer.dispScore(renderer, font, 400, 40);
    cPlayer.dispTime(renderer, font, 500, 40);

    for(int i = 0 ; i<MONSTERS; i++){
        monsters[i].draw(renderer, font);
    }

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    disp_text(renderer, "Results", font, 300, 140);
    
    sPlayer.dispName(renderer, font, 250, 200);
    sPlayer.dispScore(renderer, font, 350, 200);

    cPlayer.dispName(renderer, font, 250, 250);
    cPlayer.dispScore(renderer, font, 350, 250);

    SDL_RenderPresent(renderer);
}

void Game::renderMaze(){
    int cell_width = 64;
    int cell_height = 64;
    int coin_width = 32;
    int coin_height = 32;
    for(int i =0; i<mazeRows; i++){
        for(int j = 0; j<mazeCols; j++){
            
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
    for(int i =0; i<mazeRows; i++){
        for(int j = 0; j<mazeCols; j++){
            maze[i][j].update(15);
            maze[i][j].hascoin = false;
            maze[i][j].hastime = false;
        }
    }
    coinId = 0;
    placeCoins();
    placeTimes();
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
            std::cout<<"colliding!\n";
            std::cout<<p.name<<"\n";
            p.freeze_counter = 0;
            p.freeze = true;
        }
    }
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
    for(int i =0; i<mazeRows; i++){
        for(int j = 0; j<mazeCols; j++){
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

void Game::loadTexture(char *textName, char *path){
    SDL_Surface* tmpSurface;
    if(strcmp(textName, "player") == 0){
        tmpSurface = SDL_LoadBMP(path);
        cPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        sPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    if(strcmp(textName, "monster") == 0){
        tmpSurface = SDL_LoadBMP(path);
        for(int i = 0; i<MONSTERS; i++){
            monsters[i].Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        }
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "maze") == 0){
        tmpSurface = SDL_LoadBMP(path);
        mazeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "coin") == 0){
        tmpSurface = SDL_LoadBMP(path);
        coinTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "time") == 0){
        tmpSurface = SDL_LoadBMP(path);
        timeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }

}
