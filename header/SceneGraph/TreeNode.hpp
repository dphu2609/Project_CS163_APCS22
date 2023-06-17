#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <SceneGraph/SceneNode.hpp>

class TreeNode : public SceneNode {
private:
    sf::CircleShape mNode;
    sf::Text mText;
public:
    void setNode(const std::string &text, const sf::Font &font, const sf::Vector2f position);
    sf::Vector2f getPosition();
};

#endif