#pragma once

#include <SFML/Graphics.hpp>
#include "Piece.hpp"
#include "Board.hpp"

class BoardEvent {
public:
    static void handleMouseButtonEvent(Board& board, const sf::Event::MouseButtonEvent &event, bool isPressed);
    static void handleMouseMoveEvent(Board& board, const sf::Event::MouseMoveEvent &event);
    static int getPieceIndexAtTile(Board board, int col, int row);
    static void updatePieceLayout(Board& board, int col, int row, const Piece& piece);
    static void removeTargetPiece(Board& board, int col, int row);
    static void snapPieceToTileCenter(Board& board, int col, int row);
    static void revertMove(Board& board);

private:
    static int draggedPieceArrayIndex;
    static sf::Vector2f originalPos;
    static sf::Vector2f mouseOffset;
    static sf::Vector2f originalScale;
    static std::vector<sf::Vector2i> possibleMoves;

    static void findClickedPiece(Board& board, float x, float y);
    static void findLegalMoves(Board& board, int pieceIndex);
    static void releasePiece(Board& board, float x, float y);
    static bool isPossibleMove(int col, int row);
    static void handleLegalMove(Board& board, int col, int row);
    static void resetDragState(Board& board);
    static int getTargetPieceArrayIndexAtTile(Board& board, int col, int row);
};

