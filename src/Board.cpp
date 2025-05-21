#include "Board.hpp"

Board::Board() {
    grid.resize(WIDTH, std::vector<int>(HEIGHT, 0));
}

bool Board::isValidPosition(const Piece &piece, int posX, int posY) const {
    const auto &shape = piece.getShape();
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            if (shape[x][y]) {
                int boardX = posX + x;
                int boardY = posY + y;
                if (boardX < 0 || boardX >= WIDTH || boardY < 0 || boardY >= HEIGHT)
                    return false;
                if (grid[boardX][boardY])
                    return false;
            }
        }
    }
    return true;
}

int Board::findDropPosition(const Piece &piece, int posX, int posY) const {
    int dropY = posY;

    while (isValidPosition(piece, posX, dropY + 1)) {
        dropY++;
    }
    return dropY;    
}

void Board::placePiece(const Piece &piece, int posX, int posY) {
    const auto &shape = piece.getShape();
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            if (shape[x][y]) {
                int boardX = posX + x;
                int boardY = posY + y;
                
                if (boardX >= 0 && boardX < WIDTH && boardY >= 0 && boardY < HEIGHT) {
                    grid[boardX][boardY] = 1;
                }
            }
        }
    }
    clearFullLines();
}

void Board::clearFullLines() {
    for (int y = 0; y < HEIGHT; ++y) {
        bool full = true;
        for (int x = 0; x < WIDTH; ++x) {
            if (!grid[x][y]) {
                full = false;
                break;
            }
        }
        if (full) {
            for (int yy = y; yy > 0; --yy) {
                for (int x = 0; x < WIDTH; ++x) {
                    grid[x][yy] = grid[x][yy - 1];
                }
            }
            for (int x = 0; x < WIDTH; ++x) {
                grid[x][0] = 0;
            }
        }
    }
}

const std::vector<std::vector<int>>& Board::getGrid() const {
    return grid;
}