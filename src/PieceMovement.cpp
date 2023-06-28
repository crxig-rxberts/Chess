#include "PieceMovement.hpp"
#include "Board.hpp"
#include "BoardEvent.hpp"

namespace {
    const std::vector<sf::Vector2i> kingDirections = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    const std::vector<sf::Vector2i> queenDirections = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    const std::vector<sf::Vector2i> rookDirections = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
    const std::vector<sf::Vector2i> bishopDirections = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
    const std::vector<sf::Vector2i> knightMoves = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
}

std::vector<sf::Vector2i> PieceMovement::calculateMovesForPiece(const Piece &piece, Board &board) {
    std::vector<sf::Vector2i> possibleMoves;
    sf::Vector2f piecePosition = piece.getPosition();
    int currentCol = static_cast<int>(piecePosition.x / squareSize);
    int currentRow = static_cast<int>(piecePosition.y / squareSize);

    if (piece.isPawn()) {
        possibleMoves = calculatePawnMoves(piece, currentCol, currentRow, board);
    } else if (piece.isKnight()) {
        possibleMoves = calculateKnightMoves(piece, currentCol, currentRow, board);
    } else if (piece.isBishop()) {
        possibleMoves = calculateDirectionalPieceMoves(piece, currentCol, currentRow, board, bishopDirections);
    } else if (piece.isRook()) {
        possibleMoves = calculateDirectionalPieceMoves(piece, currentCol, currentRow, board, rookDirections);
    } else if (piece.isQueen()) {
        possibleMoves = calculateDirectionalPieceMoves(piece, currentCol, currentRow, board, queenDirections);
    } else if (piece.isKing()) {
        possibleMoves = calculateKingMoves(piece, currentCol, currentRow, board);
    }

    return possibleMoves;
}

std::vector<sf::Vector2i> PieceMovement::calculatePawnMoves(const Piece &piece, int currentCol, int currentRow, const Board &board) {
    std::vector<sf::Vector2i> possibleMoves;
    // Assume that pawns with a positive pieceIndex are white and
    // pawns with a negative pieceIndex are black
    int direction = (piece.getPieceIndex() > 0) ? -1 : 1;

    // One square forward
    int targetRow = currentRow + direction;
    if (targetRow >= 0 && targetRow < 8) {
        if (BoardEvent::getPieceIndexAtTile(board, currentCol, targetRow) ==
            0) { // Check if the target square is empty
            possibleMoves.emplace_back(currentCol, targetRow);
        }
    }
    // Two squares forward if it is the pawn's first move
    if ((piece.getPieceIndex() > 0 && currentRow == 6) || (piece.getPieceIndex() < 0 && currentRow == 1)) {
        int twoSqTargetRow = currentRow + 2 * direction;
        if (twoSqTargetRow >= 0 && twoSqTargetRow < 8) {
            if (BoardEvent::getPieceIndexAtTile(board, currentCol, targetRow) == 0 &&
                BoardEvent::getPieceIndexAtTile(board, currentCol, twoSqTargetRow) == 0) { // Check if both target squares are empty
                possibleMoves.emplace_back(currentCol, twoSqTargetRow);
            }
        }
    }
    // Diagonal moves if there are opponent pieces on the corresponding squares
    for (int colOffset = -1; colOffset <= 1; colOffset += 2) {
        int diagTargetCol = currentCol + colOffset;
        if (diagTargetCol >= 0 && diagTargetCol < 8) {
            int diagTargetRow = currentRow + direction;
            // if tile contains any piece
            if (diagTargetRow >= 0 && diagTargetRow < 8) {
                int targetPieceIndex = BoardEvent::getPieceIndexAtTile(board, diagTargetCol, diagTargetRow);
                if (isOpponentsPiece(piece, targetPieceIndex)) {
                    // remove the possible move
                    possibleMoves.emplace_back(diagTargetCol, diagTargetRow);
                }
            }
        }
    }
    return possibleMoves;
}

std::vector<sf::Vector2i> PieceMovement::calculateKnightMoves(const Piece &piece, int currentCol, int currentRow, const Board &board) {
    std::vector<sf::Vector2i> possibleMoves;

    for (const auto &move: knightMoves) {
        int targetRow = currentRow + move.x;
        int targetCol = currentCol + move.y;
        // if the target square is in bounds
        if (isWithinBoard(targetCol, targetRow)) {
            int targetPieceIndex = BoardEvent::getPieceIndexAtTile(board, targetCol, targetRow);
            // if the target square is empty or has an opponents piece
            if (isEmptyOrOpponentsPiece(piece, targetPieceIndex)) {
                possibleMoves.emplace_back(targetCol, targetRow);
            }
        }
    }

    return possibleMoves;
}

std::vector<sf::Vector2i> PieceMovement::calculateDirectionalPieceMoves(const Piece& piece, int currentCol, int currentRow, const Board& board, const std::vector<sf::Vector2i>& directions) {
    std::vector<sf::Vector2i> possibleMoves;

    for (const auto& direction : directions) {
        int newRow = currentRow + direction.x;
        int newCol = currentCol + direction.y;

        while (isWithinBoard(newCol, newRow)) {
            int targetPieceIndex = BoardEvent::getPieceIndexAtTile(board, newCol, newRow);

            if (isEmptyOrOpponentsPiece(piece, targetPieceIndex)) {
                possibleMoves.emplace_back(newCol, newRow);

                if (piece.getPieceIndex() * targetPieceIndex < 0) {
                    break;
                }
            } else {
                break;
            }

            newRow += direction.x;
            newCol += direction.y;
        }
    }

    return possibleMoves;
}

std::vector<sf::Vector2i> PieceMovement::calculateKingMoves(const Piece &piece, int currentCol, int currentRow, const Board &board) {
    std::vector<sf::Vector2i> possibleMoves;

    for (const auto &direction: kingDirections) {
        int newRow = currentRow + direction.x;
        int newCol = currentCol + direction.y;

        if (isWithinBoard(newCol, newRow)) {
            int targetPieceIndex = BoardEvent::getPieceIndexAtTile(board, newCol, newRow);

            // Check if the target square is empty or has an opponent's piece
            if (isEmptyOrOpponentsPiece(piece, targetPieceIndex)) {
                possibleMoves.emplace_back(newCol, newRow);
            }
        }
    }
    return possibleMoves;
}

bool PieceMovement::isWithinBoard(int col, int row) {
    return (col >= 0 && col < boardSize && row >= 0 && row < boardSize);
}

bool PieceMovement::isOpponentsPiece(const Piece& piece, int targetPieceIndex) {
    return (targetPieceIndex != 0 && piece.getPieceIndex() * targetPieceIndex < 0);
}

bool PieceMovement::isEmptyOrOpponentsPiece(const Piece& piece, int targetPieceIndex) {
    return (targetPieceIndex == 0 || isOpponentsPiece(piece, targetPieceIndex));
}