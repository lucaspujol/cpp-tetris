#ifndef _RENDERER_
    #define _RENDERER_

#include <SDL2/SDL.h>
#include "Board.hpp"
#include "Piece.hpp"

class Renderer {
    public:
        Renderer(SDL_Renderer *r);
        void draw_board(const Board &board, const Piece &piece, int x, int y, const std::vector<Piece> &nextPieces);

    private:
        SDL_Renderer *renderer;
        const int blockSize = 45;
        void drawPiece(const Piece &piece, int offsetX, int offsetY, int size, bool isGhost = false);
        void setPieceColor(char pieceType, bool isGhost = false);
};

#endif /* _RENDERER_ */
