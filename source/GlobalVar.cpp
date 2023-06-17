#include <GlobalVar.hpp>

namespace Constant {
    const double scaleX = (double)(sf::VideoMode::getDesktopMode().width) / (2880);
    const double scaleY = (double)(sf::VideoMode::getDesktopMode().height) / (1800);
    const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 144.f);
    const int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;
    const int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;
}

namespace Color {
    sf::Color NODE_COLOR = sf::Color::White;
    sf::Color NODE_TEXT_COLOR = sf::Color::Blue;
    sf::Color NODE_OUTLINE_COLOR = sf::Color::Red;
    sf::Color NODE_EDGE_COLOR = sf::Color(252, 186, 3, 255);
}
