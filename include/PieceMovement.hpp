#pragma once

#include "Piece.hpp"
#include "Board.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class PieceMovement {
public:
    constexpr static const float squareSize = 1600.0f / 8;
    constexpr static const int boardSize = 8;

    static std::vector<sf::Vector2i> calculateMovesForPiece(const Piece &piece, Board &board);

private:
    static std::vector<sf::Vector2i> calculatePawnMoves(const Piece &piece, int currentCol, int currentRow, const Board &board);
    static std::vector<sf::Vector2i> calculateKnightMoves(const Piece &piece, int currentCol, int currentRow, const Board &board);
    static std::vector<sf::Vector2i> calculateDirectionalPieceMoves(const Piece& piece, int currentCol, int currentRow, const Board& board, const std::vector<sf::Vector2i>& directions);
    static std::vector<sf::Vector2i> calculateKingMoves(const Piece &piece, int currentCol, int currentRow, const Board &board);

    static bool isWithinBoard(int col, int row);
    static bool isOpponentsPiece(const Piece& piece, int targetPieceIndex);
    static bool isEmptyOrOpponentsPiece(const Piece& piece, int targetPieceIndex);
};
