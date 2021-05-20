CC = g++
#SRC will contain the names of the cpp files
SRC = main.cpp Game/game.cpp Helper/font.cpp Helper/menu.cpp Network/client.cpp Network/server.cpp Game/player.cpp Game/maze.cpp Game/monster.cpp Game/Bullets.cpp Helper/sounds.cpp
SDL2 = `sdl2-config --cflags --libs` 
LIBS = $(SDL2) -lSDL2_ttf -lSDL2_mixer -lpthread
FLAGS = -Wno-write-strings -I. -Wall
#OBJ names of object files
OBJ = $(SRC:.cpp = .o)
#exe name
CNAME = play

build: $(OBJ) 
	$(info Compiling code...)
	@$(CC) $(OBJ) -o $(CNAME)  $(LIBS) $(FLAGS)

run: 
	./$(CNAME)

setup:
	$(info Installing dependencies...)
	@brew install sdl && brew install sdl_ttf && brew install sdl_mixer && brew install sdl_gfx
