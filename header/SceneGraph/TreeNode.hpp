#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <SceneGraph/SceneNode.hpp>

class TreeNode : public SceneNode {
private:
    sf::CircleShape mNode;
    sf::RectangleShape mRectangleNode;
    sf::Text mText;
    sf::Text mLabel;
    bool mIsCircle = true;
public:
    void set(
        bool isCircle,
        const std::string &text, const sf::Vector2f position, 
        const float &radius = Size::NODE_RADIUS, const sf::Color &nodeColor = Color::NODE_COLOR,
        const sf::Color &textColor = Color::NODE_TEXT_COLOR,
        const sf::Color &outlineColor = Color::NODE_OUTLINE_COLOR, const float &outlineThickness = Size::NODE_RADIUS / 8,
        const std::string &labelContent = "",
        const sf::Color &labelColor = Color::NODE_LABEL_COLOR
    );
    virtual void setPosition(const sf::Vector2f &position);
    virtual sf::Vector2f getPosition();
    virtual void setContent(const std::string &text);
    virtual void setLabel(const std::string &label);
    virtual std::string getContent();
private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent();
private: 
    virtual void setVarForZoom(const sf::Vector2f &sizeAfterZoom, const float speed);
    virtual void setVarForMove(const sf::Vector2f &positionAfterMove, const float speed);
    virtual void setVarForChange3Color(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const sf::Color &objectOutlineColorAfterChange, const float speed);
};

#endif