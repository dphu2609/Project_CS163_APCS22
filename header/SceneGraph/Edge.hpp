#ifndef EDGE_HPP
#define EDGE_HPP

#include <SceneGraph/SceneNode.hpp>

class Edge : public SceneNode {
private: 
    sf::RectangleShape mEdge;
public:
    void set(const sf::Vector2f &position, const sf::Vector2f &size, const double &angle);
    virtual void setPosition(const sf::Vector2f &position);
    virtual void setPositionByPoints(const sf::Vector2f &point1, const sf::Vector2f &point2);
private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif