#ifndef _PIECE_
    #define _PIECE_
#include <array>
#define TETROMINO_COUNT 7

class Piece {
    public:
        enum Tetromino { I, O, T, S, Z, J, L };
        using Shape = std::array<std::array<int, 5>, 5>;

        Piece() : Piece(I) {}
        Piece(Tetromino type);
        void rotate();
        const Shape &getShape() const;
        char getType() const;
    
    private:
        std::array<Shape, 4> rotations;
        int currentRotation;
        Tetromino tetrominoType;
};

#endif /* _PIECE_ */
