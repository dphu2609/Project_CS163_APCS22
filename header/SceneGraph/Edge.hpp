#ifndef EDGE_HPP
#define EDGE_HPP

#include <SceneGraph/SceneNode.hpp>

class Edge : public SceneNode {
private: 
    sf::RectangleShape mEdge;
    sf::Text mLabel;
    sf::Vector2f startPoint;
    sf::Vector2f endPoint;
public:
    void set(const sf::Vector2f &position1, const sf::Vector2f &position2, const float &edgeThickness = Size::EDGE_THICKNESS, const sf::Color &edgeColor = sf::Color::Black, const std::string &label = "");
    virtual void setPosition(const sf::Vector2f &position);
    virtual void setPositionByPoints(const sf::Vector2f &point1, const sf::Vector2f &point2, const float &edgeThickness = Size::EDGE_THICKNESS);
private:
    virtual void setVarForMove(const sf::Vector2f &positionAfterMove, const float speed);
    virtual void setVarForMoveBy2Points(const sf::Vector2f &position1AfterMove, const sf::Vector2f &position2AfterMove, const float speed);
    virtual void setVarForChange1Color(const sf::Color &objectColorAfterChange, const float speed);
private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent();
};

#endif