#include "Board.hpp"
#include <iostream>

namespace chess {

    Board::Board(float size) {

        try {
            boardTexture.loadFromFile("../assets/img/chessboard.png");
        } catch (const std::exception& e) {
            std::cerr << "Error loading application: " << e.what() << std::endl;
            std::exit(1);
        }

        boardSprite.setTexture(boardTexture);
        boardSprite.setScale(size / boardTexture.getSize().x, size / boardTexture.getSize().y);

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
                    Piece piece(texture, sf::Vector2f(col * tileSize, row * tileSize), pieceLayout[row][col]);
                    float scaleFactor = tileSize / texture.getSize().x;
                    piece.setPosition(sf::Vector2f(col * tileSize, row * tileSize), scaleFactor);
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
    }

    int Piece::getPieceIndex() const {
        return pieceIndex;
    }

} // namespace chess
