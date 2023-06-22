
#include "Board.hpp"
#include "BoardEvent.hpp"

int main() {

//    chess::Board board2(1200);
//    board2.pieceLayout = {
//            {-4, -2, -3, -5, -6, 0, -2, -4},
//            {-1, -1, -1, -1, 0, -1, -1, -1},
//            {0, 0, 0, 0, 0, 0, 0, 0},
//            {0, 0, 0, 0, 0, 0, 0, 0},
//            {0, -3, 0, 0, 0, 0, 0, 0},
//            {0, 0, 0, 0, 0, 0, 0, 0},
//            {1, 1, 1, 0, 1, 1, 1, 1},
//            {4, 2, 3, 5, 6, 3, 2, 4}
//    };

    sf::RenderWindow window(sf::VideoMode(1600, 1600), "Chess");
    window.setFramerateLimit(60);

    chess::Board board(window.getSize().x);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {

                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseButtonPressed:
                    chess::BoardEvent::handleMouseButtonEvent(board, event.mouseButton, true);
                    break;

                case sf::Event::MouseButtonReleased:
                    chess::BoardEvent::handleMouseButtonEvent(board, event.mouseButton, false);
                    break;

                case sf::Event::MouseMoved:
                    chess::BoardEvent::handleMouseMoveEvent(board, event.mouseMove);
                    break;

                default:
                    break;
            }
        }

        window.clear();
        window.draw(board);

        // check if a piece has been taken, rerender if so
        if (chess::BoardEvent::pieceHidden) {
            window.display();
            chess::BoardEvent::pieceHidden = false;
        }
        window.display();

    }
}