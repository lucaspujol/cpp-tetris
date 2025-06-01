#ifndef __AUDIO_MANAGER__
#define __AUDIO_MANAGER__

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>

class AudioManager {
    public:
        enum SoundEffect {
            ROTATE,
            PLACE,
            LINE_CLEAR,
            GAME_OVER
        };

        AudioManager();
        ~AudioManager();

        bool init();
        void playMusic();
        void pauseMusic();
        void resumeMusic();
        void stopMusic();
        void playSound(SoundEffect effect);

    private:
        Mix_Music *backgroundMusic;
        std::unordered_map<SoundEffect, Mix_Chunk *> soundEffects;

        bool loadSounds();
        bool loadMusic();
};
#endif /* __AUDIO_MANAGER__ */
