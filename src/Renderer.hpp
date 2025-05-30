#ifndef _RENDERER_
#define _RENDERER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

#include "Board.hpp"
#include "Piece.hpp"

#define FONT_PATH "./assets/fonts/OpenSans-Bold.ttf"

class Renderer {
    public:
        Renderer(SDL_Renderer *r);
        ~Renderer();
        void drawBoard(const Board &board, const Piece &piece, int x, int y, const std::vector<Piece> &nextPieces, const Piece* heldPiece = nullptr);
        void renderText(const char* text, SDL_Rect destRect, SDL_Color color = {255, 255, 255, 255}, int fontSize = 0);
        void renderTextCentered(const char* text, int x, int y, SDL_Color color, int fontSize = 0);
        void drawMainMenu(int windowWidth, int windowHeight,
            const SDL_Rect &startButtonRect, const SDL_Rect &quitButtonRect,
            bool isStartButtonHovered, bool isQuitButtonHovered);
        void drawPauseMenu(int windowWidth, int windowHeight);
        void drawGameOverMenu(int windowWidth, int windowHeight);
        void drawGradientBackground(int windowWidth, int windowHeight, bool isPurpleTheme = true);
    
    private:
        SDL_Renderer *renderer;
        TTF_Font *font;
        const int blockSize = 45;

        void drawPiece(const Piece &piece, int offsetX, int offsetY, int size, bool isGhost = false);
        void setPieceColor(char pieceType, bool isGhost = false);
        void drawBoardGrid(const Board &board, int offsetX, int offsetY);
        void drawGhostPiece(const Board &board, const Piece &piece, int posX, int posY, int offsetX = 0, int offsetY = 0);
        void drawNextPiecesPanel(const std::vector<Piece> &nextPieces, int panelX, int panelY, int nextPieceSize);
        void drawHeldPiecePanel(const Piece* heldPiece, int panelX, int panelY, int heldPieceSize);
        void drawScorePanel(int score, int level);

        // Score panel helper methods
        float calculateScorePulseIntensity(int score);
        SDL_Rect calculateScorePanelPosition();
        void drawScorePanelBackground(const SDL_Rect& scorePanel, float pulseIntensity);
        void drawScorePanelBorders(const SDL_Rect& scorePanel);
        void drawScoreSection(const SDL_Rect& scorePanel, int score, float pulseIntensity);
        void drawDivider(const SDL_Rect& scorePanel);
        void drawLevelSection(const SDL_Rect& scorePanel, int level);
        void drawLevelIndicatorDots(const SDL_Rect& scorePanel, int level);
};

#endif /* _RENDERER_ */
