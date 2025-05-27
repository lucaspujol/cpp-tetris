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
        if (quit)
            break;
        update();
        render();
        SDL_Delay(16); // ~60 FPS
    }
}

void MenuSystem::handleInput() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if (currentState == START_MENU) {
        isStartButtonHovered = (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
            mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h);
        isQuitButtonHovered = (mouseX >= quitButtonRect.x && mouseX <= quitButtonRect.x + quitButtonRect.w &&
            mouseY >= quitButtonRect.y && mouseY <= quitButtonRect.y + quitButtonRect.h);
    }
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
            case SDLK_ESCAPE:
                quit = true;
                return;
            default:
                break;
        }
    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            if (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
                mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h) {
                startNewGame();
            }
            else if (mouseX >= quitButtonRect.x && mouseX <= quitButtonRect.x + quitButtonRect.w &&
                     mouseY >= quitButtonRect.y && mouseY <= quitButtonRect.y + quitButtonRect.h) {
                quit = true;
                return;
            }
        }
    }
}

void MenuSystem::handleGameInput(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
                pauseGame();
                return;
            default:
                break;
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
            case SDLK_ESCAPE:
                resumeGame();
                break;
            case SDLK_BACKSPACE:
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

    // Calculate button sizes for menu options
    TTF_Font* menuFont = TTF_OpenFont(FONT_PATH, 40);
    int startWidth, startHeight;
    TTF_SizeText(menuFont, "Start Game", &startWidth, &startHeight);
    
    int quitWidth, quitHeight;
    TTF_SizeText(menuFont, "Quit", &quitWidth, &quitHeight);
    TTF_CloseFont(menuFont);
    
    // Calculate button rectangles
    startButtonRect = {
        windowWidth / 2 - startWidth / 2 - 10,
        windowHeight / 2 - 50 - startHeight / 2 - 10,
        startWidth + 20,
        startHeight + 20
    };
    
    quitButtonRect = {
        windowWidth / 2 - quitWidth / 2 - 10,
        windowHeight / 2 + 50 - quitHeight / 2 - 10,
        quitWidth + 20,
        quitHeight + 20
    };
    
    // Use renderer to draw the menu
    rendererWrapper->drawMainMenu(windowWidth, windowHeight, 
                                startButtonRect, quitButtonRect, 
                                isStartButtonHovered, isQuitButtonHovered);
}

void MenuSystem::renderPausedMenu() {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
    rendererWrapper->drawPauseMenu(windowWidth, windowHeight);
}

void MenuSystem::renderGameOverMenu() {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
    rendererWrapper->drawGameOverMenu(windowWidth, windowHeight);
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
