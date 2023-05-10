#include "Board.hpp"
#include <iostream>

namespace chess {

    Board::Board(float size) {
        if (!boardTexture.loadFromFile("../assets/img/chessboard.png")) {
            // Handle the error
        }

        if (!legalMoveTexture.loadFromFile("../assets/img/legal-move.png")) {
            // Handle the error
        }

        boardSprite.setTexture(boardTexture);
        boardSprite.setScale(size / boardTexture.getSize().x, size / boardTexture.getSize().y);
        legalMoveSprite.setTexture(legalMoveTexture);
        legalMoveSprite.setScale(size / static_cast<float>(legalMoveTexture.getSize().x) / 8.0f,
                                 size / static_cast<float>(legalMoveTexture.getSize().y) / 8.0f);

        loadPieceTextures();
        createPieces();
    }




    void Board::loadPieceTextures() {
        const std::vector<std::string> pieceNames = {
                "pawn", "knight", "bishop", "rook", "queen", "king"
        };

        for (const auto &color : {"white", "black"}) {
            for (const auto &name : pieceNames) {
                sf::Texture texture;
                std::string texturePath = "../assets/img/" + std::string(color) + "-" + name + ".png";
                if (texture.loadFromFile(texturePath)) {
                    pieceTextures.push_back(texture);
                }
            }
        }
    }

    void Board::createPieces() {
        pieceLayout = {
                {-4, -2, -3, -5, -6, -3, -2, -4},
                {-1, -1, -1, -1, -1, -1, -1, -1},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 1, 1, 1, 1, 1, 1},
                {4, 2, 3, 5, 6, 3, 2, 4}
        };

        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                int pieceIndex = pieceLayout[row][col];
                if (pieceIndex != 0) {
                    sf::Texture &texture = pieceTextures[(pieceIndex > 0 ? 0 : 6) + std::abs(pieceIndex) - 1];
                    Piece piece(texture, sf::Vector2f(col * squareSize, row * squareSize), pieceLayout[row][col]);
                    float scaleFactor = squareSize / texture.getSize().x;
                    piece.setPosition(sf::Vector2f(col * squareSize, row * squareSize), scaleFactor);
                    pieces.push_back(piece);
                }
            }
        }
    }


    void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(boardSprite, states);
        for (const auto &piece : pieces) {
            target.draw(piece, states);
        }

        // Draw the legal move sprite
        for (const auto& legalMove : legalMoveSprites) {
            target.draw(legalMove, states);
        }
    }

    void Board::handleMouseButtonEvent(const sf::Event::MouseButtonEvent &event, bool isPressed) {
        float x = event.x;
        float y = event.y;

        if (isPressed) {
            if (event.button == sf::Mouse::Left) {
                findClickedPiece(x, y);

                if (draggedPieceIndex >= 0) {
                    originalPos = pieces[draggedPieceIndex].getPosition();
                    findLegalMoves(draggedPieceIndex);
                }
            }
        } else {
            releasePiece(event.x, event.y);
        }
    }

    void Board::findClickedPiece(float x, float y) {
        for (size_t i = 0; i < pieces.size(); ++i) {
            auto &piece = pieces[i];
            if (piece.getGlobalBounds().contains(x, y)) {
                draggedPieceIndex = i;
                mouseOffset = piece.getPosition() - sf::Vector2f(x, y);
                originalScale = piece.getScale(); // Store the original scale
                break;
            }
        }
    }

    void Board::findLegalMoves(int pieceIndex) {
        auto& piece = pieces[pieceIndex];
        legalMoveSprites.clear(); // clear any previously stored sprites

        possibleMoves = findPossibleMovesForPieceType(piece);
        for (const auto& move : possibleMoves) {
            legalMoveSprite.setTexture(legalMoveTexture);
            legalMoveSprite.setScale(squareSize / legalMoveTexture.getSize().x, squareSize / legalMoveTexture.getSize().y);
            legalMoveSprite.setPosition(move.x * squareSize, move.y * squareSize);
            legalMoveSprites.push_back(legalMoveSprite);
        }
    }


    void Board::releasePiece(float x, float y) {
        if (draggedPieceIndex != -1) {
            int col = static_cast<int>((x + mouseOffset.x) / squareSize);
            int row = static_cast<int>((y + mouseOffset.y) / squareSize);

            // Check if the move is legal
            sf::Vector2i newTile(col, row);
            bool isLegalMove = false;
            for (const auto &move: possibleMoves) {
                if (move == newTile) {
                    isLegalMove = true;
                    break;
                }
            }

            if (isLegalMove) {
                // Check if the target square is empty or contains an opponent's piece
                int targetPieceIndex = getPieceIndexAtTile(col, row);
                if (targetPieceIndex == 0 || pieces[draggedPieceIndex].getPieceIndex() * targetPieceIndex < 0) {
                    // Update pieceLayout with the new position
                    sf::Vector2f oldPos = pieces[draggedPieceIndex].getPosition();
                    int oldCol = static_cast<int>(oldPos.x / squareSize);
                    int oldRow = static_cast<int>(oldPos.y / squareSize);
                    pieceLayout[oldRow][oldCol] = 0;
                    pieceLayout[row][col] = pieces[draggedPieceIndex].getPieceIndex();

                    snapPieceToTileCenter(col, row);
                }
            }
            else {
                // Move the piece back to its original position if the move is not legal
                int oldCol = static_cast<int>(originalPos.x / squareSize);
                int oldRow = static_cast<int>(originalPos.y / squareSize);
                snapPieceToTileCenter(oldCol, oldRow);

            }

            pieces[draggedPieceIndex].setScale(originalScale);
            draggedPieceIndex = -1;

            // Clear legalMoveSprites vector when the piece is released
            legalMoveSprites.clear();
        }
    }



    void Board::snapPieceToTileCenter(int col, int row) {
        sf::Vector2f newCenter(
                col * squareSize + squareSize / 2,
                row * squareSize + squareSize / 2
        );

        sf::FloatRect bounds = pieces[draggedPieceIndex].getGlobalBounds();
        sf::Vector2f topLeftOffset(
                bounds.width / 2,
                bounds.height / 2
        );

        pieces[draggedPieceIndex].setPosition(sf::Vector2f(newCenter.x - (topLeftOffset.x * originalScale.x), newCenter.y - (topLeftOffset.y * originalScale.y)));
    }

    void Board::handleMouseMoveEvent(const sf::Event::MouseMoveEvent &event) {
        if (draggedPieceIndex != -1 && draggedPieceIndex < pieces.size()) {
            pieces[draggedPieceIndex].setPosition(sf::Vector2f(event.x, event.y) + mouseOffset);
        }
    }

    std::vector<sf::Vector2i> Board::findPossibleMovesForPieceType(const Piece &piece) {
        possibleMoves.clear();

        // Calculate the current tile position of the piece
        sf::Vector2f piecePosition = piece.getPosition();
        int currentCol = static_cast<int>(piecePosition.x / squareSize);
        int currentRow = static_cast<int>(piecePosition.y / squareSize);

        // Implement logic to find possible moves based on the piece type
        if (piece.isPawn()) {
            // Assume that pawns with a positive pieceIndex are white and
            // pawns with a negative pieceIndex are black
            int direction = (piece.getPieceIndex() > 0) ? -1 : 1;

            // One square forward
            int targetRow = currentRow + direction;
            if (targetRow >= 0 && targetRow < 8) {
                if (getPieceIndexAtTile(currentCol, targetRow) == 0) { // Check if the target square is empty
                    possibleMoves.emplace_back(currentCol, targetRow);
                }
            }

            // Two squares forward if it is the pawn's first move
            if ((piece.getPieceIndex() > 0 && currentRow == 6) || (piece.getPieceIndex() < 0 && currentRow == 1)) {
                int twoSqTargetRow = currentRow + 2 * direction;
                if (twoSqTargetRow >= 0 && twoSqTargetRow < 8) {
                    if (getPieceIndexAtTile(currentCol, targetRow) == 0 && getPieceIndexAtTile(currentCol, twoSqTargetRow) == 0) { // Check if both target squares are empty
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
                        int targetPieceIndex = getPieceIndexAtTile(diagTargetCol, diagTargetRow);
                        if (targetPieceIndex != 0 && piece.getPieceIndex() * targetPieceIndex < 0) {
                            // An opponent piece is on the corresponding diagonal square
                            possibleMoves.emplace_back(diagTargetCol, diagTargetRow);
                        }
                    }
                }
            }
        } else if (piece.isKnight()) {
            // Possible knight moves: (row, col)
            std::vector<sf::Vector2i> knightMoves = {
                    {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
                    {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
            };

            for (const auto &move : knightMoves) {
                int targetRow = currentRow + move.x;
                int targetCol = currentCol + move.y;

                if (targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8) {
                    int targetPieceIndex = getPieceIndexAtTile(targetCol, targetRow);

                    // Check if the target square is empty or has an opponent's piece
                    if (targetPieceIndex == 0 || piece.getPieceIndex() * targetPieceIndex < 0) {
                        possibleMoves.emplace_back(targetCol, targetRow);
                    }
                }
            }
        } else if (piece.isBishop()) {
            // TODO: Add possible moves for a bishop
        } else if (piece.isRook()) {
            // TODO: Add possible moves for a rook
        } else if (piece.isQueen()) {
            // TODO: Add possible moves for a queen
        } else if (piece.isKing()) {
            // TODO: Add possible moves for a king
        }
        for (const auto& move : possibleMoves) {
            std::cout << "(" << move.x << ", " << move.y << ") ";

        }

        std::cout << std::endl;
        std::cout << currentCol << std::endl;
        std::cout << currentRow << std::endl;



        return possibleMoves;
    }



    int Board::getPieceIndexAtTile(int col, int row) {
        for (size_t i = 0; i < pieces.size(); ++i) {
            if (i == draggedPieceIndex) {
                continue; // Ignore the dragged piece
            }

            const auto &piece = pieces[i];
            sf::Vector2f position = piece.getPosition();
            int pieceCol = static_cast<int>(position.x / squareSize);
            int pieceRow = static_cast<int>(position.y / squareSize);

            if (pieceCol == col && pieceRow == row) {
                return piece.getPieceIndex();
            }
        }

        return 0;
    }


    int Piece::getPieceIndex() const {
        return pieceIndex;
    }



} // namespace chess
