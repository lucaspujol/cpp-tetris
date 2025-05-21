#ifndef _RENDERER_
    #define _RENDERER_

#include <SDL2/SDL.h>
#include "Board.hpp"
#include "Piece.hpp"

class Renderer {
    public:
        Renderer(SDL_Renderer *r);
        void draw(const Board &board, const Piece &piece, int x, int y);

    private:
        SDL_Renderer *renderer;
        const int blockSize = 45;
};

#endif /* _RENDERER_ */
