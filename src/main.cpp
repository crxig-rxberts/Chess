#include "Board.hpp"
#include "BoardEvent.hpp"

#include <unordered_map>

const unsigned int WINDOW_SIZE = 1600;
const unsigned int FRAME_RATE = 60;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Chess");
    window.setFramerateLimit(FRAME_RATE);

    Board board(window.getSize().x);

    std::unordered_map<sf::Event::EventType, std::function<void(sf::Event&)>> eventHandlers {
            { sf::Event::Closed, [&window](sf::Event& event) {window.close(); } },
            { sf::Event::MouseMoved, [&board](sf::Event& event) { BoardEvent::handleMouseMoveEvent(board, event.mouseMove); } },
            { sf::Event::MouseButtonPressed, [&board](sf::Event& event) { BoardEvent::handleMouseButtonEvent(board, event.mouseButton, true); } },
            { sf::Event::MouseButtonReleased, [&board](sf::Event& event) { BoardEvent::handleMouseButtonEvent(board, event.mouseButton, false); } },
    };

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if(eventHandlers.count(event.type) > 0) {
                eventHandlers[event.type](event);
            }
        }

        window.clear();
        window.draw(board);
        window.display();
    }
}