#include <SFML/Graphics.hpp>

class Menu {
public:
    explicit Menu(unsigned int window_size);

    enum class Option {
        None,
        PlayerVsPlayer,
        PlayerVsComputer
    };

    void draw(sf::RenderWindow& window) const;  // Make the function const if it doesn't modify the class state.
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    Option getSelectedOption() const;

private:
    const std::string MENU_TEXTURE_FILE = "../src/assets/img/menu.png";
    const std::string PVP_TEXTURE_FILE = "../src/assets/img/pvp_button.png";
    const std::string PVC_TEXTURE_FILE = "../src/assets/img/pvc_button.png";

    Option optionSelected{Option::None};  // Use in-class member initialization for simple types.
    sf::Texture texture, pvpTexture, pvcTexture;
    sf::Sprite sprite, pvpButton, pvcButton;

    bool loadTextures();  // Move the texture loading into its own function for clarity and maintainability.
    void positionButtons(unsigned int window_size);
};

