#include "Renderer.hpp"
#include <string>

Renderer::Renderer(SDL_Renderer *r) : renderer(r) {
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }
    
    font = TTF_OpenFont(FONT_PATH, 20);
    if (!font) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        printf("Tried multiple font paths but all failed.\n");
    }
}

Renderer::~Renderer() {
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
    TTF_Quit();
}

void Renderer::setPieceColor(char pieceType, bool isGhost) {
    if (isGhost) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        return;
    }
    
    switch (pieceType) {
        case 'I': SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);   break;  // cyan
        case 'O': SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);   break;  // jaune
        case 'T': SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);   break;  // violet
        case 'S': SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);     break;  // vert
        case 'Z': SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);     break;  // rouge
        case 'J': SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);     break;  // bleu
        case 'L': SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);   break;  // orange
        default:  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);     break;  // rouge par défaut
    }
}

void Renderer::drawPiece(const Piece &piece, int offsetX, int offsetY, int size, bool isGhost) {
    const auto &shape = piece.getShape();
    
    setPieceColor(piece.getType(), isGhost);
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            if (shape[x][y]) {
                SDL_Rect rect = { offsetX + x * size, offsetY + y * size, size, size };
                if (isGhost) {
                    SDL_RenderDrawRect(renderer, &rect);
                } else {
                    // save la couleur de base
                    Uint8 r, g, b, a;
                    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
                    SDL_RenderFillRect(renderer, &rect);

                    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                    SDL_RenderDrawRect(renderer, &rect);
                    
                    // restore la couleur pour le next block
                    SDL_SetRenderDrawColor(renderer, r, g, b, a);
                }
            }
        }
    }
}

void Renderer::renderText(const char* text, SDL_Rect destRect, SDL_Color color, int fontSize) {
    if (fontSize == 0) {
        if (!font)
            return;
            
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
        if (!textSurface) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }
        
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            SDL_FreeSurface(textSurface);
            return;
        }
        if (destRect.w == 0 || destRect.h == 0) {
            destRect.w = textSurface->w;
            destRect.h = textSurface->h;
        }
        SDL_RenderCopy(renderer, textTexture, NULL, &destRect);
        
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    } else {
        TTF_Font* tempFont = TTF_OpenFont(FONT_PATH, fontSize);
        if (!tempFont) {
            printf("Failed to load font with size %d! SDL_ttf Error: %s\n", fontSize, TTF_GetError());
            return;
        }
        
        SDL_Surface* textSurface = TTF_RenderText_Solid(tempFont, text, color);
        if (!textSurface) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            TTF_CloseFont(tempFont);
            return;
        }
        
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            SDL_FreeSurface(textSurface);
            TTF_CloseFont(tempFont);
            return;
        }
        if (destRect.w == 0 || destRect.h == 0) {
            destRect.w = textSurface->w;
            destRect.h = textSurface->h;
        }
        SDL_RenderCopy(renderer, textTexture, NULL, &destRect);
        
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        TTF_CloseFont(tempFont);
    }
}

void Renderer::renderTextCentered(char const *text, int x, int y, SDL_Color color, int fontSize) {
    TTF_Font *textFont = (fontSize == 0) ? font : TTF_OpenFont(FONT_PATH, fontSize);
    if (!textFont)
        return;
    int textWidth, textHeight;
    TTF_SizeText(textFont, text, &textWidth, &textHeight);

    SDL_Rect destRect = {
        x - textWidth / 2,
        y - textHeight / 2,
        0,
        0
    };
    if (fontSize == 0) {
        renderText(text, destRect, color);
    } else {
        renderText(text, destRect, color, fontSize);
        TTF_CloseFont(textFont);
    }
}

void Renderer::drawBoardGrid(const Board &board, int offsetX, int offsetY) {
    const auto &grid = board.getGrid();
    for (int x = 0; x < Board::WIDTH; ++x) {
        for (int y = 0; y < Board::HEIGHT; ++y) {
            if (grid[x][y] != 0) {
                if (y >= 0 && y < Board::HEIGHT) {
                    setPieceColor(grid[x][y]);
                    SDL_Rect rect = { offsetX + x * blockSize, offsetY + y * blockSize, blockSize, blockSize };
                    SDL_RenderFillRect(renderer, &rect);
                    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                    SDL_RenderDrawRect(renderer, &rect);
                }
            } else {
                // Fill empty cell with black
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_Rect rect = { offsetX + x * blockSize, offsetY + y * blockSize, blockSize, blockSize };
                SDL_RenderFillRect(renderer, &rect);
                // Draw dark grey outline
                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

void Renderer::drawGhostPiece(const Board &board, const Piece &piece, int posX, int posY, int offsetX, int offsetY) {
    int dropY = board.findDropPosition(piece, posX, posY);
    if (dropY > posY) {
        const auto &shape = piece.getShape();        
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
        setPieceColor(piece.getType(), false);
        
        for (int x = 0; x < 5; ++x) {
            for (int y = 0; y < 5; ++y) {
                if (shape[x][y]) {
                    int boardX = posX + x;
                    int boardY = dropY + y;
                    if (boardX >= 0 && boardX < Board::WIDTH && boardY >= 0 && boardY < Board::HEIGHT) {
                        SDL_Rect rect = { offsetX + boardX * blockSize, offsetY + boardY * blockSize, blockSize, blockSize };
                        SDL_RenderDrawRect(renderer, &rect);
                        SDL_Rect innerRect = { 
                            offsetX + boardX * blockSize + 1, 
                            offsetY + boardY * blockSize + 1, 
                            blockSize - 2, 
                            blockSize - 2 
                        };
                        SDL_RenderDrawRect(renderer, &innerRect);
                    }
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }
}

void Renderer::drawNextPiecesPanel(const std::vector<Piece> &nextPieces, int panelX, int panelY, int nextPieceSize) {
    SDL_Rect nextPanel;
    nextPanel.x = panelX;
    nextPanel.y = panelY - 50;
    nextPanel.w = 5 * nextPieceSize + 20;
    nextPanel.h = 4 * (5 * nextPieceSize + 20) + 30;
    
    // le meme gradient que le score panel
    for (int y = 0; y < nextPanel.h; y++) {
        int r = 40 + (y * 20 / nextPanel.h);
        int g = 0;
        int b = 80 + (y * 40 / nextPanel.h);
        
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 
                          nextPanel.x, nextPanel.y + y, 
                          nextPanel.x + nextPanel.w, nextPanel.y + y);
    }
    
    SDL_SetRenderDrawColor(renderer, 180, 180, 200, 255);
    SDL_RenderDrawRect(renderer, &nextPanel);
    
    SDL_Rect innerBorder = {nextPanel.x + 3, nextPanel.y + 3, nextPanel.w - 6, nextPanel.h - 6};
    SDL_SetRenderDrawColor(renderer, 100, 100, 140, 255);
    SDL_RenderDrawRect(renderer, &innerBorder);
    
    SDL_Rect textRect = { nextPanel.x + 10, nextPanel.y + 10, 0, 0 };
    SDL_Color goldColor = { 255, 255, 0, 255 };
    renderText("NEXT PIECES", textRect, goldColor);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
    SDL_RenderDrawLine(renderer, 
                      nextPanel.x + 10, nextPanel.y + 40, 
                      nextPanel.x + nextPanel.w - 10, nextPanel.y + 40);
    
    for (size_t i = 0; i < nextPieces.size() && i < 4; ++i) {
        int pieceY = nextPanel.y + 50 + i * (5 * nextPieceSize + 10);
        
        SDL_Rect pieceBackground = { 
            nextPanel.x + 10, 
            pieceY, 
            5 * nextPieceSize, 
            5 * nextPieceSize 
        };
        
        SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
        SDL_RenderFillRect(renderer, &pieceBackground);
        
        SDL_SetRenderDrawColor(renderer, 100, 100, 140, 255);
        SDL_RenderDrawRect(renderer, &pieceBackground);
        
        drawPiece(nextPieces[i], nextPanel.x + 10, pieceY, nextPieceSize);
    }
}

void Renderer::drawHeldPiecePanel(const Piece* heldPiece, int panelX, int panelY, int heldPieceSize) {
    SDL_Rect holdPanel;
    holdPanel.x = panelX - 15;
    holdPanel.y = panelY + 200;
    holdPanel.w = 5 * heldPieceSize + 20;
    holdPanel.h = 5 * heldPieceSize + 80;
    
    for (int y = 0; y < holdPanel.h; y++) {
        int r = 40 + (y * 20 / holdPanel.h);
        int g = 0;
        int b = 80 + (y * 40 / holdPanel.h);
        
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 
                          holdPanel.x, holdPanel.y + y, 
                          holdPanel.x + holdPanel.w, holdPanel.y + y);
    }
    
    SDL_SetRenderDrawColor(renderer, 180, 180, 200, 255);
    SDL_RenderDrawRect(renderer, &holdPanel);
    
    SDL_Rect innerBorder = {holdPanel.x + 3, holdPanel.y + 3, holdPanel.w - 6, holdPanel.h - 6};
    SDL_SetRenderDrawColor(renderer, 100, 100, 140, 255);
    SDL_RenderDrawRect(renderer, &innerBorder);
    
    SDL_Rect textRect = { holdPanel.x + 10, holdPanel.y + 10, 0, 0 };
    SDL_Color goldColor = { 255, 255, 0, 255 };
    renderText("HOLD", textRect, goldColor);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
    SDL_RenderDrawLine(renderer, 
                      holdPanel.x + 10, holdPanel.y + 40, 
                      holdPanel.x + holdPanel.w - 10, holdPanel.y + 40);
    
    if (heldPiece != nullptr) {
        int pieceY = holdPanel.y + 50;
        
        SDL_Rect pieceBackground = { 
            holdPanel.x + 10, 
            pieceY, 
            5 * heldPieceSize, 
            5 * heldPieceSize 
        };
        
        SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
        SDL_RenderFillRect(renderer, &pieceBackground);
        
        SDL_SetRenderDrawColor(renderer, 100, 100, 140, 255);
        SDL_RenderDrawRect(renderer, &pieceBackground);
        
        drawPiece(*heldPiece, holdPanel.x + 10, pieceY, heldPieceSize);
    } else {
        SDL_Rect emptyTextRect = { holdPanel.x + 15, holdPanel.y + 65, 0, 0 };
        SDL_Color grayColor = { 150, 150, 150, 255 };
        renderText("EMPTY", emptyTextRect, grayColor);
    }
}

void Renderer::drawScorePanel(int score, int level) {
    float pulseIntensity = calculateScorePulseIntensity(score);
    
    SDL_Rect scorePanel = calculateScorePanelPosition();
    
    drawScorePanelBackground(scorePanel, pulseIntensity);
    drawScorePanelBorders(scorePanel);
    drawScoreSection(scorePanel, score, pulseIntensity);
    drawDivider(scorePanel);
    drawLevelSection(scorePanel, level);
    drawLevelIndicatorDots(scorePanel, level);
}

float Renderer::calculateScorePulseIntensity(int score) {
    static int previousScore = 0;
    static Uint32 scoreChangeTime = 0;
    
    if (score > previousScore) {
        previousScore = score;
        scoreChangeTime = SDL_GetTicks();
    }
    
    Uint32 timeSinceChange = SDL_GetTicks() - scoreChangeTime;
    float pulseIntensity = 0;
    if (timeSinceChange < 1000) {
        pulseIntensity = 1.0f - (timeSinceChange / 1000.0f);
    }
    
    return pulseIntensity;
}

SDL_Rect Renderer::calculateScorePanelPosition() {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    int boardWidthPixels = Board::WIDTH * blockSize;
    int offsetX = (windowWidth - boardWidthPixels) / 2;

    SDL_Rect scorePanel;
    scorePanel.x = offsetX - 200;
    scorePanel.y = 100;
    scorePanel.w = 180;
    scorePanel.h = 200;
    
    return scorePanel;
}

void Renderer::drawScorePanelBackground(const SDL_Rect& scorePanel, float pulseIntensity) {
    for (int y = 0; y < scorePanel.h; y++) {
        int r = 40 + (y * 20 / scorePanel.h);
        int g = 0;
        int b = 80 + (y * 40 / scorePanel.h);
        
        if (pulseIntensity > 0) {
            r = r + static_cast<int>(pulseIntensity * 50);
            g = g + static_cast<int>(pulseIntensity * 20);
            b = b + static_cast<int>(pulseIntensity * 70);
            r = std::min(255, r);
            g = std::min(255, g);
            b = std::min(255, b);
        }
        
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 
                          scorePanel.x, scorePanel.y + y, 
                          scorePanel.x + scorePanel.w, scorePanel.y + y);
    }
}

void Renderer::drawScorePanelBorders(const SDL_Rect& scorePanel) {
    // Outer border
    SDL_SetRenderDrawColor(renderer, 180, 180, 200, 255);
    SDL_RenderDrawRect(renderer, &scorePanel);
    
    // Inner border
    SDL_Rect innerBorder = {
        scorePanel.x + 3, 
        scorePanel.y + 3, 
        scorePanel.w - 6, 
        scorePanel.h - 6
    };
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 140, 255);
    SDL_RenderDrawRect(renderer, &innerBorder);
}

void Renderer::drawScoreSection(const SDL_Rect& scorePanel, int score, float pulseIntensity) {
    // Draw "SCORE" heading
    SDL_Rect textRect = { scorePanel.x + 20, scorePanel.y + 15, 0, 0 };
    SDL_Color goldColor = { 255, 255, 0, 255 };
    renderText("SCORE", textRect, goldColor);
    
    // Draw score value
    std::string scoreStr = std::to_string(score);
    textRect = { scorePanel.x + 50, scorePanel.y + 45, 0, 0 };
    
    SDL_Color scoreColor;
    if (pulseIntensity > 0) {
        scoreColor = { 
            255, 
            static_cast<Uint8>(255 * pulseIntensity), 
            static_cast<Uint8>(255 * pulseIntensity), 
            255 
        };
    } else {
        scoreColor = { 255, 255, 255, 255 };
    }
    
    SDL_Rect scaledRect = textRect;
    scaledRect.w = 0;
    scaledRect.h = 32;
    renderText(scoreStr.c_str(), scaledRect, scoreColor);
}

void Renderer::drawDivider(const SDL_Rect& scorePanel) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
    SDL_RenderDrawLine(renderer, 
                      scorePanel.x + 15, scorePanel.y + 95, 
                      scorePanel.x + scorePanel.w - 15, scorePanel.y + 95);
}

void Renderer::drawLevelSection(const SDL_Rect& scorePanel, int level) {
    // Draw "LEVEL" heading
    SDL_Rect textRect = { scorePanel.x + 20, scorePanel.y + 110, 0, 0 };
    SDL_Color goldColor = { 255, 255, 0, 255 };
    renderText("LEVEL", textRect, goldColor);
    
    // Draw level value
    std::string levelStr = std::to_string(level);
    textRect = { scorePanel.x + 50, scorePanel.y + 140, 0, 0 };
    SDL_Color levelColor = { 255, 255, 255, 255 };
    SDL_Rect levelRect = textRect;
    levelRect.w = 0;
    levelRect.h = 28;
    renderText(levelStr.c_str(), levelRect, levelColor);
}

void Renderer::drawLevelIndicatorDots(const SDL_Rect& scorePanel, int level) {
    for (int i = 0; i < 10; i++) {
        SDL_Rect dot = { 
            scorePanel.x + 20 + (i * 15), 
            scorePanel.y + 175, 
            10, 
            10 
        };
        
        if (i < level % 10) {
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        }
        
        SDL_RenderFillRect(renderer, &dot);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawRect(renderer, &dot);
    }
}
void Renderer::drawGradientBackground(int windowWidth, int windowHeight, bool isPurpleTheme) {
    for (int y = 0; y < windowHeight; y++) {
        float ratio = static_cast<float>(y) / windowHeight;
        int r, g, b;
        if (isPurpleTheme) {                    // gradient en violet
            r = 40 + (y * 100 / windowHeight);
            g = 0 + (y * 30 / windowHeight);
            b = 100 + (y * 100 / windowHeight);
        } else {
            // Blue/teal gradient
            r = static_cast<int>(30 + 40 * ratio);
            g = static_cast<int>(80 + 60 * ratio);
            b = static_cast<int>(120 + 80 * ratio);
        }
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
    }
}

void Renderer::drawMainMenu(int windowWidth,
                            int windowHeight,
                            const SDL_Rect &startButtonRect,
                            const SDL_Rect &quitButtonRect,
                            bool isStartButtonHovered,
                            bool isQuitButtonHovered
) {
    drawGradientBackground(windowWidth, windowHeight, false);
    SDL_Color titleColor = { 255, 255, 0, 255 };
    renderTextCentered("TETRIS", windowWidth / 2, windowHeight / 4, titleColor, 220);

    SDL_SetRenderDrawColor(renderer, 100, 100, 140, 255);
    SDL_RenderDrawRect(renderer, &startButtonRect);
    SDL_RenderDrawRect(renderer, &quitButtonRect);

    SDL_Color startColor = isStartButtonHovered ?
        SDL_Color{255, 255, 0, 255} :
        SDL_Color{255, 255, 255, 255};

    SDL_Color quitColor = isQuitButtonHovered ?
        SDL_Color{255, 255, 0, 255} :
        SDL_Color{255, 255, 255, 255};

    renderTextCentered("Start Game", windowWidth / 2, windowHeight / 2 - 50, startColor, 40);
    renderTextCentered("Quit", windowWidth / 2, windowHeight / 2 + 50, quitColor, 40);
    if (isStartButtonHovered) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &startButtonRect);
    }
    if (isQuitButtonHovered) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &quitButtonRect);
    }
}

void Renderer::drawPauseMenu(int windowWidth, int windowHeight) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = { 0, 0, windowWidth, windowHeight };
    SDL_RenderFillRect(renderer, &overlay);
    
    SDL_Rect pausePanel = { windowWidth / 2 - 150, windowHeight / 2 - 100, 300, 200 };
    SDL_SetRenderDrawColor(renderer, 40, 40, 80, 255);
    SDL_RenderFillRect(renderer, &pausePanel);
    
    SDL_SetRenderDrawColor(renderer, 180, 180, 200, 255);
    SDL_RenderDrawRect(renderer, &pausePanel);
    
    SDL_Color pauseColor = { 255, 255, 0, 255 };
    renderTextCentered("PAUSED", windowWidth / 2, windowHeight / 2 - 60, pauseColor, 40);
    
    SDL_Color instructionColor = { 255, 255, 255, 255 };
    renderTextCentered("Resume", windowWidth / 2, windowHeight / 2 - 10, instructionColor, 30);
    renderTextCentered("(Escape)", windowWidth / 2, windowHeight / 2 + 15, instructionColor, 12);
    renderTextCentered("Main Menu", windowWidth / 2, windowHeight / 2 + 40, instructionColor, 30);
    renderTextCentered("(Backspace)", windowWidth / 2, windowHeight / 2 + 65, instructionColor, 12);
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Renderer::drawGameOverMenu(int windowWidth, int windowHeight) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = { 0, 0, windowWidth, windowHeight };
    SDL_RenderFillRect(renderer, &overlay);
    
    SDL_Rect gameOverPanel = { windowWidth / 2 - 150, windowHeight / 2 - 100, 300, 200 };
    SDL_SetRenderDrawColor(renderer, 80, 40, 40, 255);
    SDL_RenderFillRect(renderer, &gameOverPanel);
    
    SDL_SetRenderDrawColor(renderer, 200, 180, 180, 255);
    SDL_RenderDrawRect(renderer, &gameOverPanel);
    
    renderTextCentered("GAME OVER", windowWidth / 2, windowHeight / 2 - 60, 
                      SDL_Color{255, 100, 100, 255}, 40);
    
    SDL_Color instructionColor = { 255, 255, 255, 255 };
    renderTextCentered("Restart", windowWidth / 2, windowHeight / 2 - 10, instructionColor, 30);
    renderTextCentered("(R)", windowWidth / 2, windowHeight / 2 + 15, instructionColor, 12);
    renderTextCentered("Main Menu", windowWidth / 2, windowHeight / 2 + 40, instructionColor, 30);
    renderTextCentered("(Escape)", windowWidth / 2, windowHeight / 2 + 65, instructionColor, 12);    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Renderer::drawBoard(const Board &board,
                         const Piece &piece,
                         int posX, int posY,
                         const std::vector<Piece> &nextPieces,
                         const Piece* heldPiece
) {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
    drawGradientBackground(windowWidth, windowHeight, true);

    int boardWidthPixels = Board::WIDTH * blockSize;
    int boardHeightPixels = Board::HEIGHT * blockSize;

    int offsetX = (windowWidth - boardWidthPixels) / 2;
    int offsetY = (windowHeight - boardHeightPixels) / 2 - blockSize;

    drawBoardGrid(board, offsetX, offsetY);
    drawGhostPiece(board, piece, posX, posY, offsetX, offsetY);
    drawPiece(piece, offsetX + posX * blockSize, offsetY + posY * blockSize, blockSize);

    int nextPiecesPanelX = offsetX + boardWidthPixels + 50;
    int nextPiecesPanelY = offsetY + 100;
    int nextPieceSize = blockSize - 10;
    drawNextPiecesPanel(nextPieces, nextPiecesPanelX, nextPiecesPanelY, nextPieceSize);

    int heldPiecePanelX = offsetX - 200;
    int heldPiecePanelY = offsetY + 100;
    int heldPieceSize = blockSize - 10;
    drawHeldPiecePanel(heldPiece, heldPiecePanelX, heldPiecePanelY, heldPieceSize);

    // draw a score panel
    drawScorePanel(board.getScore(), board.getLevel());

    SDL_RenderPresent(renderer);
}
