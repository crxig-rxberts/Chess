#include "Board.hpp"
#include "BoardEvent.hpp"
#include "Menu.hpp"

#include <unordered_map>

const unsigned int WINDOW_SIZE = 1600;
const unsigned int FRAME_RATE = 60;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Chess");
    window.setFramerateLimit(FRAME_RATE);

    Menu menu(window.getSize().x);
    Board board(window.getSize().x);

    std::unordered_map<sf::Event::EventType, std::function<void(sf::Event&)>> menuEventHandlers {
            { sf::Event::Closed, [&window](sf::Event const& event) {window.close(); } },
            { sf::Event::MouseButtonPressed, [&menu, &window](sf::Event const& event) { menu.handleEvent(event, window); } },
    };

    std::unordered_map<sf::Event::EventType, std::function<void(sf::Event&)>> boardEventHandlers {
            { sf::Event::Closed, [&window](sf::Event const& event) {window.close(); } },
            { sf::Event::MouseMoved, [&board](sf::Event const& event) { BoardEvent::handleMouseMoveEvent(board, event.mouseMove); } },
            { sf::Event::MouseButtonPressed, [&board](sf::Event const& event) { BoardEvent::handleMouseButtonEvent(board, event.mouseButton, true); } },
            { sf::Event::MouseButtonReleased, [&board](sf::Event const& event) { BoardEvent::handleMouseButtonEvent(board, event.mouseButton, false); } },
    };

    while (window.isOpen()) {
        if (Menu::Option::None == menu.getSelectedOption()) {
            sf::Event event{};
            while (window.pollEvent(event)) {
                if(menuEventHandlers.count(event.type) > 0) {
                    menuEventHandlers[event.type](event);
                }
            }

            window.clear();
            menu.draw(window);

        } else if (Menu::Option::PlayerVsPlayer == menu.getSelectedOption()) {
            sf::Event event{};
            while (window.pollEvent(event)) {
                if(boardEventHandlers.count(event.type) > 0) {
                    boardEventHandlers[event.type](event);
                }
            }

            window.clear();
            window.draw(board);
        }
        window.display();
    }
}
