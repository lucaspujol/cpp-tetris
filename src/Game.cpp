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
    heldPiece = nullptr;
    hasHeldPiece = false;
    canHold = true;
    initNextPieces();
    spawnNewPiece();
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete rendererWrapper;
    if (heldPiece != nullptr) {
        delete heldPiece;  // Clean up held piece memory
    }
}

int Game::getDropDelay() const {
    int level = board.getLevel();

    if (level == 0) return 48 * 16; // 48 frames
    else if (level == 1) return 43 * 16;
    else if (level == 2) return 38 * 16;
    else if (level == 3) return 33 * 16;
    else if (level == 4) return 28 * 16;
    else if (level == 5) return 23 * 16;
    else if (level == 6) return 18 * 16;
    else if (level == 7) return 13 * 16;
    else if (level == 8) return 8 * 16;
    else if (level == 9) return 6 * 16;
    else if (level >= 10 && level <= 12) return 5 * 16;
    else if (level >= 13 && level <= 15) return 4 * 16;
    else if (level >= 16 && level <= 18) return 3 * 16;
    else if (level >= 19 && level <= 28) return 2 * 16;
    else return 16; // 1 frame (level 29+)
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
                    board.setScore(board.getScore() + 1);
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
                case SDLK_RSHIFT:
                    if (canHold) {
                        holdPiece();
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
    int dropDelay = getDropDelay();
    
    if (SDL_GetTicks() - lastTick > static_cast<Uint32>(dropDelay)) {
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
    rendererWrapper->drawBoard(board, currentPiece, pieceX, pieceY, nextPieces, heldPiece);
}

void Game::spawnNewPiece() {
    pieceX = (Board::WIDTH / 2) - 2;
    pieceY = 0;
    
    currentPiece = nextPieces[0];
    
    for (int i = 0; i < NEXT_PIECE_COUNT - 1; i++) {
        nextPieces[i] = nextPieces[i + 1];
    }
    
    // nextPieces[NEXT_PIECE_COUNT - 1] = Piece::I;
    nextPieces[NEXT_PIECE_COUNT - 1] = Piece(getRandomTetromino());
    
    canHold = true;  // Reset hold availability for new piece
    
    // printNextPiece();
    if (!board.isValidPosition(currentPiece, pieceX, pieceY))
        quit = true;
}

void Game::holdPiece() {
    if (!canHold) {
        return;  // Can't hold twice in one drop
    }
    
    if (heldPiece == nullptr) {
        // First time holding - store current piece and spawn next
        heldPiece = new Piece(currentPiece);
        hasHeldPiece = true;
        spawnNewPiece();
    } else {
        // Swap current piece with held piece
        Piece tempPiece = currentPiece;
        currentPiece = *heldPiece;
        delete heldPiece;  // Clean up old held piece
        heldPiece = new Piece(tempPiece);
        
        // Reset piece position for swapped piece
        pieceX = (Board::WIDTH / 2) - 2;
        pieceY = 0;
        
        // Check if swapped piece fits at spawn position
        if (!board.isValidPosition(currentPiece, pieceX, pieceY)) {
            quit = true;  // Game over if held piece doesn't fit
        }
    }
    
    canHold = false;  // Prevent holding again until next piece
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
        SDL_Delay(16);
    }
}