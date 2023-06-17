#include <SceneGraph/TreeNode.hpp>

void TreeNode::setNode(const std::string &text, const sf::Font &font, const sf::Vector2f position) {
    mNode.setRadius(40.f * Constants::scaleX);
    mNode.setPosition(position);
    mNode.setFillColor(Color::NODE_COLOR);
    mNode.setOutlineThickness(5.f * Constants::scaleX);
    mNode.setOutlineColor(Color::NODE_OUTLINE_COLOR);

    if (text.size() == 0) return;

    mText.setFont(font);
    mText.setString(text);
    mText.setCharacterSize(20 * Constants::scaleX * 1/text.size());
    mText.setFillColor(Color::NODE_TEXT_COLOR);
    mText.setPosition(position - sf::Vector2f(mText.getGlobalBounds().width/2, mText.getGlobalBounds().height/2));
}

sf::Vector2f TreeNode::getPosition() {
    return mNode.getPosition();
}