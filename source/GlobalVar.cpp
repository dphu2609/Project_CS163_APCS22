#include <GlobalVar.hpp>

namespace Constant {
    const double SCALE_X = (double)(sf::VideoMode::getDesktopMode().width) / (2880);
    const double SCALE_Y = (double)(sf::VideoMode::getDesktopMode().height) / (1800);
    const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 144.f);
    const int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;
    const int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;
};

namespace Color {
    sf::Color BACKGROUND_COLOR = sf::Color::White;
    sf::Color NODE_COLOR = sf::Color::White;
    sf::Color NODE_TEXT_COLOR = sf::Color(133, 163, 137, 255);
    sf::Color NODE_OUTLINE_COLOR = sf::Color(133, 163, 137, 255);
    sf::Color NODE_EDGE_COLOR = sf::Color::Black;
    sf::Color NODE_HIGHLIGHT_COLOR = sf::Color(255, 171, 25);
    sf::Color NODE_HIGHLIGHT_TEXT_COLOR = sf::Color::White;
    sf::Color NODE_HIGHLIGHT_OUTLINE_COLOR = sf::Color(255, 181, 25);
};

namespace Size {
    float NODE_RADIUS = 40.f * Constant::SCALE_X;
    float EDGE_THICKNESS = 5.f * Constant::SCALE_Y;
    sf::Vector2f SETTINGS_BUTTON_SIZE = sf::Vector2f(400.f * Constant::SCALE_X, 80.f * Constant::SCALE_Y);
};

namespace Animation {
    float SPEED = 1;
};

namespace ResourcesHolder {
    ResourceHolder<sf::Font, Fonts::ID> fontsHolder;
    ResourceHolder<sf::Texture, Textures::ID> texturesHolder;
};
