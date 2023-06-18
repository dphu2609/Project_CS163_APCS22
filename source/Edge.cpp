#include <SceneGraph/Edge.hpp>

void Edge::set(const sf::Vector2f &position, const sf::Vector2f &size, const double &angle) {
    mEdge.setSize(size);
    mEdge.setFillColor(Color::NODE_EDGE_COLOR);
    mEdge.setPosition(position);
    mEdge.setRotation(angle);
}

void Edge::setPosition(const sf::Vector2f &position) {
    mEdge.setPosition(position);
}

void Edge::setPositionByPoints(const sf::Vector2f &point1, const sf::Vector2f &point2) {
    sf::Vector2f size(Size::EDGE_THICKNESS, std::sqrt(std::pow(point2.x - point1.x, 2) + std::pow(point2.y - point1.y, 2)) * Constant::SCALE_X);
    sf::Vector2f position = point1;
    double angle = std::atan((point2.x - point1.x)/(point2.y - point1.y)) * 180 / std::acos(-1);
    set(position, size, -angle);
    startPoint = point1;
    endPoint = point2;
}

void Edge::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mEdge);
}

void Edge::updateCurrent() {
    if (mAnimationExecuting[Move]) {
        sf::Vector2f newPosition = startPoint;
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

        mEdge.setPosition(newPosition);
        startPoint = newPosition;

        if (mCurrentPosition.x > 0) mCurrentPosition.x -= mPositionJumpStep.x;
        if (mCurrentPosition.y > 0) mCurrentPosition.y -= mPositionJumpStep.y;

        if (mCurrentPosition.x <= 0 && mCurrentPosition.y <= 0) {
            mAnimationExecuting[Move] = false;
            mAnimationFinished[Move] = true;
        }
    }

    if (mAnimationExecuting[MoveBy2Points]) {
        sf::Vector2f newStartPosition = startPoint;
        if (mCurrentPosition1.x > 0) {
            if (mDeltaPosition1.x >= 0) 
                newStartPosition.x = mStartPosition1.x - std::pow(mCurrentPosition1.x, 2) + mDeltaPosition1.x;
            else 
                newStartPosition.x = mStartPosition1.x + std::pow(mCurrentPosition1.x, 2) + mDeltaPosition1.x;
        }
        if (mCurrentPosition1.y > 0) {
            if (mDeltaPosition1.y >= 0) 
                newStartPosition.y = mStartPosition1.y - std::pow(mCurrentPosition1.y, 2) + mDeltaPosition1.y;
            else 
                newStartPosition.y = mStartPosition1.y + std::pow(mCurrentPosition1.y, 2) + mDeltaPosition1.y;
        }

        sf::Vector2f newEndPosition = endPoint;
        if (mCurrentPosition2.x > 0) {
            if (mDeltaPosition2.x >= 0) 
                newEndPosition.x = mStartPosition2.x - std::pow(mCurrentPosition2.x, 2) + mDeltaPosition2.x;
            else 
                newEndPosition.x = mStartPosition2.x + std::pow(mCurrentPosition2.x, 2) + mDeltaPosition2.x;
        }
        if (mCurrentPosition2.y > 0) {
            if (mDeltaPosition2.y >= 0) 
                newEndPosition.y = mStartPosition2.y - std::pow(mCurrentPosition2.y, 2) + mDeltaPosition2.y;
            else 
                newEndPosition.y = mStartPosition2.y + std::pow(mCurrentPosition2.y, 2) + mDeltaPosition2.y;
        }
        
        setPositionByPoints(newStartPosition, newEndPosition);

        if (mCurrentPosition1.x > 0) mCurrentPosition1.x -= mPositionJumpStep1.x;
        if (mCurrentPosition1.y > 0) mCurrentPosition1.y -= mPositionJumpStep1.y;
        if (mCurrentPosition2.x > 0) mCurrentPosition2.x -= mPositionJumpStep2.x;
        if (mCurrentPosition2.y > 0) mCurrentPosition2.y -= mPositionJumpStep2.y;
        if (mCurrentPosition1.x <= 0 && mCurrentPosition1.y <= 0 && mCurrentPosition2.x <= 0 && mCurrentPosition2.y <= 0) {
            mAnimationExecuting[MoveBy2Points] = false;
            mAnimationFinished[MoveBy2Points] = true;
        }
    }
}

void Edge::move(const sf::Vector2f &positionAfterMove, const float speed) {
    mStartPosition = startPoint;
    mDeltaPosition = positionAfterMove - startPoint;
    mCurrentPosition = sf::Vector2f(std::sqrt(std::abs(mDeltaPosition.x)), std::sqrt(std::abs(mDeltaPosition.y)));
    mPositionJumpStep = sf::Vector2f(mCurrentPosition * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);
}



void Edge::moveBy2Points(const sf::Vector2f &position1AfterMove, const sf::Vector2f &position2AfterMove, const float speed) {
    mStartPosition1 = startPoint;
    mDeltaPosition1 = position1AfterMove - startPoint;
    mCurrentPosition1 = sf::Vector2f(std::sqrt(std::abs(mDeltaPosition1.x)), std::sqrt(std::abs(mDeltaPosition1.y)));
    mPositionJumpStep1 = sf::Vector2f(mCurrentPosition1 * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);

    mStartPosition2 = endPoint;
    mDeltaPosition2 = position2AfterMove - endPoint;
    mCurrentPosition2 = sf::Vector2f(std::sqrt(std::abs(mDeltaPosition2.x)), std::sqrt(std::abs(mDeltaPosition2.y)));
    mPositionJumpStep2 = sf::Vector2f(mCurrentPosition2 * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);

    mAnimationExecuting[MoveBy2Points] = true;
    mAnimationFinished[MoveBy2Points] = false;
}