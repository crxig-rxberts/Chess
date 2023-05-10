#pragma once

#include <SFML/Graphics.hpp>

namespace chess {
    class Piece : public sf::Drawable {
    public:
        Piece(const sf::Texture &texture, const sf::Vector2f &position, int pieceIndex);
        explicit Piece(const sf::Texture &texture);

        void setPosition(const sf::Vector2f &position, float scaleFactor = 1.0f);
        sf::Vector2f getPosition() const;
        sf::FloatRect getGlobalBounds() const;
        const sf::Vector2f &getScale() const;
        void setScale(const sf::Vector2f &scale);
        int getPieceIndex() const;


        bool isPawn() const {
            int index = abs(pieceIndex);
            return index == 1;
        }

        bool isKnight() const {
            int index = abs(pieceIndex);
            return index == 2;
        }

        bool isBishop() const {
            int index = abs(pieceIndex);
            return index == 3;
        }

        bool isRook() const {
            int index = abs(pieceIndex);
            return index == 4;
        }

        bool isQueen() const {
            int index = abs(pieceIndex);
            return index == 5;
        }

        bool isKing() const {
            int index = abs(pieceIndex);
            return index == 6;
        }


    private:
        sf::Sprite sprite;
        int pieceIndex;


        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    };
}
