#ifndef _GAME_
    #define _GAME_
#include <SDL2/SDL.h>
#include "Board.hpp"
#include "Piece.hpp"
#define WIN_HEIGHT  1080
#define WIN_WIDTH   1920
#define WAIT_TIME   500

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
        int pieceX, pieceY;

        bool quit;

        void handleInput();
        void update();
        void render();
        void spawnNewPiece();
        bool tryWallKicks();
};
#endif /* _GAME_ */
