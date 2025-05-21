#ifndef _RENDERER_
    #define _RENDERER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Board.hpp"
#include "Piece.hpp"

#define FONT_PATH "/home/lucas/perso/cpp-tetris/assets/fonts/OpenSans-Bold.ttf"

class Renderer {
    public:
        Renderer(SDL_Renderer *r);
        ~Renderer();
        void draw_board(const Board &board, const Piece &piece, int x, int y, const std::vector<Piece> &nextPieces);

    private:
        SDL_Renderer *renderer;
        TTF_Font *font;
        const int blockSize = 45;
        void drawPiece(const Piece &piece, int offsetX, int offsetY, int size, bool isGhost = false);
        void setPieceColor(char pieceType, bool isGhost = false);
        void renderText(const char* text, SDL_Rect destRect, SDL_Color color = {255, 255, 255, 255});
};

#endif /* _RENDERER_ */
