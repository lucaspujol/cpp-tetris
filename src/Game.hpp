#ifndef _GAME_
    #define _GAME_
#include <SDL2/SDL.h>
#include "Board.hpp"
#include "Piece.hpp"
#include "AudioManager.hpp"
#include <vector>
#define WIN_HEIGHT  1080
#define WIN_WIDTH   1920
#define WAIT_TIME   500
#define NEXT_PIECE_COUNT 4

class Renderer;

class Game {
    public:
        Game();
        Game(Renderer* externalRenderer);
        ~Game();
        
        // Menu system interface
        void update();
        void render();
        void handleInputEvent(SDL_Event &e);
        bool isGameOver() const;
        
        // void run();

    private:
        Renderer *rendererWrapper;
        SDL_Window *window;
        SDL_Renderer *renderer;
        AudioManager audioManager;
        bool ownsSdlResources;

        Board board;
        Piece currentPiece;
        std::vector<Piece> nextPieces;
        Piece* heldPiece;
        bool hasHeldPiece;
        bool canHold;
        int pieceX, pieceY;

        bool quit;
        bool gameOver;

        void handleInput();
        void spawnNewPiece();
        void holdPiece();
        bool tryWallKicks();
        Piece::Tetromino getRandomTetromino();
        void initNextPieces();
        void printNextPiece();
        int getDropDelay() const;
};

#endif /* _GAME_ */
