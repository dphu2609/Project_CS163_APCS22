#include <SceneGraph/TreeNode.hpp>

void TreeNode::set(const std::string &text, const sf::Font &font, const sf::Vector2f position) {
    mNode.setRadius(Size::NODE_RADIUS);
    mNode.setPosition(position);
    mNode.setFillColor(Color::NODE_COLOR);
    mNode.setOutlineThickness(Size::NODE_RADIUS / 8);
    mNode.setOutlineColor(Color::NODE_OUTLINE_COLOR);

    if (text.size() == 0) return;

    mText.setFont(font);
    mText.setString(text);
    mText.setCharacterSize(Size::NODE_RADIUS);
    mText.setFillColor(Color::NODE_TEXT_COLOR);
    mText.setPosition(position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS) - sf::Vector2f(mText.getGlobalBounds().width/2, mText.getGlobalBounds().height));
}

sf::Vector2f TreeNode::getPosition() {
    return mNode.getPosition();
}

void TreeNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mNode);
    target.draw(mText);
}

void TreeNode::updateCurrent() {
    if (this->mAnimationExecuting[Zoom]) {
        float newRadius;
        if (this->mDeltaSize.x >= 0)
            newRadius = this->mStartSize.x - std::pow(this->mCurrentSize.x, 2) + this->mDeltaSize.x; 
        else
            newRadius = this->mStartSize.x + std::pow(this->mCurrentSize.x, 2) + this->mDeltaSize.x;
        sf::Vector2f oldPosition = mNode.getPosition() + sf::Vector2f(mNode.getRadius(), mNode.getRadius());
        mNode.setRadius(newRadius);
        mNode.setOutlineThickness(newRadius / 8);
        mNode.setPosition(oldPosition - sf::Vector2f(newRadius, newRadius));
        mText.setCharacterSize(newRadius);
        mText.setPosition(mNode.getPosition() + sf::Vector2f(newRadius, newRadius) - sf::Vector2f(mText.getGlobalBounds().width/2, mText.getGlobalBounds().height));
        if (this->mCurrentSize.x > 0) this->mCurrentSize -= this->mSizeJumpStep;
        else {
            this->mAnimationExecuting[Zoom] = false;
            this->mAnimationFinished[Zoom] = true;
        }
    }

    if (mAnimationExecuting[Move]) {
        sf::Vector2f newPosition = mNode.getPosition();
        if (mCurrentPosition.x > 0) {
            if (mDeltaPosition.x >= 0) 
                newPosition.x = mStartPosition.x - std::pow(mCurrentPosition.x, 2) + mDeltaPosition.x;
            else 
                newPosition.x = mStartPosition.x + std::pow(mCurrentPosition.x, 2) + mDeltaPosition.x;
        }
        if (mCurrentPosition.y > 0) {
            if (mDeltaPosition1.y >= 0) 
                newPosition.y = mStartPosition.y - std::pow(mCurrentPosition.y, 2) + mDeltaPosition.y;
            else 
                newPosition.y = mStartPosition.y + std::pow(mCurrentPosition.y, 2) + mDeltaPosition.y;
        }

        mNode.setPosition(newPosition);

        if (mCurrentPosition.x > 0) mCurrentPosition.x -= mPositionJumpStep.x;
        if (mCurrentPosition.y > 0) mCurrentPosition.y -= mPositionJumpStep.y;

        if (mCurrentPosition.x <= 0 && mCurrentPosition.y <= 0) {
            mAnimationExecuting[Move] = false;
            mAnimationFinished[Move] = true;
        }
    }
}

void TreeNode::setPosition(const sf::Vector2f &position) {
    mNode.setPosition(position);
    mText.setPosition(position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS) - sf::Vector2f(mText.getGlobalBounds().width/2, mText.getGlobalBounds().height));
}

void TreeNode::zoom(const sf::Vector2f &sizeAfterZoom, const float speed) {
    this->mStartSize = sf::Vector2f(mNode.getRadius(), 0);
    this->mDeltaSize = sizeAfterZoom - sf::Vector2f(mNode.getRadius(), 0);
    this->mCurrentSize = sf::Vector2f(std::sqrt(std::abs(sizeAfterZoom.x - mNode.getRadius())), 0);
    this->mSizeJumpStep = sf::Vector2f(this->mCurrentSize * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);
    this->mAnimationExecuting[Zoom] = true;
    this->mAnimationFinished[Zoom] = false;
}

void TreeNode::move(const sf::Vector2f &positionAfterMove, const float speed) {
    mStartPosition = mNode.getPosition();
    mDeltaPosition = positionAfterMove - startPoint;
    mCurrentPosition = sf::Vector2f(std::sqrt(std::abs(mDeltaPosition.x)), std::sqrt(std::abs(mDeltaPosition.y)));
    mPositionJumpStep = sf::Vector2f(mCurrentPosition * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);
}