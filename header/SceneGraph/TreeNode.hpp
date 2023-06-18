#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <SceneGraph/SceneNode.hpp>

class TreeNode : public SceneNode {
private:
    sf::CircleShape mNode;
    sf::Text mText;
public:
    void set(const std::string &text, const sf::Font &font, const sf::Vector2f position);
    virtual void setPosition(const sf::Vector2f &position);
    virtual sf::Vector2f getPosition();
private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent();
public:
    virtual void zoom(const sf::Vector2f &sizeAfterZoom, const float speed = 1.f);
    virtual void move(const sf::Vector2f &positionAfterMove, const float speed = 1.f);
    virtual void changeColor(const sf::Color &objectColorAfterChange, const float speed = 1.f) {}
    virtual void changeColor(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const float speed = 1.f) {}
    virtual void changeColor(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const sf::Color &objectOutlineColorAfterChange, const float speed = 1.f) {}
};

#endif