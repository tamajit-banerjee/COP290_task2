CC = g++
#SRC will contain the names of the cpp files
SRC = main.cpp game.cpp font.cpp menu.cpp client.cpp server.cpp player.cpp maze.cpp monster.cpp
SDL2 = `sdl2-config --cflags --libs` -lSDL2_ttf -lpthread -I. -Wall
LIBS = $(SDL2)
FLAGS = -Wno-write-strings
#OBJ names of object files
OBJ = $(SRC:.cpp = .o)
#exe name
CNAME = game

build: $(OBJ) 
	$(info Compiling code...)
	@$(CC) $(OBJ) -o $(CNAME)  $(LIBS) $(FLAGS)

run: 
	./game
