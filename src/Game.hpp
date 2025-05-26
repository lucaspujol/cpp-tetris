#ifndef _GAME_
    #define _GAME_
#include <SDL2/SDL.h>
#include "Board.hpp"
#include "Piece.hpp"
#include <vector>
#define WIN_HEIGHT  1080
#define WIN_WIDTH   1920
#define WAIT_TIME   500
#define NEXT_PIECE_COUNT 4

class Game {
    public:
        Game();
        ~Game();
        void run();

    private:
        SDL_Window *window;
        SDL_Renderer *renderer;

        Board board;
        Piece currentPiece;
        std::vector<Piece> nextPieces;
        Piece* heldPiece;
        bool hasHeldPiece;
        bool canHold;
        int pieceX, pieceY;

        bool quit;
        int score = 0;
        int level = 1;

        void handleInput();
        void update();
        void render();
        void spawnNewPiece();
        void holdPiece();
        bool tryWallKicks();
        Piece::Tetromino getRandomTetromino();
        void initNextPieces();
        void printNextPiece();
        int getDropDelay() const;
};

#endif /* _GAME_ */
