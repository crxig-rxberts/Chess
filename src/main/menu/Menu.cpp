#include "Menu.hpp"
#include <iostream>

Menu::Menu(unsigned int window_size) {
    if (!loadTextures()) {
        std::cerr << "Error loading texture Menu Textures"  << std::endl;
        std::exit(1);
    }

    sprite.setTexture(texture);
    sprite.scale(static_cast<float>(window_size) / static_cast<float>(texture.getSize().x),
                 static_cast<float>(window_size) / static_cast<float>(texture.getSize().y));

    pvpButton.setTexture(pvpTexture);
    pvcButton.setTexture(pvcTexture);

    positionButtons(window_size);
}

bool Menu::loadTextures() {
    return texture.loadFromFile(MENU_TEXTURE_FILE) &&
           pvpTexture.loadFromFile(PVP_TEXTURE_FILE) &&
           pvcTexture.loadFromFile(PVC_TEXTURE_FILE);
}

void Menu::positionButtons(unsigned int window_size) {
    float buttonWidth = pvpTexture.getSize().x;
    float buttonHeight = pvpTexture.getSize().y;

    float spaceBetweenButtons = static_cast<float>(window_size) * 0.05f;  // Adjust this value as needed.
    float totalWidth = 2 * buttonWidth + spaceBetweenButtons;

    pvpButton.setPosition((static_cast<float>(window_size) - totalWidth) / 2.f, (static_cast<float>(window_size) - buttonHeight) / 2.f);
    pvcButton.setPosition((static_cast<float>(window_size) + totalWidth) / 2.f - buttonWidth, (static_cast<float>(window_size) - buttonHeight) / 2.f);
}

void Menu::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    window.draw(pvpButton);
    window.draw(pvcButton);
}

void Menu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if(event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i position = sf::Mouse::getPosition(window);
        if (pvpButton.getGlobalBounds().contains(position.x, position.y)) {
            optionSelected = Option::PlayerVsPlayer;
        }
        else if (pvcButton.getGlobalBounds().contains(position.x, position.y)) {
            optionSelected = Option::PlayerVsComputer;
        }
    }
}

Menu::Option Menu::getSelectedOption() const {
    return optionSelected;
}
