#include "Game.hpp"
#include "Renderer.hpp"
#include <cstdlib>
#include "Board.hpp"
#include <ctime>
#include <SDL2/SDL.h>
#include <iostream>
#include "Board.hpp"

Renderer *rendererWrapper = nullptr;

Piece::Tetromino Game::getRandomTetromino() {
    return static_cast<Piece::Tetromino>(rand() % TETROMINO_COUNT);
}

void Game::initNextPieces() {
    nextPieces.clear();
    for (int i = 0; i < NEXT_PIECE_COUNT; i++) {
        nextPieces.push_back(Piece(getRandomTetromino()));
    }
}

void Game::printNextPiece() {
    std::cout << "Next pieces: ";
    for (const auto &piece : nextPieces) {
        std::cout << piece.getType() << " ";
    }
    std::cout << std::endl;
}

Game::Game() : currentPiece(Piece::I), nextPieces() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    rendererWrapper = new Renderer(renderer);
    std::srand(std::time(nullptr));
    quit = false;
    initNextPieces();
    spawnNewPiece();
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete rendererWrapper;
}

void Game::handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            quit = true;
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    if (board.isValidPosition(currentPiece, pieceX - 1, pieceY))
                        pieceX--;
                    break;
                case SDLK_RIGHT:
                    if (board.isValidPosition(currentPiece, pieceX + 1, pieceY))
                        pieceX++;
                    break;
                case SDLK_DOWN:
                    if (board.isValidPosition(currentPiece, pieceX, pieceY + 1))
                        pieceY++;
                    break;
                case SDLK_UP:
                    {
                        Piece tempPiece = currentPiece;
                        
                        currentPiece.rotate();
                        if (!board.isValidPosition(currentPiece, pieceX, pieceY)) {
                            if (!tryWallKicks()) {
                                currentPiece = tempPiece;
                            }
                        }
                    }
                    break;
                case SDLK_SPACE:
                    {
                        int dropY = board.findDropPosition(currentPiece, pieceX, pieceY);
                        if (board.isValidPosition(currentPiece, pieceX, dropY)) {
                            pieceY = dropY;
                            board.placePiece(currentPiece, pieceX, pieceY);
                            spawnNewPiece();
                        }
                        
                    }
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::update() {
    static Uint32 lastTick = SDL_GetTicks();
    
    if (SDL_GetTicks() - lastTick > WAIT_TIME) {
        if (board.isValidPosition(currentPiece, pieceX, pieceY + 1)) {
            pieceY++;
        } else {
            board.placePiece(currentPiece, pieceX, pieceY);
            spawnNewPiece();
        }
        lastTick = SDL_GetTicks();
    }
}

void Game::render() {
    rendererWrapper->draw(board, currentPiece, pieceX, pieceY);
}

void Game::spawnNewPiece() {
    pieceX = (Board::WIDTH / 2) - 2;
    pieceY = 0;
    
    currentPiece = nextPieces[0];
    
    for (int i = 0; i < NEXT_PIECE_COUNT - 1; i++) {
        nextPieces[i] = nextPieces[i + 1];
    }
    
    nextPieces[NEXT_PIECE_COUNT - 1] = Piece(getRandomTetromino());
    
    // printNextPiece();
    if (!board.isValidPosition(currentPiece, pieceX, pieceY))
        quit = true;
}

bool Game::tryWallKicks() {
    // define tout les offests de wall kicks
    const std::pair<int, int> kicks[] = {
        {-1, 0},  // gauche
        {1, 0},   // droite
        {-2, 0},  // 2 a gauche
        {2, 0},   // 2 a droite
        {0, -1},  // en haut
        {0, 1},   // en bas
        {-1, -1}, // en haut a gauche
        {1, -1}   // en haut a droite
    };

    for (size_t i = 0; i < sizeof(kicks) / sizeof(kicks[0]); ++i) {
        int xOffset = kicks[i].first;
        int yOffset = kicks[i].second;
        if (board.isValidPosition(currentPiece, pieceX + xOffset, pieceY + yOffset)) {
            pieceX += xOffset;
            pieceY += yOffset;
            return true;
        }
    }
    // pas de position valide
    return false;
}

void Game::run() {
    while (!quit)  {
        handleInput();
        update();
        render();
        SDL_Delay(100);
    }
}