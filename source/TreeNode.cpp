#include <SceneGraph/TreeNode.hpp>

void TreeNode::set(const std::string &text, const sf::Font &font, const sf::Vector2f position) {
    mNode.setRadius(40.f * Constant::scaleX);
    mNode.setPosition(position);
    mNode.setFillColor(Color::NODE_COLOR);
    mNode.setOutlineThickness(5.f * Constant::scaleX);
    mNode.setOutlineColor(Color::NODE_OUTLINE_COLOR);

    if (text.size() == 0) return;

    mText.setFont(font);
    mText.setString(text);
    mText.setCharacterSize(50 * Constant::scaleX);
    mText.setFillColor(Color::NODE_TEXT_COLOR);
    mText.setPosition(position + sf::Vector2f(mText.getGlobalBounds().width/2, mText.getGlobalBounds().height/2));
}

sf::Vector2f TreeNode::getPosition() {
    return mNode.getPosition();
}

void TreeNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mNode);
    target.draw(mText);
}

void TreeNode::setPosition(const sf::Vector2f &position) {
    mNode.setPosition(position);
    mText.setPosition(position + sf::Vector2f(mText.getGlobalBounds().width/2, mText.getGlobalBounds().height/2));
}