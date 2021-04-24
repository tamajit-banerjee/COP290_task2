build: main.cpp font.cpp menu.cpp
	g++ -o game main.cpp font.cpp menu.cpp `sdl2-config --cflags --libs` -lSDL2_ttf -lpthread -I. -Wall
