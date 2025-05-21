#ifndef _BOARD_
    #define _BOARD_
#include "Piece.hpp"
#include <vector>

class Board {
    public:
        static constexpr int WIDTH = 10;
        static constexpr int HEIGHT = 20;

        Board();
        bool isValidPosition(const Piece &piece, int x, int y) const;
        int findDropPosition(const Piece &piece, int x, int y) const;
        void placePiece(const Piece &piece, int x, int y);
        void clearFullLines();
        const std::vector<std::vector<char>>& getGrid() const;
    private:
        std::vector<std::vector<char>> grid;
};
#endif /* _BOARD_ */
