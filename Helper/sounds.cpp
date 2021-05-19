#include "sounds.h"
 
 
void Sounds::toggleSounds() {
   if ( on ) {
       on = false;
       stop();
   }
   else
       on = true;
}
 
void Sounds::stop() {
   int i;
   for (i=0;i<NUMOFSOUNDS;i++)
       stop(i);
}
 
void Sounds::stop(int i) {
   if ( !isinit ) return;
   if (Mix_Playing(i))
       Mix_HaltChannel(i);
}

void Sounds::play(char *soundName, bool looped, int volume){
    for(int i = 0; i<NUMOFSOUNDS; i++){
        if(strcmp(soundName, soundnames[i]) == 0)
            return play(i, looped, volume);
    }
    std::cout<<"Error: sound trying to play does not exist\n";
}
 
void Sounds::play(int i, bool looped, int volume) {
   /* frequency used to be a parameter here until switching to sdl
    * we might want to reenable that functionality sometime */
 
   if ( !isinit ) return;
   if (!on) return;
 
   if (Mix_Playing(i))
       Mix_HaltChannel(i);
 
   int loop = 0;
   if ( looped )
       loop = -1;
 
   Mix_Volume(i,volume);
   Mix_PlayChannel(i,sounds[i],loop);
}
bool Sounds::init() {
 
   if ( isinit)
       return true;
 
   try {
       //initialize SDL mixer
       int audio_rate = 44100;
       Uint16 audio_format = AUDIO_S16SYS;
       int audio_channels = 2;
       int audio_buffers = 512;
 
       if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
           throw "Error while initializing SDL";
 
       Mix_AllocateChannels(NUMOFSOUNDS);
 
       //load wav files
       int i;
       for (i=0;i<NUMOFSOUNDS;i++) {
           std::string path = "/" + soundpaths[i];
           sounds[i] = Mix_LoadWAV(path.c_str());
           if ( sounds[i] == NULL )
               throw Mix_GetError();
       }
 
       isinit = true;
   }
   catch(const char * s){
       std::cout<<s<<'\n';
   }
   catch ( ... ) {
       std::cerr << "Unexpected exception\n";
   }
   return true;
}
 
Sounds::Sounds() :
       on(true), isinit(false)
{
    
   //set sound paths
   soundpaths[0] = "assets/sounds/intro.wav";
   soundnames[0] = "start";
   soundpaths[1] = "assets/sounds/clock.wav";
   soundnames[1] = "clock";
   soundpaths[2] = "assets/sounds/coin_eat.wav";
   soundnames[2] = "coin";
   soundpaths[3] = "assets/sounds/time_eat.wav";
   soundnames[3] = "time";
   soundpaths[4] = "assets/sounds/death.wav";
   soundnames[4] = "death";
   soundpaths[5] = "assets/sounds/freeze.wav";
   soundnames[5] = "freeze";
   soundpaths[6] = "assets/sounds/shoot.wav";
   soundnames[6] = "shoot";
   soundpaths[7] = "assets/sounds/level_end.wav";
   soundnames[7] = "level_end";
   
}
 
Sounds::~Sounds()
{
   Mix_CloseAudio();
}
 
