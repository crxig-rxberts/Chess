#pragma once

#include <SFML/Graphics.hpp>
#include "Piece.hpp"

namespace chess {
    class Board : public sf::Drawable {
    public:
        explicit Board(float size);

        void handleMouseButtonEvent(const sf::Event::MouseButtonEvent &event, bool isPressed);
        void handleMouseMoveEvent(const sf::Event::MouseMoveEvent &event);


    private:
        const float squareSize = 1600.0f / 8;


        sf::Texture boardTexture;
        sf::Sprite boardSprite;
        sf::Texture legalMoveTexture;
        sf::Sprite legalMoveSprite;
        std::vector<sf::Texture> pieceTextures;
        std::vector<Piece> pieces;
        std::vector<std::vector<int>> pieceLayout;
        std::vector<sf::Vector2i> possibleMoves;

        int draggedPieceIndex = -1;
        sf::Vector2f mouseOffset;
        sf::Vector2f originalScale;
        std::vector<sf::Sprite> legalMoveSprites;
        sf::Vector2f originalPos;



        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
        void loadPieceTextures();
        void createPieces();

        void findClickedPiece(float x, float y);
        void findLegalMoves(int pieceIndex);
        void releasePiece(float x, float y);
        void snapPieceToTileCenter(int col, int row);
        int getPieceIndexAtTile(int col, int row);
        std::vector<sf::Vector2i> findPossibleMovesForPieceType(const Piece &piece);
    };
}
