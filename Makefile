CC = g++
#SRC will contain the names of the cpp files
SRC = main.cpp Game/game.cpp Helper/font.cpp Helper/menu.cpp Network/client.cpp Network/server.cpp Game/player.cpp Game/maze.cpp Game/monster.cpp Game/Bullets.cpp Helper/sounds.cpp
SDL2 = `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -lpthread -I. -Wall
LIBS = $(SDL2)
FLAGS = -Wno-write-strings
#OBJ names of object files
OBJ = $(SRC:.cpp = .o)
#exe name
CNAME = play

build: $(OBJ) 
	$(info Compiling code...)
	@$(CC) $(OBJ) -o $(CNAME)  $(LIBS) $(FLAGS)

run: 
	./$(CNAME)
