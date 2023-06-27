#include <SceneGraph/ImageButton.hpp>

void ImageButton::set(
    const sf::Texture &buttonTexture, const sf::Texture &buttonHoveredTexture, 
    const sf::Vector2f &position, const sf::Vector2f &size
) {
    mButton.setTexture(buttonTexture);
    mButtonHovered.setTexture(buttonHoveredTexture);
    mButton.setPosition(position);
    mButtonHovered.setPosition(position);
    mButton.setScale(size.x / mButton.getLocalBounds().width, size.y / mButton.getLocalBounds().height);
    mButtonHovered.setScale(size.x / mButtonHovered.getLocalBounds().width, size.y / mButtonHovered.getLocalBounds().height);
}

void ImageButton::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    if (mIsHovered) {
        target.draw(mButtonHovered);
    } else {
        target.draw(mButton);
    }
}

void ImageButton::updateCurrent() {}

void ImageButton::handleCurrentEvent(sf::RenderWindow &window, sf::Event &event) {
    if (isHovered(window, event)) {
        mIsHovered = true;
    } else {
        mIsHovered = false;
    }
}

bool ImageButton::isHovered(sf::RenderWindow &window, sf::Event& event) {
    sf::Vector2i localPosition(sf::Mouse::getPosition(window));
    sf::Vector2f localPositionF(static_cast<float>(localPosition.x), static_cast<float>(localPosition.y));
    localPositionF = window.mapPixelToCoords(localPosition);
    sf::FloatRect dataBounds = mButton.getGlobalBounds();
    sf::Vector2f startPoint(dataBounds.left, dataBounds.top);
    sf::Vector2f endPoint(dataBounds.left + dataBounds.width, dataBounds.top + dataBounds.height);
    return (
        localPositionF.x >= dataBounds.left && 
        localPositionF.x <= dataBounds.left + dataBounds.width && 
        localPositionF.y >= dataBounds.top && 
        localPositionF.y <= dataBounds.top + dataBounds.height
    );
}

bool ImageButton::isLeftClicked(sf::RenderWindow &window, sf::Event &event) {
    return (isHovered(window, event) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.type == sf::Event::MouseButtonPressed);
}
