#ifndef _BOARD_
    #define _BOARD_
#include "Piece.hpp"
#include <vector>

class AudioManager;
class Game;

class Board {
    public:
        static constexpr int WIDTH = 10;
        static constexpr int HEIGHT = 20;

        Board();
        void setAudioManager(AudioManager *manager);
        bool isValidPosition(const Piece &piece, int x, int y) const;
        int findDropPosition(const Piece &piece, int x, int y) const;
        void placePiece(const Piece &piece, int x, int y);
        int clearFullLines();
        const std::vector<std::vector<char>>& getGrid() const;
        int getScore() const;
        void setScore(int score);
        int getLevel() const;
        void updateScore(int lines);
        void setLevel(int level);
    private:
        std::vector<std::vector<char>> grid;
        int linesCleared;
        int currentLevel;
        int score;
        AudioManager *audioManager;
};
#endif /* _BOARD_ */
