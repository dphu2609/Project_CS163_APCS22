#ifndef IMAGE_BUTTON_HPP
#define IMAGE_BUTTON_HPP

#include <SceneGraph/SceneNode.hpp>
#include <SFML/Graphics.hpp>

class ImageButton : public SceneNode {
private:
    sf::Sprite mButton;
    sf::Sprite mButtonHovered;
private:
    bool mIsHovered = false;
public:
    void set(
        const sf::Texture &buttonTexture, const sf::Texture &buttonHoveredTexture, 
        const sf::Vector2f &position, const sf::Vector2f &size = sf::Vector2f(50, 50)
    );
public: 
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent();
    virtual void handleCurrentEvent(sf::RenderWindow &window, sf::Event &event);  
public:
    virtual bool isLeftClicked(sf::RenderWindow &window, sf::Event &event);
    virtual bool isHovered(sf::RenderWindow &window, sf::Event &event);
};

#endif