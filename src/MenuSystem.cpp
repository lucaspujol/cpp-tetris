#include "MenuSystem.hpp"
#include <iostream>

#define WIN_HEIGHT  1080
#define WIN_WIDTH   1920

MenuSystem::MenuSystem() : game(nullptr), currentState(START_MENU), quit(false) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    rendererWrapper = new Renderer(renderer);
}

MenuSystem::~MenuSystem() {
    if (game) {
        delete game;
    }
    delete rendererWrapper;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MenuSystem::run() {
    while (!quit) {
        handleInput();
        update();
        render();
        SDL_Delay(16); // plus ou moins 60 FPS
    }
}

void MenuSystem::handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
            return;
        }

        switch (currentState) {
            case START_MENU:
                handleStartMenuInput(e);
                break;
            case PLAYING:
                handleGameInput(e);
                break;
            case PAUSED:
                handlePausedInput(e);
                break;
            case GAME_OVER:
                handleGameOverInput(e);
                break;
        }
    }
}

void MenuSystem::handleStartMenuInput(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_r:
                startNewGame();
                break;
            case SDLK_ESCAPE:
                quit = true;
                break;
        }
    }
}

void MenuSystem::handleGameInput(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_p:
                pauseGame();
                return;
            case SDLK_ESCAPE:
                resetToMenu();
                return;
        }
    }
    // les autres inputs sont dans la class Game
    if (game) {
        game->handleInputEvent(e);
        if (game->isGameOver()) {
            currentState = GAME_OVER;
        }
    }
}

void MenuSystem::handlePausedInput(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_p:
                resumeGame();
                break;
            case SDLK_ESCAPE:
                resetToMenu();
                break;
        }
    }
}

void MenuSystem::handleGameOverInput(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_r:
                startNewGame();
                break;
            case SDLK_ESCAPE:
                resetToMenu();
                break;
        }
    }
}

void MenuSystem::update() {
    if (currentState == PLAYING && game) {
        game->update();
        if (game->isGameOver()) {
            currentState = GAME_OVER;
        }
    }
}

void MenuSystem::render() {
    // les deux lignes suivantes sont pour éviter de redessiner l'écran
    // si l'état n'a pas changé, on ne dedraw pas l'écran
    // sinon l'écran clignote et c'est moche
    static bool hasRenderedStaticScreen = false;
    bool isStaticState = (currentState == PAUSED || currentState == GAME_OVER);

    if (!isStaticState || !hasRenderedStaticScreen) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        switch (currentState) {
            case START_MENU:
                renderStartMenu();
                hasRenderedStaticScreen = false;
                break;
            case PLAYING:
                if (game) {
                    game->render();
                }
                hasRenderedStaticScreen = false;
                break;
            case PAUSED:
                if (game) {
                    game->render();
                }
                renderPausedMenu();
                hasRenderedStaticScreen = true;
                break;
            case GAME_OVER:
                if (game) {
                    game->render();
                }
                renderGameOverMenu();
                hasRenderedStaticScreen = true;
                break;
        }

        SDL_RenderPresent(renderer);
    }
}

void MenuSystem::renderStartMenu() {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    for (int y = 0; y < windowHeight; y++) {
        int r = 20 + (y * 40 / windowHeight);
        int g = 0;
        int b = 60 + (y * 60 / windowHeight);
        
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
    }

    SDL_Color titleColor = { 255, 255, 0, 255 };
    rendererWrapper->renderTextCentered("TETRIS", windowWidth / 2, windowHeight / 6, titleColor, 220);

    SDL_Color instructionColor = { 255, 255, 255, 255 };
    rendererWrapper->renderTextCentered("Start Game", windowWidth / 2, windowHeight / 2 - 50, instructionColor, 40);
    
    rendererWrapper->renderTextCentered("Quit", windowWidth / 2, windowHeight / 2 + 50, instructionColor, 40);
}

void MenuSystem::renderPausedMenu() {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    // overlay semi transparent
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = { 0, 0, windowWidth, windowHeight };
    SDL_RenderFillRect(renderer, &overlay);

    // panel
    SDL_Rect pausePanel = { windowWidth / 2 - 150, windowHeight / 2 - 100, 300, 200 };
    SDL_SetRenderDrawColor(renderer, 40, 40, 80, 255);
    SDL_RenderFillRect(renderer, &pausePanel);
    
    SDL_SetRenderDrawColor(renderer, 180, 180, 200, 255);
    SDL_RenderDrawRect(renderer, &pausePanel);

    SDL_Rect pauseTextRect = { windowWidth / 2 - 50, windowHeight / 2 - 60, 0, 0 };
    SDL_Color pauseColor = { 255, 255, 0, 255 };
    rendererWrapper->renderText("PAUSED", pauseTextRect, pauseColor);

    // instructions
    SDL_Rect instructionRect = { windowWidth / 2 - 80, windowHeight / 2 - 20, 0, 0 };
    SDL_Color instructionColor = { 255, 255, 255, 255 };
    rendererWrapper->renderText("Press P to Resume", instructionRect, instructionColor);
    
    instructionRect.y += 30;
    rendererWrapper->renderText("Press ESC for Menu", instructionRect, instructionColor);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void MenuSystem::renderGameOverMenu() {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    // overlay semi transparent
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = { 0, 0, windowWidth, windowHeight };
    SDL_RenderFillRect(renderer, &overlay);

    // panel
    SDL_Rect gameOverPanel = { windowWidth / 2 - 150, windowHeight / 2 - 100, 300, 200 };
    SDL_SetRenderDrawColor(renderer, 80, 40, 40, 255);
    SDL_RenderFillRect(renderer, &gameOverPanel);
    
    SDL_SetRenderDrawColor(renderer, 200, 180, 180, 255);
    SDL_RenderDrawRect(renderer, &gameOverPanel);

    SDL_Rect gameOverTextRect = { windowWidth / 2 - 80, windowHeight / 2 - 60, 0, 0 };
    SDL_Color gameOverColor = { 255, 100, 100, 255 };
    rendererWrapper->renderText("GAME OVER", gameOverTextRect, gameOverColor);

    // instructions
    SDL_Rect instructionRect = { windowWidth / 2 - 80, windowHeight / 2 - 20, 0, 0 };
    SDL_Color instructionColor = { 255, 255, 255, 255 };
    rendererWrapper->renderText("Press R to Restart", instructionRect, instructionColor);
    
    instructionRect.y += 30;
    rendererWrapper->renderText("Press ESC for Menu", instructionRect, instructionColor);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void MenuSystem::startNewGame() {
    if (game) {
        delete game;
    }
    game = new Game(rendererWrapper);
    currentState = PLAYING;
    render();
}

void MenuSystem::pauseGame() {
    if (currentState == PLAYING) {
        currentState = PAUSED;
    }
}

void MenuSystem::resumeGame() {
    if (currentState == PAUSED) {
        currentState = PLAYING;
        render();
    }
}

void MenuSystem::resetToMenu() {
    if (game) {
        delete game;
        game = nullptr;
    }
    currentState = START_MENU;
    render();
}
