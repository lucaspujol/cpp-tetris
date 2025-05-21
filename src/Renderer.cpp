#include "Renderer.hpp"

Renderer::Renderer(SDL_Renderer *r) : renderer(r) {
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        // In a real application, you might want to handle this error more gracefully
    }
    
    // Load font
    font = TTF_OpenFont(FONT_PATH, 16); // 16 is the font size
    if (!font) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        // You could fall back to a default system font or handle this error differently
    }
}

Renderer::~Renderer() {
    // Free font resources
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
    
    // Quit SDL_ttf
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

void Renderer::renderText(const char* text, SDL_Rect destRect, SDL_Color color) {
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
}

void Renderer::draw_board(const Board &board, const Piece &piece, int posX, int posY, const std::vector<Piece> &nextPieces) {
    SDL_SetRenderDrawColor(renderer, 75, 75, 75, 255);
    SDL_RenderClear(renderer);

    int boardWidthPixels = Board::WIDTH * blockSize;
    int boardHeightPixels = Board::HEIGHT * blockSize;

    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    int offsetX = (windowWidth - boardWidthPixels) / 2;
    int offsetY = (windowHeight - boardHeightPixels) / 2 - blockSize;

    // dessiner la board
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
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_Rect rect = { offsetX + x * blockSize, offsetY + y * blockSize, blockSize, blockSize };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // dessiner la projection
    int dropY = board.findDropPosition(piece, posX, posY);

    if (dropY > posY) {
        const auto &shape = piece.getShape();
        for (int x = 0; x < 5; ++x) {
            for (int y = 0; y < 5; ++y) {
                if (shape[x][y]) {
                    int boardX = posX + x;
                    int boardY = dropY + y;

                    if (boardX >= 0 && boardX < Board::WIDTH && boardY >= 0 && boardY < Board::HEIGHT) {
                        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                        SDL_Rect rect = { offsetX + boardX * blockSize, offsetY + boardY * blockSize, blockSize, blockSize };
                        SDL_RenderDrawRect(renderer, &rect);
                    }
                }
            }
        }
    }

    // dessiner la piece actuelle
    drawPiece(piece, offsetX + posX * blockSize, offsetY + posY * blockSize, blockSize);

    // dessiner les pieces qui sont queued
    int nextPiecesPanelX = offsetX + boardWidthPixels + 50;
    int nextPiecesPanelY = offsetY + 100;
    int nextPieceSize = blockSize - 10;

    // "Pieces suivantes:"
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect titlePanel = { nextPiecesPanelX, nextPiecesPanelY - 50, 150, 30 };
    SDL_RenderDrawRect(renderer, &titlePanel);
    SDL_Rect textRect = { nextPiecesPanelX + 10, nextPiecesPanelY - 45, 0, 0 };
    renderText("Pieces suivantes:", textRect);

    // draw les 4 next
    for (size_t i = 0; i < nextPieces.size() && i < 4; ++i) {
        int pieceY = nextPiecesPanelY + i * (5 * nextPieceSize + 20);

        // box autour des pièces
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_Rect piecePanel = { nextPiecesPanelX, pieceY, 5 * nextPieceSize, 5 * nextPieceSize };
        SDL_RenderDrawRect(renderer, &piecePanel);

        drawPiece(nextPieces[i], nextPiecesPanelX, pieceY, nextPieceSize);
    }
    SDL_RenderPresent(renderer);
}
