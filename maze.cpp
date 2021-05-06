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
