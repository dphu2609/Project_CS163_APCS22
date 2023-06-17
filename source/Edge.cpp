#include <SceneGraph/Edge.hpp>

void Edge::set(const sf::Vector2f &position, const sf::Vector2f &size, const double &angle) {
    mEdge.setSize(size * (float)Constant::scaleX);
    mEdge.setFillColor(Color::NODE_EDGE_COLOR);
    mEdge.setPosition(position);
    mEdge.setRotation(angle);
}

void Edge::setPosition(const sf::Vector2f &position) {
    mEdge.setPosition(position);
}

void Edge::setPositionByPoints(const sf::Vector2f &point1, const sf::Vector2f &point2) {
    sf::Vector2f size(5, std::sqrt(std::pow(point2.x - point1.x, 2) + std::pow(point2.y - point1.y, 2)));
    sf::Vector2f position = point1;
    double angle = std::atan((point2.x - point1.x)/(point2.y - point1.y)) * 180 / std::acos(-1);
    set(position, size, -angle);
}

void Edge::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mEdge);
}