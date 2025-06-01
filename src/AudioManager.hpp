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
        
        // Volume controls (0-100 range, will be converted to 0-128 for SDL_mixer)
        void setMusicVolume(int volume);
        void setSoundVolume(int volume);
        int getMusicVolume() const;
        int getSoundVolume() const;

    private:
        Mix_Music *backgroundMusic;
        std::unordered_map<SoundEffect, Mix_Chunk *> soundEffects;
        int musicVolume;  // (0-100)
        int soundVolume;  // (0-100)

        bool loadSounds();
        bool loadMusic();
};
#endif /* __AUDIO_MANAGER__ */
