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
    catch ( ... ) {
        std::cerr << "Unexpected exception";
    }
    return true;
}

Sounds::Sounds() :
        on(true), isinit(false)
{
    //set sound paths
    soundpaths[0] = "sound/intro.wav";
    soundpaths[1] = "sound/munch_a.wav";
    soundpaths[2] = "sound/munch_b.wav";
    soundpaths[3] = "sound/large_pellet.wav";
    soundpaths[4] = "sound/ghost_eat.wav";
    soundpaths[5] = "sound/fruit.wav";
    soundpaths[6] = "sound/extra_man.wav";
    soundpaths[7] = "sound/vuln.wav";
    soundpaths[8] = "sound/death.wav";
    soundpaths[9] = "sound/newgame.wav";
    // soundpaths[10] = "sound/siren.wav";
    // soundpaths[11] = "sound/intermission.wav";
    // soundpaths[12] = "sound/booster.wav";
}

Sounds::~Sounds()
{
    // Mix_CloseAudio();
}
