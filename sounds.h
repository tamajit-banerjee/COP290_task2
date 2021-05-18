#pragma once
#include "Header.h"
#define NUMOFSOUNDS 5

class Sounds{
public:
    Sounds();
    ~Sounds();
    bool init();
    void play(int i, bool looped=0, int volume=128);
    void stop(int i);
    void stop();
    void toggleSounds();
    bool on;

    Mix_Chunk * sounds[NUMOFSOUNDS];
    std::string soundpaths[NUMOFSOUNDS];
    bool isinit;

};