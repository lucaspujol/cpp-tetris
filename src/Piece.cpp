#include "Piece.hpp"

Piece::Piece(Tetromino type) : currentRotation(0) {
    // definir tout les tetrominos
    switch (type) {
        case I:
            // I tetromino (line)
            rotations[0] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 1, 1, 1},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[1] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0}
            }};
            rotations[2] = rotations[0];
            rotations[3] = rotations[1];
            break;
        case O:
            // O tetromino (square)
            rotations[0] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[1] = rotations[0];
            rotations[2] = rotations[0];
            rotations[3] = rotations[0];
            break;
        case T:
            // T tetromino
            rotations[0] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[1] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[2] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[3] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            break;
        case S:
            // S tetromino
            rotations[0] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[1] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[2] = rotations[0];
            rotations[3] = rotations[1];
            break;
        case Z:
            // Z tetromino
            rotations[0] = {{
                {0, 0, 0, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[1] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[2] = rotations[0];
            rotations[3] = rotations[1];
            break;
        case J:
            // J tetromino
            rotations[0] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[1] = {{
                {0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[2] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[3] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0}
            }};
            break;
        case L:
            // L tetromino
            rotations[0] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[1] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[2] = {{
                {0, 0, 0, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            rotations[3] = {{
                {0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }};
            break;
    }
}

void Piece::rotate() {
    currentRotation = (currentRotation + 1) % 4;
}

const Piece::Shape &Piece::getShape() const {
    return rotations[currentRotation];
}