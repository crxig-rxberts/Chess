#include "../include/Piece.hpp"

namespace chess {
    Piece::Piece(const sf::Texture &texture, const sf::Vector2f &position, int pieceIndex)
            : sprite(texture), pieceIndex(pieceIndex), isVisible(true) {
        setPosition(position);
    }

    void Piece::setPosition(const sf::Vector2f &position, float scaleFactor) {
        sprite.setPosition(position);
        sprite.setScale(scaleFactor, scaleFactor);
    }

    sf::Vector2f Piece::getPosition() const {
        return sprite.getPosition();
    }

    void Piece::hide() {
        isVisible = false;
    }

    sf::FloatRect Piece::getGlobalBounds() const {
        return sprite.getGlobalBounds();
    }

    void Piece::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        if (isVisible) {
            target.draw(sprite, states);
        }
    }

    const sf::Vector2f &Piece::getScale() const {
        return sprite.getScale();
    }

    void Piece::setScale(const sf::Vector2f &scale) {
        sprite.setScale(scale);
    }
}
