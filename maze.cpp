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
    
    srcR.x = srcR.w*(id%10);
    srcR.y = srcR.h*int(id/10);
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
    if(strcmp(direction, "top") == 0){
        switch(id){
            case 0:
                update(11);break;
            case 2:
                update(15);break;
            case 4:
                update(10);break;
            case 6:
                update(1);break;
            case 9:
                update(3);break;
            case 12:
                update(7);break;
            case 13:
                update(5);break;
            case 14:
                update(8);break;
            default:
                std::cout<<"Cell with id "<<id<<" has no top wall\n";
                exit(EXIT_FAILURE);
                break;
        }
    }
    else if(strcmp(direction, "bottom") == 0){
        switch(id){
            case 0:
                update(13);break;
            case 4:
                update(2);break;
            case 7:
                update(1);break;
            case 8:
                update(3);break;
            case 10:
                update(15);break;
            case 11:
                update(5);break;
            case 12:
                update(6);break;
            case 14:
                update(9);break;
            default:
                std::cout<<"Cell with id "<<id<<" has no bottom wall\n";
                exit(EXIT_FAILURE);
                break;
        }
    }
    else if(strcmp(direction, "right") == 0){
        switch(id){
            case 0:
                update(12);break;
            case 3:
                update(15);break;
            case 5:
                update(1);break;
            case 8:
                update(10);break;
            case 9:
                update(2);break;
            case 11:
                update(7);break;
            case 13:
                update(6);break;
            case 14:
                update(4);break;
            default:
                std::cout<<"Cell with id "<<id<<" has no right wall\n";
                exit(EXIT_FAILURE);
                break;
        }
    }
    else if(strcmp(direction, "left") == 0){
        switch(id){
            case 0:
                update(14);break;
            case 1:
                update(15);break;
            case 5:
                update(3);break;
            case 6:
                update(2);break;
            case 7:
                update(10);break;
            case 11:
                update(8);break;
            case 12:
                update(4);break;
            case 13:
                update(9);break;
            default:
                std::cout<<"Cell with id "<<id<<" has no left wall\n";
                exit(EXIT_FAILURE);
                break;
        }
    }
    else{
        std::cout<<"Invalid use of removeWall(). Please use top, bottom, left or right as argument!\n";
        exit(EXIT_FAILURE);
    }
}