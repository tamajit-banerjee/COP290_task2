#ifndef CONSTANS_H
#define CONSTANS_H

#include "stdbool.h"

// Dimensions of each cell in the maze and dimensions of the maze in terms of number of cells
#define CELL_SIZE 64
#define MAZECOLS 10
#define MAZEROWS 10

// Sizes of the SDL screen
#define SCREEN_WIDTH (MAZECOLS * CELL_SIZE)
#define SCREEN_HEIGHT (MAZECOLS * CELL_SIZE)

#define SPEED 2
#define LEVELS 4

#define FRAME_RATE 60
#define CLIENT_FRAME_TIME 15
#define SERVER_FRAME_TIME 15
// If sending or receiving takes linger than this (in ms), display a 'slow network' warning
#define NETWORK_THRESHOLD 1000
#define COIN_SIZE (CELL_SIZE / 2)
#define COIN_SCORE 10

#define TIME_SIZE (COIN_SIZE)
#define TIME_INCREASE 200

// dimensions of the player avatars
#define PLAYER_WIDTH_SRC 48
#define PLAYER_HEIGHT_SRC 72
#define RENDER_PLAYER_DELAY 10
#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 48

// dimensions of the monster avatars
#define MONSTERS 4
#define MONSTERS_DIR_CHANGE 30
#define MONSTER_SIZE 131
#define MONSTER_SIZE_SCALE_TO 95
#define MONSTER_DELAY 8
#define FREEZE_LIMIT 100
// After how many cells will a monster stop chasing and start chasing
#define CHASE_TIME 15
#define NON_CHASE_TIME 20

// dimensions of the bullet and its cost
#define BULLET_COST 5
#define BULLET_WIDTH 8
#define BULLET_HEIGHT 8

// total level time, steps in which time increases 
#define LEVEL_TIME 2000
#define TIME_STEP 10

#define WALL_REMOVE_TIME 100
#define WALL_RATIO 20

#define SCORE_DISPLAY_HEIGHT 100
#define SLEEP_UNIT 100

#define COIN_DIST 2
#define MONSTER_DIST 2

#define PERISCOPE 1
#define PROBABILITY 9

#endif
