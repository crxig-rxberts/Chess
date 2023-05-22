#include "PieceMovement.hpp"
#include "iostream"
#include "Board.hpp"

namespace chess {

    std::vector<sf::Vector2i>
    PieceMovement::findPossibleMovesForPieceType(const Piece &piece, std::vector<std::vector<int>> pieceLayout, Board &board) {
        std::vector<sf::Vector2i> possibleMoves;

        // Calculate the current tile position of the piece
        sf::Vector2f piecePosition = piece.getPosition();
        int currentCol = static_cast<int>(piecePosition.x / squareSize);
        int currentRow = static_cast<int>(piecePosition.y / squareSize);

        // Implement logic to find possible moves based on the piece type
        if (piece.isPawn()) {
            possibleMoves = findLegalMovesForPawn(piece, currentCol, currentRow, board);
        } else if (piece.isKnight()) {
            possibleMoves = findLegalMovesForKnight(piece, currentCol, currentRow, board);
        } else if (piece.isBishop()) {
            possibleMoves = findLegalMovesForBishop(piece, currentCol, currentRow, board);
        } else if (piece.isRook()) {
            possibleMoves = findLegalMovesForRook(piece, currentCol, currentRow, board);
        } else if (piece.isQueen()) {
            possibleMoves = findLegalMovesForQueen(piece, currentCol, currentRow, board);
        } else if (piece.isKing()) {
            possibleMoves = findLegalMovesForKing(piece, currentCol, currentRow, board);
        }

        for (const auto &move: possibleMoves) {
            std::cout << "(" << move.x << ", " << move.y << ") ";
        }
        std::cout << std::endl;
        std::cout << currentCol << std::endl;
        std::cout << currentRow << std::endl;
        return possibleMoves;
    }

    std::vector<sf::Vector2i> PieceMovement::findLegalMovesForPawn(const Piece &piece, int currentCol, int currentRow, Board &board) {
        std::vector<sf::Vector2i> possibleMoves;

        // Assume that pawns with a positive pieceIndex are white and
        // pawns with a negative pieceIndex are black
        int direction = (piece.getPieceIndex() > 0) ? -1 : 1;

        // One square forward
        int targetRow = currentRow + direction;
        if (targetRow >= 0 && targetRow < 8) {
            if (board.getPieceIndexAtTile(currentCol, targetRow) == 0) { // Check if the target square is empty
                possibleMoves.emplace_back(currentCol, targetRow);
            }
        }

        // Two squares forward if it is the pawn's first move
        if ((piece.getPieceIndex() > 0 && currentRow == 6) || (piece.getPieceIndex() < 0 && currentRow == 1)) {
            int twoSqTargetRow = currentRow + 2 * direction;
            if (twoSqTargetRow >= 0 && twoSqTargetRow < 8) {
                if (board.getPieceIndexAtTile(currentCol, targetRow) == 0 &&
                    board.getPieceIndexAtTile(currentCol, twoSqTargetRow) == 0) { // Check if both target squares are empty
                    possibleMoves.emplace_back(currentCol, twoSqTargetRow);
                }
            }
        }
        // Diagonal moves if there are opponent pieces on the corresponding squares
        for (int colOffset = -1; colOffset <= 1; colOffset += 2) {
            int diagTargetCol = currentCol + colOffset;
            if (diagTargetCol >= 0 && diagTargetCol < 8) {
                int diagTargetRow = currentRow + direction;
                if (diagTargetRow >= 0 && diagTargetRow < 8) {
                    int targetPieceIndex = board.getPieceIndexAtTile(diagTargetCol, diagTargetRow);
                    if (targetPieceIndex != 0 && piece.getPieceIndex() * targetPieceIndex < 0) {
                        // An opponent piece is on the corresponding diagonal square
                        possibleMoves.emplace_back(diagTargetCol, diagTargetRow);
                    }
                }
            }
        }
        return possibleMoves;
    }

    std::vector<sf::Vector2i>
    PieceMovement::findLegalMovesForKnight(const Piece &piece, int currentCol, int currentRow, Board &board) {
        std::vector<sf::Vector2i> possibleMoves;
        // Possible knight moves: (row, col)
        std::vector<sf::Vector2i> knightMoves = {
                {2,  1},
                {1,  2},
                {-1, 2},
                {-2, 1},
                {-2, -1},
                {-1, -2},
                {1,  -2},
                {2,  -1}
        };

        for (const auto &move: knightMoves) {
            int targetRow = currentRow + move.x;
            int targetCol = currentCol + move.y;
            // if the target square is in bounds
            if (targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8) {
                int targetPieceIndex = board.getPieceIndexAtTile(targetCol, targetRow);
                // if the target square is empty or has an opponents piece
                if (targetPieceIndex == 0 || piece.getPieceIndex() * targetPieceIndex < 0) {
                    possibleMoves.emplace_back(targetCol, targetRow);
                }
            }
        }
        return possibleMoves;
    }

    std::vector<sf::Vector2i>
    PieceMovement::findLegalMovesForBishop(const Piece &piece, int currentCol, int currentRow, Board &board) {
        std::vector<sf::Vector2i> possibleMoves;

        // Possible bishop move directions: (row, col)
        std::vector<sf::Vector2i> bishopDirections = {
                {-1, -1},
                {-1, 1},
                {1,  1},
                {1,  -1}
        };

        for (const auto &direction: bishopDirections) {
            int newRow = currentRow + direction.x;
            int newCol = currentCol + direction.y;

            // Keep moving in the direction until an invalid square is reached
            while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                int targetPieceIndex = board.getPieceIndexAtTile(newCol, newRow);

                // Check if the target square is empty or has an opponent's piece
                if (targetPieceIndex == 0 || piece.getPieceIndex() * targetPieceIndex < 0) {
                    possibleMoves.emplace_back(newCol, newRow);

                    // If the target square has an opponent's piece, stop checking further in this direction
                    if (piece.getPieceIndex() * targetPieceIndex < 0) {
                        break;
                    }
                } else {
                    // If the target square has a piece of the same color, stop checking further in this direction
                    break;
                }

                newRow += direction.x;
                newCol += direction.y;
            }
        }

        return possibleMoves;
    }


    std::vector<sf::Vector2i> PieceMovement::findLegalMovesForRook(const Piece &piece, int currentCol, int currentRow, Board &board) {
        std::vector<sf::Vector2i> possibleMoves;

        // Possible rook move directions: (row, col)
        std::vector<sf::Vector2i> rookDirections = {
                {-1, 0},
                {1,  0},
                {0,  1},
                {0,  -1}
        };

        for (const auto &direction: rookDirections) {
            int newRow = currentRow + direction.x;
            int newCol = currentCol + direction.y;

            // Keep moving in the direction until an invalid square is reached
            while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                int targetPieceIndex = board.getPieceIndexAtTile(newCol, newRow);

                // Check if the target square is empty or has an opponent's piece
                if (targetPieceIndex == 0 || piece.getPieceIndex() * targetPieceIndex < 0) {
                    possibleMoves.emplace_back(newCol, newRow);

                    // If the target square has an opponent's piece, stop checking further in this direction
                    if (piece.getPieceIndex() * targetPieceIndex < 0) {
                        break;
                    }
                } else {
                    // If the target square has a piece of the same color, stop checking further in this direction
                    break;
                }

                newRow += direction.x;
                newCol += direction.y;
            }
        }

        return possibleMoves;
    }

    std::vector<sf::Vector2i>
    PieceMovement::findLegalMovesForQueen(const Piece &piece, int currentCol, int currentRow, Board &board) {
        std::vector<sf::Vector2i> possibleMoves;

        // Possible queen move directions: (row, col)
        std::vector<sf::Vector2i> queenDirections = {
                {-1, 0},
                {1,  0},
                {0,  1},
                {0,  -1}, // Rook directions
                {-1, -1},
                {-1, 1},
                {1,  -1},
                {1,  1}  // Bishop directions
        };

        for (const auto &direction: queenDirections) {
            int newRow = currentRow + direction.x;
            int newCol = currentCol + direction.y;

            // Keep moving in the direction until an invalid square is reached
            while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                int targetPieceIndex = board.getPieceIndexAtTile(newCol, newRow);

                // Check if the target square is empty or has an opponent's piece
                if (targetPieceIndex == 0 || piece.getPieceIndex() * targetPieceIndex < 0) {
                    possibleMoves.emplace_back(newCol, newRow);

                    // If the target square has an opponent's piece, stop checking further in this direction
                    if (piece.getPieceIndex() * targetPieceIndex < 0) {
                        break;
                    }
                } else {
                    // If the target square has a piece of the same color, stop checking further in this direction
                    break;
                }

                newRow += direction.x;
                newCol += direction.y;
            }
        }

        return possibleMoves;
    }

    std::vector<sf::Vector2i> PieceMovement::findLegalMovesForKing(const Piece &piece, int currentCol, int currentRow, Board &board) {
        std::vector<sf::Vector2i> possibleMoves;

        // Possible king move directions: (row, col)
        std::vector<sf::Vector2i> kingDirections = {
                {-1, -1},
                {-1, 0},
                {-1, 1},
                {0,  -1},
                {0,  1},
                {1,  -1},
                {1,  0},
                {1,  1}
        };

        for (const auto &direction: kingDirections) {
            int newRow = currentRow + direction.x;
            int newCol = currentCol + direction.y;

            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                int targetPieceIndex = board.getPieceIndexAtTile(newCol, newRow);

                // Check if the target square is empty or has an opponent's piece
                if (targetPieceIndex == 0 || piece.getPieceIndex() * targetPieceIndex < 0) {
                    possibleMoves.emplace_back(newCol, newRow);
                }
            }
        }

        return possibleMoves;
    }
}