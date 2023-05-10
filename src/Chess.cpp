#include "../include/Board.hpp"
#include "../include/Chess.hpp"

#include <SFML/Graphics.hpp>

namespace chess {

    int Chess::run() {
        sf::RenderWindow window(sf::VideoMode(1600, 1600), "Chess Game");
        window.setFramerateLimit(12);

        Board board(window.getSize().x);

        while (window.isOpen()) {
            while (window.isOpen()) {
                sf::Event event{};
                while (window.pollEvent(event)) {
                    switch (event.type) {
                        case sf::Event::Closed:
                            window.close();
                            break;

                            // Handle mouse button events
                        case sf::Event::MouseButtonPressed:
                            board.handleMouseButtonEvent(event.mouseButton, true);
                            break;
                        case sf::Event::MouseButtonReleased:
                            board.handleMouseButtonEvent(event.mouseButton, false);
                            break;

                            // Handle mouse move events
                        case sf::Event::MouseMoved:
                            board.handleMouseMoveEvent(event.mouseMove);
                            break;

                        default:
                            break;
                    }
                }

                window.clear();
                window.draw(board);
                window.display();
            }

        }

        return 0;
    }
}
