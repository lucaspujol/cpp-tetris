#include "AudioManager.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <linux/limits.h>

AudioManager::AudioManager() : backgroundMusic(nullptr) {
    soundEffects.clear();
}

AudioManager::~AudioManager() {
    try {
        for (auto &pair : soundEffects) {
            if (pair.second) {
                Mix_FreeChunk(pair.second);
                pair.second = nullptr;
            }
        }
        soundEffects.clear();
        
        if (backgroundMusic) {
            Mix_FreeMusic(backgroundMusic);
            backgroundMusic = nullptr;
        }
        
        Mix_CloseAudio();
        while(Mix_Init(0)) {
            Mix_Quit();
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception during AudioManager cleanup: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception during AudioManager cleanup" << std::endl;
    }
}

bool AudioManager::init() {
    try {
        if (SDL_WasInit(SDL_INIT_AUDIO) == 0) {
            if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
                std::cerr << "SDL Audio subsystem could not initialize! SDL Error: " << SDL_GetError() << std::endl;
                return false;
            }
        }
        
        int flags = MIX_INIT_MP3;
        int initted = Mix_Init(flags);
        if ((initted & flags) != flags) {
            std::cerr << "Mix_Init: Failed to init required mp3 support! SDL_Mixer Error: " << Mix_GetError() << std::endl;
        }
        
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_Mixer could not initialize! SDL_Mixer Error: " << Mix_GetError() << std::endl;
            return false;
        }
        
        bool soundsLoaded = loadSounds();
        bool musicLoaded = loadMusic();
        
        if (!soundsLoaded) {
            std::cerr << "Warning: Failed to load some sound effects!" << std::endl;
        }
        
        if (!musicLoaded) {
            std::cerr << "Warning: Failed to load background music!" << std::endl;
        }
        
        return soundsLoaded || musicLoaded;
    } catch (const std::exception& e) {
        std::cerr << "Exception during audio initialization: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Unknown exception during audio initialization" << std::endl;
        return false;
    }
}

bool AudioManager::loadSounds() {
    try {
        char currentPath[PATH_MAX];
        if (getcwd(currentPath, sizeof(currentPath)) == nullptr) {
            std::cerr << "Failed to get current working directory" << std::endl;
            return false;
        }
        
        std::string basePath = std::string(currentPath) + "/assets/sounds/";
        std::cout << "Loading sounds from: " << basePath << std::endl;
        
        const struct {
            SoundEffect effect;
            const char* filename;
        } soundFiles[] = {
            { ROTATE, "tetromino_rotates.wav" },
            { PLACE, "place.wav" },
            { LINE_CLEAR, "line_cleared.wav" },
            { GAME_OVER, "game_over.wav" }
        };
        
        bool allSoundsLoaded = true;
        
        for (const auto& soundFile : soundFiles) {
            std::string fullPath = basePath + soundFile.filename;
            std::cout << "Loading sound: " << fullPath << std::endl;
            
            Mix_Chunk* sound = Mix_LoadWAV(fullPath.c_str());
            if (!sound) {
                std::cerr << "Failed to load sound " << soundFile.filename 
                          << "! SDL_Mixer Error: " << Mix_GetError() << std::endl;
                allSoundsLoaded = false;
            } else {
                soundEffects[soundFile.effect] = sound;
            }
        }
        
        return allSoundsLoaded;
    } catch (const std::exception& e) {
        std::cerr << "Exception during loadSounds(): " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Unknown exception during loadSounds()" << std::endl;
        return false;
    }
}

bool AudioManager::loadMusic() {
    try {
        char currentPath[PATH_MAX];
        if (getcwd(currentPath, sizeof(currentPath)) == nullptr) {
            std::cerr << "Failed to get current working directory" << std::endl;
            return false;
        }
        
        std::string basePath = std::string(currentPath) + "/assets/sounds/";
        std::string musicPath = basePath + "background_music.mp3";
        std::cout << "Loading music from: " << musicPath << std::endl;
        
        backgroundMusic = Mix_LoadMUS(musicPath.c_str());
        if (!backgroundMusic) {
            std::cerr << "Failed to load background music! SDL_Mixer Error: " << Mix_GetError() << std::endl;
            return false;
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception during loadMusic(): " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Unknown exception during loadMusic()" << std::endl;
        return false;
    }
}

void AudioManager::playMusic() {
    if (backgroundMusic && Mix_PlayingMusic() == 0) {
        if (Mix_PlayMusic(backgroundMusic, -1) == -1) {
            std::cerr << "Warning: Could not play music: " << Mix_GetError() << std::endl;
        }
    }
}

void AudioManager::pauseMusic() {
    try {
        if (Mix_PlayingMusic() == 1) {
            Mix_PauseMusic();
        }
    } catch (...) {
        std::cerr << "Warning: Exception while pausing music" << std::endl;
    }
}

void AudioManager::resumeMusic() {
    try {
        if (Mix_PausedMusic() == 1) {
            Mix_ResumeMusic();
        }
    } catch (...) {
        std::cerr << "Warning: Exception while resuming music" << std::endl;
    }
}

void AudioManager::stopMusic() {
    try {
        Mix_HaltMusic();
    } catch (...) {
        std::cerr << "Warning: Exception while stopping music" << std::endl;
    }
}

void AudioManager::playSound(SoundEffect effect) {
    try {
        auto it = soundEffects.find(effect);
        if (it != soundEffects.end() && it->second != nullptr) {
            if (Mix_PlayChannel(-1, it->second, 0) == -1) {
                std::cerr << "Warning: Could not play sound effect: " << Mix_GetError() << std::endl;
            }
        } else {
            std::cerr << "Warning: Attempted to play unavailable sound effect (ID: " << effect << ")" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception during playSound(): " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception during playSound()" << std::endl;
    }
}
