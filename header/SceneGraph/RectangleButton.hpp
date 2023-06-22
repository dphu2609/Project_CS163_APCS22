#ifndef RECTANGLEBUTTON_HPP
#define RECTANGLEBUTTON_HPP

#include <SFML/Graphics.hpp>
#include <SceneGraph/SceneNode.hpp>
#include <bits/stdc++.h>

class RectangleButton : public SceneNode {
private: 
    sf::RectangleShape mRectangle;
    sf::Text mText;
private:
    bool mIsHovered = false;
private:
    sf::Color mButtonColor;
    sf::Color mButtonHoveredColor;
    sf::Color mTextColor;
    sf::Color mTextHoveredColor;
    sf::Color mOutlineColor;
    sf::Color mOutlineHoveredColor;
public:
    void set(
        const sf::Vector2f &size, const sf::Vector2f &position, const std::string &text, 
        const sf::Font &font, const sf::Color &buttonColor, const sf::Color &textColor,  
        const sf::Color &buttonHoveredColor, const sf::Color &textHoveredColor,
        const float outlineThickness = 0.f, const sf::Color &outlineColor = sf::Color::Black, 
        const sf::Color &outlineHoveredColor = sf::Color::Black
    );
public: 
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent();
    virtual void handleCurrentEvent(sf::RenderWindow &window, sf::Event &event);  
public:
    virtual bool isLeftClicked(sf::RenderWindow &window, sf::Event &event);
    virtual bool isHovered(sf::RenderWindow &window, sf::Event &event);
private: 
    virtual void setVarForMove(const sf::Vector2f &positionAfterMove, const float speed);
    virtual void setVarForChange3Color(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const sf::Color &objectOutlineColorAfterChange, const float speed);
};

#endif