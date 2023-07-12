#include <SFML/Graphics.hpp>

class Menu {
public:
    explicit Menu(unsigned int window_size);

    enum class Option {
        None,
        PlayerVsPlayer,
        PlayerVsComputer
    };

    void draw(sf::RenderWindow& window) const;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    Option getSelectedOption() const;

private:
    const std::string MENU_TEXTURE_FILE = "../src/assets/img/menu.png";
    const std::string PVP_TEXTURE_FILE = "../src/assets/img/pvp_button.png";
    const std::string PVC_TEXTURE_FILE = "../src/assets/img/pvc_button.png";

    Option optionSelected{Option::None};
    sf::Texture texture;
    sf::Texture pvpTexture;
    sf::Texture pvcTexture;
    sf::Sprite sprite;
    sf::Sprite pvpButton;
    sf::Sprite pvcButton;

    bool loadTextures();
    void positionButtons(unsigned int window_size);
};

