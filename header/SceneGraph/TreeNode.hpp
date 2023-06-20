#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <SceneGraph/SceneNode.hpp>

class TreeNode : public SceneNode {
private:
    sf::CircleShape mNode;
    sf::Text mText;
public:
    void set(const std::string &text, const sf::Vector2f position);
    virtual void setPosition(const sf::Vector2f &position);
    virtual sf::Vector2f getPosition();
private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent();
private: 
    virtual void setVarForZoom(const sf::Vector2f &sizeAfterZoom, const float speed);
    virtual void setVarForMove(const sf::Vector2f &positionAfterMove, const float speed);
    virtual void setVarForChange3Color(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const sf::Color &objectOutlineColorAfterChange, const float speed);
};

#endif