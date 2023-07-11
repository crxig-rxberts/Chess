#pragma once

#include <SFML/Graphics.hpp>
#include "Piece.hpp"

class Board : public sf::Drawable {

public:
    explicit Board(float size);

    const float tileSize = 1600.0f / 8;
    std::vector<std::vector<int>> pieceLayout;
    std::vector<Piece> pieces;
    int currentPlayerTurn = 1;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    const std::string IMAGE_DIRECTORY = "../src/assets/img/";
    const std::vector<std::string> PIECE_NAMES = { "pawn", "knight", "bishop", "rook", "queen", "king" };
    const std::vector<std::string> COLORS = { "white", "black" };

    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    std::vector<sf::Texture> pieceTextures;

    static void loadTexture(sf::Texture& texture, const std::string& filename);
    void loadPieceTextures();
    void createPieces();
};
