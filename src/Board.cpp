#include "Board.hpp"
#include "AudioManager.hpp"
#include <iostream>

Board::Board() {
    grid.resize(WIDTH, std::vector<char>(HEIGHT, 0));
    score = 0;
    currentLevel = 1;
    linesCleared = 0;
    audioManager = nullptr;
}

void Board::setAudioManager(AudioManager *manager) {
    audioManager = manager;
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
                if (grid[boardX][boardY] != 0)
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

void Board::updateScore(int lines) {
    int linePoints = 0;

    switch (lines) {
        case 0:
            linePoints = 0;
            break;
        case 1:
            linePoints = 40 * (currentLevel + 1);
            break;
        case 2:
            linePoints = 100 * (currentLevel + 1);
            break;
        case 3:
            linePoints = 300 * (currentLevel + 1);
            break;
        default:
            linePoints = 1200 * (currentLevel + 1);
            break;
    }
    score += linePoints;
    linesCleared += lines;
    if (linesCleared >= 10) {
        currentLevel++;
        linesCleared = 0;
    }
}

void Board::placePiece(const Piece &piece, int posX, int posY) {
    const auto &shape = piece.getShape();
    char pieceType = piece.getType();
    
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            if (shape[x][y]) {
                int boardX = posX + x;
                int boardY = posY + y;
                
                if (boardX >= 0 && boardX < WIDTH && boardY >= 0 && boardY < HEIGHT) {
                    grid[boardX][boardY] = pieceType;
                }
            }
        }
    }
    int lines = clearFullLines();
    if (lines > 0) {
        updateScore(lines);
    }
}

int Board::clearFullLines() {
    int linesCleared = 0;
    std::vector<int> fullLines;
    for (int y = 0; y < HEIGHT; ++y) {
        bool full = true;
        for (int x = 0; x < WIDTH; ++x) {
            if (grid[x][y] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            fullLines.push_back(y);
        }
    }
    linesCleared = fullLines.size();
    for (int fullY : fullLines) {
        for (int y = fullY; y > 0; --y) {
            for (int x = 0; x < WIDTH; ++x) {
                grid[x][y] = grid[x][y - 1];
            }
        }
        for (int x = 0; x < WIDTH; ++x) {
            grid[x][0] = 0;
        }
    }

    try {
        if (linesCleared > 0 && audioManager != nullptr) {
            audioManager->playSound(AudioManager::LINE_CLEAR);
        }
    } catch (...) {
        std::cerr << "Warning: Exception when playing line clear sound" << std::endl;
    }

    return linesCleared;
}

const std::vector<std::vector<char>>& Board::getGrid() const {
    return grid;
}

void Board::setLevel(int level) {
    if (level > 0) 
        currentLevel = level;
}

void Board::setScore(int newScore) {
    score = newScore;
}

int Board::getScore() const {
    return score;
}

int Board::getLevel() const {
    return currentLevel;
}
