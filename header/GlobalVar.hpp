#ifndef GLOBALVAR_HPP
#define GLOBALVAR_HPP

#include <SFML/Graphics.hpp>
#include <ResourceHolder.hpp>

namespace Constant {
    extern const double SCALE_X;
    extern const double SCALE_Y;
    extern const sf::Time TIME_PER_FRAME;
    extern const int WINDOW_WIDTH;
    extern const int WINDOW_HEIGHT;
};

namespace Color {
    extern sf::Color BACKGROUND_COLOR;
    extern sf::Color NODE_COLOR;
    extern sf::Color NODE_TEXT_COLOR;
    extern sf::Color NODE_OUTLINE_COLOR;
    extern sf::Color NODE_EDGE_COLOR;
};

namespace Size {
    extern const float NODE_RADIUS;
    extern const float EDGE_THICKNESS;
};

namespace Animation {
    extern float SPEED;
};

namespace ResourcesHolder {
    extern ResourceHolder<sf::Font, Fonts::ID> fontsHolder;
    extern ResourceHolder<sf::Texture, Textures::ID> texturesHolder;
};
#endif