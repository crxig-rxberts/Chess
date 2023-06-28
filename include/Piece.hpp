#pragma once
#include <SFML/Graphics.hpp>

class Piece : public sf::Drawable {
public:
    Piece(const sf::Texture &texture, const sf::Vector2f &position, int pieceIndex);

    void setPosition(const sf::Vector2f &position, float scaleFactor = 1.0f);
    sf::Vector2f getPosition() const;
    void hide();
    sf::FloatRect getGlobalBounds() const;
    const sf::Vector2f &getScale() const;
    void setScale(const sf::Vector2f &scale);
    int getPieceIndex() const;

    bool isPawn() const { return abs(pieceIndex) == 1; }
    bool isKnight() const { return abs(pieceIndex) == 2; }
    bool isBishop() const { return abs(pieceIndex) == 3; }
    bool isRook() const { return abs(pieceIndex) == 4; }
    bool isQueen() const { return abs(pieceIndex) == 5; }
    bool isKing() const { return abs(pieceIndex) == 6; }

    bool isVisible;

private:
    sf::Sprite sprite;
    int pieceIndex;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
