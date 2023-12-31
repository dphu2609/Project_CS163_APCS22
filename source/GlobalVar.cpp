#include <GlobalVar.hpp>

namespace Constant {
    const double SCALE_X = (double)(sf::VideoMode::getDesktopMode().width) / (2880);
    const double SCALE_Y = (double)(sf::VideoMode::getDesktopMode().height) / (1800);
    const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 144.f);
    const int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;
    const int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;
};  

namespace Color {
    sf::Color BACKGROUND_COLOR = sf::Color(44, 54, 57);
    sf::Color NODE_COLOR = sf::Color(44, 54, 57);
    sf::Color NODE_TEXT_COLOR = sf::Color::White;
    sf::Color NODE_OUTLINE_COLOR = sf::Color::White;
    sf::Color NODE_EDGE_COLOR = sf::Color::White;
    sf::Color NODE_LABEL_COLOR = sf::Color(232, 54, 0);
    sf::Color NODE_HIGHLIGHT_COLOR = sf::Color(255, 171, 25);
    sf::Color NODE_HIGHLIGHT_TEXT_COLOR = sf::Color(44, 54, 57);
    sf::Color NODE_HIGHLIGHT_OUTLINE_COLOR = sf::Color(255, 181, 25);
    sf::Color SETTINGS_BUTTON_COLOR = sf::Color(255, 244, 214);
    sf::Color SETTINGS_BUTTON_TEXT_COLOR = sf::Color::Black;
    sf::Color SETTINGS_BUTTON_OUTLINE_COLOR = sf::Color::Black;
    sf::Color SETTINGS_BUTTON_HOVERED_COLOR = sf::Color(224, 134, 7);
    sf::Color SETTINGS_BUTTON_HOVERED_TEXT_COLOR = sf::Color::Black;
    sf::Color SETTINGS_BUTTON_HOVERED_OUTLINE_COLOR = sf::Color::Black;

    sf::Color INPUTBOX_COLOR = sf::Color(44, 54, 57);
    sf::Color INPUTBOX_TEXT_COLOR = sf::Color::White;
    sf::Color INPUTBOX_OUTLINE_COLOR = sf::Color::White;
    sf::Color INPUTBOX_CURSOR_COLOR = sf::Color::White;


    sf::Color MATRIX_SLOT_COLOR = sf::Color(44, 54, 57);
    sf::Color MATRIX_SLOT_TEXT_COLOR = sf::Color::White;
    sf::Color MATRIX_SLOT_OUTLINE_COLOR = sf::Color::White;

    sf::Color CODEBLOCK_BOX_COLOR = sf::Color(255, 244, 214);
    sf::Color CODEBLOCK_TEXT_COLOR = sf::Color::Black;
    sf::Color CODEBLOCK_BOX_ACTIVATED_COLOR = sf::Color(255, 217, 115);
    sf::Color CODEBLOCK_TEXT_ACTIVATED_COLOR = sf::Color::Black;

    sf::Color DATA_STRUCTURE_BUTTON_OUTLINE_COLOR = sf::Color::White;
    sf::Color DATA_STRUCTURE_BUTTON_OUTLINE_HOVERED_COLOR = sf::Color(255, 171, 25);

    sf::Color ERROR_MESSAGE_BOX_COLOR = sf::Color(44, 54, 57);
    sf::Color ERROR_MESSAGE_BOX_TEXT_COLOR = sf::Color::White;
    sf::Color ERROR_MESSAGE_BOX_OUTLINE_COLOR = sf::Color::Transparent;

    sf::Color THEME_BUTTON_COLOR = sf::Color(44, 54, 57);
    sf::Color THEME_BUTTON_TEXT_COLOR = sf::Color::White;
    sf::Color THEME_BUTTON_OUTLINE_COLOR = sf::Color::White;
    sf::Color THEME_BUTTON_HOVERED_COLOR = sf::Color::White;
    sf::Color THEME_BUTTON_HOVERED_TEXT_COLOR = sf::Color(116, 155, 194, 255);
    sf::Color THEME_BUTTON_HOVERED_OUTLINE_COLOR = sf::Color::White;

    std::vector<sf::Color> CONNECTED_COMPONENTS_COLOR = {
        sf::Color(5, 247, 183), sf::Color(2, 170, 247), sf::Color(133, 2, 247),
        sf::Color(247, 2, 145), sf::Color(247, 2, 35), sf::Color(230, 153, 0),
        sf::Color(222, 230, 0), sf::Color(29, 219, 0), sf::Color(255, 119, 0), 
        sf::Color(120, 83, 252)
    };

    bool IS_DARK_THEME = true;
};

namespace Size {
    float NODE_RADIUS = 40.f * Constant::SCALE_X;
    float NODE_RADIUS_X = 40.f * Constant::SCALE_X;
    float NODE_RADIUS_Y = 40.f * Constant::SCALE_Y;
    float EDGE_THICKNESS = 5.f * Constant::SCALE_Y;
    sf::Vector2f SETTINGS_BUTTON_SIZE = sf::Vector2f(400.f * Constant::SCALE_X, 80.f * Constant::SCALE_Y);
    sf::Vector2f MATRIX_SLOT_SIZE = sf::Vector2f(70.f * Constant::SCALE_X, 50.f * Constant::SCALE_Y);
    sf::Vector2f DATA_STRUCTURE_BUTTON = sf::Vector2f(500.f * Constant::SCALE_X, 400.f * Constant::SCALE_Y);
    sf::Vector2f SPEED_BUTTON_SIZE = sf::Vector2f(300.f * Constant::SCALE_X, 60.f * Constant::SCALE_Y);
    float DATA_STRUCTURE_BUTTON_EDGE_THICKNESS = 5.f * Constant::SCALE_Y;
};

namespace Animation {
    float SPEED = 1;
};

namespace ResourcesHolder {
    ResourceHolder<sf::Font, Fonts::ID> fontsHolder;
    ResourceHolder<sf::Texture, Textures::ID> texturesHolder;
};

namespace CodeContainer {
    CodeHolder codeHolder;
}
