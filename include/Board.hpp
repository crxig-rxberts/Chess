#pragma once

#include <SFML/Graphics.hpp>
#include "Piece.hpp"

namespace chess {

    class Board : public sf::Drawable {

    public:
        explicit Board(float size);

        const float tileSize = 1600.0f / 8;
        std::vector<std::vector<int>> pieceLayout;
        std::vector<Piece> pieces;

    private:

        sf::Texture boardTexture;
        sf::Sprite boardSprite;
        std::vector<sf::Texture> pieceTextures;

        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
        void loadPieceTextures();
        void createPieces();
    };
}
