#include "Board.hpp"
#include "PieceMovement.hpp"
#include <iostream>

namespace chess {

    Board::Board(float size) : pieceHidden(true) {
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
            if (piece.isVisible) {
                target.draw(piece, states);
            }
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

        possibleMoves = ::chess::PieceMovement::findPossibleMovesForPieceType(piece, pieceLayout, *this);
        for (const auto& move : possibleMoves) {
            legalMoveSprite.setTexture(legalMoveTexture);
            legalMoveSprite.setScale(squareSize / legalMoveTexture.getSize().x, squareSize / legalMoveTexture.getSize().y);
            legalMoveSprite.setPosition(move.x * squareSize, move.y * squareSize);
            legalMoveSprites.push_back(legalMoveSprite);
        }
    }


    void Board::releasePiece(float x, float y) {
        if (draggedPieceIndex != -1) {
            Piece tempPiece = pieces[draggedPieceIndex];  // Store the dragged piece in a temporary object

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
                if (targetPieceIndex == 0 || tempPiece.getPieceIndex() * targetPieceIndex < 0) {
                    // Update pieceLayout with the new position
                    sf::Vector2f oldPos = tempPiece.getPosition();
                    int oldCol = static_cast<int>(oldPos.x / squareSize);
                    int oldRow = static_cast<int>(oldPos.y / squareSize);
                    pieceLayout[oldRow][oldCol] = 0;
                    pieceLayout[row][col] = tempPiece.getPieceIndex();
                    int targetPieceArrayIndex = getPieceArrayIndexAtTile(col, row);
                    if (targetPieceArrayIndex != -1) {
                        pieces[targetPieceArrayIndex].hide();
                        pieces.erase(pieces.begin() + targetPieceArrayIndex);
                        if(targetPieceArrayIndex < draggedPieceIndex){
                            draggedPieceIndex--;
                        }
                        pieceHidden = true;
                    }
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

    int Board::getPieceArrayIndexAtTile(int col, int row) {
        for (size_t i = 0; i < pieces.size(); ++i) {
            if (i == draggedPieceIndex) {
                continue; // Ignore the dragged piece
            }

            const auto &piece = pieces[i];
            sf::Vector2f position = piece.getPosition();
            int pieceCol = static_cast<int>(position.x / squareSize);
            int pieceRow = static_cast<int>(position.y / squareSize);

            if (pieceCol == col && pieceRow == row) {
                return i;
            }
        }

        return -1;
    }

    int Piece::getPieceIndex() const {
        return pieceIndex;
    }

} // namespace chess
