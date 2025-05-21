#include "Renderer.hpp"

Renderer::Renderer(SDL_Renderer *r) : renderer(r) {}

void Renderer::draw(const Board &board, const Piece &piece, int posX, int posY) {
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
            if (grid[x][y]) {
                if (y >= 0 && y < Board::HEIGHT) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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

    // dessiner les shapes
    const auto &shape = piece.getShape();
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            if (shape[x][y]) {
                int boardX = posX + x;
                int boardY = posY + y;
                
                if (boardX >= 0 && boardX < Board::WIDTH && boardY >= 0 && boardY < Board::HEIGHT) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_Rect rect = { offsetX + boardX * blockSize, offsetY + boardY * blockSize, blockSize, blockSize };
                    SDL_RenderFillRect(renderer, &rect);
                    
                    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                    SDL_RenderDrawRect(renderer, &rect);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}