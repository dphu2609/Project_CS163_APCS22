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
    extern sf::Color NODE_LABEL_COLOR;

    extern sf::Color NODE_HIGHLIGHT_COLOR;
    extern sf::Color NODE_HIGHLIGHT_TEXT_COLOR;
    extern sf::Color NODE_HIGHLIGHT_OUTLINE_COLOR;

    extern sf::Color SETTINGS_BUTTON_COLOR;
    extern sf::Color SETTINGS_BUTTON_TEXT_COLOR;
    extern sf::Color SETTINGS_BUTTON_OUTLINE_COLOR;

    extern sf::Color INPUTBOX_COLOR;
    extern sf::Color INPUTBOX_TEXT_COLOR;
    extern sf::Color INPUTBOX_OUTLINE_COLOR;
    extern sf::Color INPUTBOX_CURSOR_COLOR;

    extern sf::Color SETTINGS_BUTTON_HOVERED_COLOR;
    extern sf::Color SETTINGS_BUTTON_HOVERED_TEXT_COLOR;
    extern sf::Color SETTINGS_BUTTON_HOVERED_OUTLINE_COLOR;

    extern sf::Color MATRIX_SLOT_COLOR;
    extern sf::Color MATRIX_SLOT_TEXT_COLOR;
    extern sf::Color MATRIX_SLOT_OUTLINE_COLOR;
};

namespace Size {
    extern float NODE_RADIUS;
    extern float NODE_RADIUS_X;
    extern float NODE_RADIUS_Y;
    extern float EDGE_THICKNESS;
    extern sf::Vector2f SETTINGS_BUTTON_SIZE;
    extern sf::Vector2f MATRIX_SLOT_SIZE;
};

namespace Animation {
    extern float SPEED;
};

namespace ResourcesHolder {
    extern ResourceHolder<sf::Font, Fonts::ID> fontsHolder;
    extern ResourceHolder<sf::Texture, Textures::ID> texturesHolder;
};
#endif