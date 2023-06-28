#include <SceneGraph/RectangleButton.hpp>

void RectangleButton::set(
    const sf::Vector2f &size, const sf::Vector2f &position, const std::string &text, 
    const sf::Font &font, const sf::Color &buttonColor, const sf::Color &textColor, 
    const sf::Color &buttonHoveredColor, const sf::Color &textHoveredColor,
    const float outlineThickness, const sf::Color &outlineColor, const sf::Color &outlineHoveredColor
) {
    mRectangle.setSize(size);
    mRectangle.setPosition(position);
    mRectangle.setFillColor(buttonColor);
    mRectangle.setOutlineThickness(outlineThickness);
    mRectangle.setOutlineColor(outlineColor);


    mText.setFont(font);
    mText.setString(text);
    mText.setCharacterSize(size.y / 2);
    mText.setFillColor(textColor);
    mText.setPosition(position.x + size.x / 2 - mText.getGlobalBounds().width / 2, position.y + size.y / 2 - mText.getGlobalBounds().height);

    mButtonColor = buttonColor;
    mButtonHoveredColor = buttonHoveredColor;
    mTextColor = textColor;
    mTextHoveredColor = textHoveredColor;
    mOutlineColor = outlineColor;
    mOutlineHoveredColor = outlineHoveredColor;
}

void RectangleButton::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mRectangle);
    target.draw(mText);
}

void RectangleButton::updateCurrent() {
    if (mAnimationExecuting[Move]) {
        sf::Vector2f newPosition = mRectangle.getPosition();
        if (mCurrentPosition.x > 0) {
            if (mDeltaPosition.x >= 0) 
                newPosition.x = mStartPosition.x - std::pow(mCurrentPosition.x, 2) + mDeltaPosition.x;
            else 
                newPosition.x = mStartPosition.x + std::pow(mCurrentPosition.x, 2) + mDeltaPosition.x;
        }
        if (mCurrentPosition.y > 0) {
            if (mDeltaPosition.y >= 0) 
                newPosition.y = mStartPosition.y - std::pow(mCurrentPosition.y, 2) + mDeltaPosition.y;
            else 
                newPosition.y = mStartPosition.y + std::pow(mCurrentPosition.y, 2) + mDeltaPosition.y;
        }

        mRectangle.setPosition(newPosition);
        mText.setPosition(newPosition.x + mRectangle.getSize().x / 2 - mText.getGlobalBounds().width / 2, newPosition.y + mRectangle.getSize().y / 2 - mText.getGlobalBounds().height);

        if (mCurrentPosition.x > 0) mCurrentPosition.x -= mPositionJumpStep.x;
        if (mCurrentPosition.y > 0) mCurrentPosition.y -= mPositionJumpStep.y;

        if (mCurrentPosition.x <= 0 && mCurrentPosition.y <= 0) {
            mAnimationExecuting[Move] = false;
            mAnimationFinished[Move] = true;
        }
    }

    if (mAnimationExecuting[Change3Color]) {
        sf::Color newColor = mRectangle.getFillColor();
        if (mCurrentObjectColor.r > 0) {
            if (mDeltaObjectColor.r >= 0) 
                newColor.r = mStartObjectColor.r - std::pow(mCurrentObjectColor.r, 2) + mDeltaObjectColor.r;
            else 
                newColor.r = mStartObjectColor.r + std::pow(mCurrentObjectColor.r, 2) + mDeltaObjectColor.r;
        }
        if (mCurrentObjectColor.g > 0) {
            if (mDeltaObjectColor.g >= 0) 
                newColor.g = mStartObjectColor.g - std::pow(mCurrentObjectColor.g, 2) + mDeltaObjectColor.g;
            else 
                newColor.g = mStartObjectColor.g + std::pow(mCurrentObjectColor.g, 2) + mDeltaObjectColor.g;
        }
        if (mCurrentObjectColor.b > 0) {
            if (mDeltaObjectColor.b >= 0) 
                newColor.b = mStartObjectColor.b - std::pow(mCurrentObjectColor.b, 2) + mDeltaObjectColor.b;
            else 
                newColor.b = mStartObjectColor.b + std::pow(mCurrentObjectColor.b, 2) + mDeltaObjectColor.b;
        }
        if (mCurrentObjectColor.a > 0) {
            if (mDeltaObjectColor.a >= 0) 
                newColor.a = mStartObjectColor.a - std::pow(mCurrentObjectColor.a, 2) + mDeltaObjectColor.a;
            else 
                newColor.a = mStartObjectColor.a + std::pow(mCurrentObjectColor.a, 2) + mDeltaObjectColor.a;
        }
        mRectangle.setFillColor(newColor);
        
        if (mCurrentObjectColor.r > 0) mCurrentObjectColor.r -= mObjectColorJumpStep.r;
        if (mCurrentObjectColor.g > 0) mCurrentObjectColor.g -= mObjectColorJumpStep.g;
        if (mCurrentObjectColor.b > 0) mCurrentObjectColor.b -= mObjectColorJumpStep.b;
        if (mCurrentObjectColor.a > 0) mCurrentObjectColor.a -= mObjectColorJumpStep.a;

        sf::Color newTextColor = mText.getFillColor();
        if (mCurrentTextColor.r > 0) {
            if (mDeltaTextColor.r >= 0) 
                newTextColor.r = mStartTextColor.r - std::pow(mCurrentTextColor.r, 2) + mDeltaTextColor.r;
            else 
                newTextColor.r = mStartTextColor.r + std::pow(mCurrentTextColor.r, 2) + mDeltaTextColor.r;
        }
        if (mCurrentTextColor.g > 0) {
            if (mDeltaTextColor.g >= 0) 
                newTextColor.g = mStartTextColor.g - std::pow(mCurrentTextColor.g, 2) + mDeltaTextColor.g;
            else 
                newTextColor.g = mStartTextColor.g + std::pow(mCurrentTextColor.g, 2) + mDeltaTextColor.g;
        }
        if (mCurrentTextColor.b > 0) {
            if (mDeltaTextColor.b >= 0) 
                newTextColor.b = mStartTextColor.b - std::pow(mCurrentTextColor.b, 2) + mDeltaTextColor.b;
            else 
                newTextColor.b = mStartTextColor.b + std::pow(mCurrentTextColor.b, 2) + mDeltaTextColor.b;
        }
        if (mCurrentTextColor.a > 0) {
            if (mDeltaTextColor.a >= 0) 
                newTextColor.a = mStartTextColor.a - std::pow(mCurrentTextColor.a, 2) + mDeltaTextColor.a;
            else 
                newTextColor.a = mStartTextColor.a + std::pow(mCurrentTextColor.a, 2) + mDeltaTextColor.a;
        }

        mText.setFillColor(newTextColor);

        if (mCurrentTextColor.r > 0) mCurrentTextColor.r -= mTextColorJumpStep.r;
        if (mCurrentTextColor.g > 0) mCurrentTextColor.g -= mTextColorJumpStep.g;
        if (mCurrentTextColor.b > 0) mCurrentTextColor.b -= mTextColorJumpStep.b;
        if (mCurrentTextColor.a > 0) mCurrentTextColor.a -= mTextColorJumpStep.a;

        sf::Color newOutlineColor = mRectangle.getOutlineColor();
        if (mCurrentOutlineColor.r > 0) {
            if (mDeltaOutlineColor.r >= 0) 
                newOutlineColor.r = mStartOutlineColor.r - std::pow(mCurrentOutlineColor.r, 2) + mDeltaOutlineColor.r;
            else 
                newOutlineColor.r = mStartOutlineColor.r + std::pow(mCurrentOutlineColor.r, 2) + mDeltaOutlineColor.r;
        }
        if (mCurrentOutlineColor.g > 0) {
            if (mDeltaOutlineColor.g >= 0) 
                newOutlineColor.g = mStartOutlineColor.g - std::pow(mCurrentOutlineColor.g, 2) + mDeltaOutlineColor.g;
            else 
                newOutlineColor.g = mStartOutlineColor.g + std::pow(mCurrentOutlineColor.g, 2) + mDeltaOutlineColor.g;
        }
        if (mCurrentOutlineColor.b > 0) {
            if (mDeltaOutlineColor.b >= 0) 
                newOutlineColor.b = mStartOutlineColor.b - std::pow(mCurrentOutlineColor.b, 2) + mDeltaOutlineColor.b;
            else 
                newOutlineColor.b = mStartOutlineColor.b + std::pow(mCurrentOutlineColor.b, 2) + mDeltaOutlineColor.b;
        }
        if (mCurrentOutlineColor.a > 0) {
            if (mDeltaOutlineColor.a >= 0) 
                newOutlineColor.a = mStartOutlineColor.a - std::pow(mCurrentOutlineColor.a, 2) + mDeltaOutlineColor.a;
            else 
                newOutlineColor.a = mStartOutlineColor.a + std::pow(mCurrentOutlineColor.a, 2) + mDeltaOutlineColor.a;
        }

        mRectangle.setOutlineColor(newOutlineColor);

        if (mCurrentOutlineColor.r > 0) mCurrentOutlineColor.r -= mOutlineColorJumpStep.r;
        if (mCurrentOutlineColor.g > 0) mCurrentOutlineColor.g -= mOutlineColorJumpStep.g;
        if (mCurrentOutlineColor.b > 0) mCurrentOutlineColor.b -= mOutlineColorJumpStep.b;
        if (mCurrentOutlineColor.a > 0) mCurrentOutlineColor.a -= mOutlineColorJumpStep.a;

        if (
            mCurrentObjectColor.r <= 0 && mCurrentObjectColor.g <= 0 && mCurrentObjectColor.b <= 0 && mCurrentObjectColor.a <= 0 &&
            mCurrentTextColor.r <= 0 && mCurrentTextColor.g <= 0 && mCurrentTextColor.b <= 0 && mCurrentTextColor.a <= 0 &&
            mCurrentOutlineColor.r <= 0 && mCurrentOutlineColor.g <= 0 && mCurrentOutlineColor.b <= 0 && mCurrentOutlineColor.a <= 0
        ) {
            mAnimationExecuting[Change3Color] = false;
            mAnimationFinished[Change3Color] = true;
        }
    }

}

void RectangleButton::handleCurrentEvent(sf::RenderWindow &window, sf::Event &event) {
    if (isHovered(window, event)) {
        if (!mIsHovered) {
            mIsHovered = true;
            setVarForChange3Color(mButtonHoveredColor, mTextHoveredColor, mOutlineHoveredColor, 5 * (1 / Animation::SPEED));
        }
    } 
    else if (mIsHovered) {
        setVarForChange3Color(mButtonColor, mTextColor, mOutlineColor, 5 * (1 / Animation::SPEED));
        mIsHovered = false;
    }
}

bool RectangleButton::isHovered(sf::RenderWindow &window, sf::Event& event) {
    sf::Vector2i localPosition(sf::Mouse::getPosition(window));
    sf::Vector2f localPositionF(static_cast<float>(localPosition.x), static_cast<float>(localPosition.y));
    localPositionF = window.mapPixelToCoords(localPosition);
    sf::FloatRect dataBounds = mRectangle.getGlobalBounds();
    sf::Vector2f startPoint(dataBounds.left, dataBounds.top);
    sf::Vector2f endPoint(dataBounds.left + dataBounds.width, dataBounds.top + dataBounds.height);
    return (
        localPositionF.x >= dataBounds.left && 
        localPositionF.x <= dataBounds.left + dataBounds.width && 
        localPositionF.y >= dataBounds.top && 
        localPositionF.y <= dataBounds.top + dataBounds.height
    );
}



bool RectangleButton::isLeftClicked(sf::RenderWindow &window, sf::Event &event) {
    return (isHovered(window, event) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.type == sf::Event::MouseButtonPressed);
}

void RectangleButton::setVarForMove(const sf::Vector2f &positionAfterMove, const float speed) {
    mStartPosition = mRectangle.getPosition();
    mDeltaPosition = positionAfterMove - mStartPosition;
    mCurrentPosition = sf::Vector2f(std::sqrt(std::abs(mDeltaPosition.x)), std::sqrt(std::abs(mDeltaPosition.y)));
    mPositionJumpStep = sf::Vector2f(mCurrentPosition * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);
    this->mAnimationExecuting[Move] = true;
    this->mAnimationFinished[Move] = false;
}

void RectangleButton::setVarForChange3Color(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const sf::Color &objectOutlineColorAfterChange, const float speed) {
    mStartObjectColor = mRectangle.getFillColor();
    mDeltaObjectColor = RGBA(objectColorAfterChange.r, objectColorAfterChange.g, objectColorAfterChange.b, objectColorAfterChange.a) - RGBA(mStartObjectColor.r, mStartObjectColor.g, mStartObjectColor.b, mStartObjectColor.a);
    mCurrentObjectColor = RGBA(std::sqrt(std::abs(mDeltaObjectColor.r)), std::sqrt(std::abs(mDeltaObjectColor.g)), std::sqrt(std::abs(mDeltaObjectColor.b)), std::sqrt(std::abs(mDeltaObjectColor.a)));
    mObjectColorJumpStep = RGBA(mCurrentObjectColor.r * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentObjectColor.g * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentObjectColor.b * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentObjectColor.a * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);

    mStartTextColor = mText.getFillColor();
    mDeltaTextColor = RGBA(textColorAfterChange.r, textColorAfterChange.g, textColorAfterChange.b, textColorAfterChange.a) - RGBA(mStartTextColor.r, mStartTextColor.g, mStartTextColor.b, mStartTextColor.a);
    mCurrentTextColor = RGBA(std::sqrt(std::abs(mDeltaTextColor.r)), std::sqrt(std::abs(mDeltaTextColor.g)), std::sqrt(std::abs(mDeltaTextColor.b)), std::sqrt(std::abs(mDeltaTextColor.a)));
    mTextColorJumpStep = RGBA(mCurrentTextColor.r * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentTextColor.g * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentTextColor.b * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentTextColor.a * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);

    mStartOutlineColor = mRectangle.getOutlineColor();
    mDeltaOutlineColor = RGBA(objectOutlineColorAfterChange.r, objectOutlineColorAfterChange.g, objectOutlineColorAfterChange.b, objectOutlineColorAfterChange.a) - RGBA(mStartOutlineColor.r, mStartOutlineColor.g, mStartOutlineColor.b, mStartOutlineColor.a);
    mCurrentOutlineColor = RGBA(std::sqrt(std::abs(mDeltaOutlineColor.r)), std::sqrt(std::abs(mDeltaOutlineColor.g)), std::sqrt(std::abs(mDeltaOutlineColor.b)), std::sqrt(std::abs(mDeltaOutlineColor.a)));
    mOutlineColorJumpStep = RGBA(mCurrentOutlineColor.r * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentOutlineColor.g * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentOutlineColor.b * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentOutlineColor.a * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);

    this->mAnimationExecuting[Change3Color] = true;
    this->mAnimationFinished[Change3Color] = false;
}
