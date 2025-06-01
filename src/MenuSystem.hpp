#ifndef _MENU_SYSTEM_
#define _MENU_SYSTEM_

#include <SDL2/SDL.h>
#include "Game.hpp"
#include "Renderer.hpp"
#include "AudioManager.hpp"

class MenuSystem {
public:
    enum State {
        START_MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };

    MenuSystem();
    ~MenuSystem();
    void run();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Renderer *rendererWrapper;
    Game *game;
    AudioManager audioManager;

    State currentState;
    bool quit;

    SDL_Rect startButtonRect;
    SDL_Rect quitButtonRect;
    bool isStartButtonHovered = false;
    bool isQuitButtonHovered = false;
    bool hasRenderedStaticScreen = false;

    void handleInput();
    void update();
    void render();

    void handleStartMenuInput(SDL_Event &e);
    void handleGameInput(SDL_Event &e);
    void handlePausedInput(SDL_Event &e);
    void handleGameOverInput(SDL_Event &e);

    void renderStartMenu();
    void renderPausedMenu();
    void renderGameOverMenu();

    void startNewGame();
    void pauseGame();
    void resumeGame();
    void resetToMenu();
};

#endif /* _MENU_SYSTEM_ */
