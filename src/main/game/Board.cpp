#include "Board.hpp"
#include <iostream>

Board::Board(float size) {
    loadTexture(boardTexture, IMAGE_DIRECTORY + "chessboard.png");
    boardSprite.setTexture(boardTexture);
    boardSprite.setScale(size / boardTexture.getSize().x, size / boardTexture.getSize().y);

    loadPieceTextures();
    createPieces();
}

void Board::loadTexture(sf::Texture& texture, const std::string& filename) {
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Error loading texture from file: " << filename << std::endl;
        std::exit(1);
    }
}

void Board::loadPieceTextures() {
    for (const auto &color : COLORS) {
        for (const auto &name : PIECE_NAMES) {
            sf::Texture texture;
            std::string texturePath = IMAGE_DIRECTORY;
            texturePath.append(color).append("-").append(name).append(".png");
            loadTexture(texture, texturePath);
            pieceTextures.push_back(texture);
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