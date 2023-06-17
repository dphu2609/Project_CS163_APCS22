#include <GlobalVar.hpp>

namespace Constant {
    const double scaleX = (double)(sf::VideoMode::getDesktopMode().width) / (2880);
    const double scaleY = (double)(sf::VideoMode::getDesktopMode().height) / (1800);
    const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 144.f);
}

namespace Color {
    sf::Color NODE_COLOR = sf::Color::White;
    sf::Color NODE_TEXT_COLOR = sf::Color::Black;
    sf::Color NODE_OUTLINE = sf::Color::Black;
}
