#pragma once
#include "Piece.hpp"
#include "Board.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

namespace chess {
    class PieceMovement {
    public:
        static std::vector<sf::Vector2i> findPossibleMovesForPieceType(const Piece &piece, std::vector<std::vector<int>> pieceLayout, Board &board);
        constexpr static const float squareSize = 1600.0f / 8;

    private:
        static std::vector<sf::Vector2i> findLegalMovesForPawn(const Piece &piece, int currentCol, int currentRow, Board &board);
        static std::vector<sf::Vector2i> findLegalMovesForKnight(const Piece &piece, int currentCol, int currentRow, Board &board);
        static std::vector<sf::Vector2i> findLegalMovesForBishop(const Piece &piece, int currentCol, int currentRow, Board &board);
        static std::vector<sf::Vector2i> findLegalMovesForRook(const Piece &piece, int currentCol, int currentRow, Board &board);
        static std::vector<sf::Vector2i> findLegalMovesForQueen(const Piece &piece, int currentCol, int currentRow, Board &board);
        static std::vector<sf::Vector2i> findLegalMovesForKing(const Piece &piece, int currentCol, int currentRow, Board &board);
    };
}
