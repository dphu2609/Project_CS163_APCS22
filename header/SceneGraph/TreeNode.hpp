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
    sf::Vector2f getPosition();
private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif