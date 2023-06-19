#ifndef EDGE_HPP
#define EDGE_HPP

#include <SceneGraph/SceneNode.hpp>

class Edge : public SceneNode {
private: 
    sf::RectangleShape mEdge;
    sf::Vector2f startPoint;
    sf::Vector2f endPoint;
public:
    void set(const sf::Vector2f &position, const sf::Vector2f &size, const double &angle);
    virtual void setPosition(const sf::Vector2f &position);
    virtual void setPositionByPoints(const sf::Vector2f &point1, const sf::Vector2f &point2);
private:
    virtual void setVarForMove(const sf::Vector2f &positionAfterMove, const float speed);
    virtual void setVarForMoveBy2Points(const sf::Vector2f &position1AfterMove, const sf::Vector2f &position2AfterMove, const float speed);
private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent();
};

#endif