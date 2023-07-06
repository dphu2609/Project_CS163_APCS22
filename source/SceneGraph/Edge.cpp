#include <SceneGraph/Edge.hpp>

void Edge::set(const sf::Vector2f &point1, const sf::Vector2f &point2, const float &edgeThickness, const sf::Color &edgeColor) {
    setPositionByPoints(point1, point2);
    mEdge.setFillColor(edgeColor);
}

void Edge::setPosition(const sf::Vector2f &position) {
    mEdge.setPosition(position);
}

void Edge::setPositionByPoints(const sf::Vector2f &point1, const sf::Vector2f &point2, const float &edgeThickness) {
    sf::Vector2f size(edgeThickness, std::sqrt(std::pow(point2.x - point1.x, 2) + std::pow(point2.y - point1.y, 2)) * Constant::SCALE_X);
    sf::Vector2f position = point1;
    double angle = std::atan2(point2.x - point1.x, point2.y - point1.y) * 180 / std::acos(-1);
    mEdge.setSize(size);
    mEdge.setPosition(position);
    mEdge.setRotation(-angle);
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

    if (mAnimationExecuting[Change1Color]) {
        sf::Color newColor = mEdge.getFillColor();
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
        mEdge.setFillColor(newColor);
        
        if (mCurrentObjectColor.r > 0) mCurrentObjectColor.r -= mObjectColorJumpStep.r;
        if (mCurrentObjectColor.g > 0) mCurrentObjectColor.g -= mObjectColorJumpStep.g;
        if (mCurrentObjectColor.b > 0) mCurrentObjectColor.b -= mObjectColorJumpStep.b;
        if (mCurrentObjectColor.a > 0) mCurrentObjectColor.a -= mObjectColorJumpStep.a;

        if (mCurrentObjectColor.r <= 0 && mCurrentObjectColor.g <= 0 && mCurrentObjectColor.b <= 0 && mCurrentObjectColor.a <= 0) {
            mAnimationExecuting[Change1Color] = false;
            mAnimationFinished[Change1Color] = true;
        }
    }
}

void Edge::setVarForMove(const sf::Vector2f &positionAfterMove, const float speed) {
    mStartPosition = startPoint;
    mDeltaPosition = positionAfterMove - startPoint;
    mCurrentPosition = sf::Vector2f(std::sqrt(std::abs(mDeltaPosition.x)), std::sqrt(std::abs(mDeltaPosition.y)));
    mPositionJumpStep = sf::Vector2f(mCurrentPosition * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);
}



void Edge::setVarForMoveBy2Points(const sf::Vector2f &position1AfterMove, const sf::Vector2f &position2AfterMove, const float speed) {
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

void Edge::setVarForChange1Color(const sf::Color &objectColorAfterChange, const float speed) {
    mStartObjectColor = mEdge.getFillColor();
    mDeltaObjectColor = RGBA(objectColorAfterChange.r, objectColorAfterChange.g, objectColorAfterChange.b, objectColorAfterChange.a) - RGBA(mStartObjectColor.r, mStartObjectColor.g, mStartObjectColor.b, mStartObjectColor.a);
    mCurrentObjectColor = RGBA(std::sqrt(std::abs(mDeltaObjectColor.r)), std::sqrt(std::abs(mDeltaObjectColor.g)), std::sqrt(std::abs(mDeltaObjectColor.b)), std::sqrt(std::abs(mDeltaObjectColor.a)));
    mObjectColorJumpStep = RGBA(mCurrentObjectColor.r * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentObjectColor.g * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentObjectColor.b * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed,
                                    mCurrentObjectColor.a * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);
    this->mAnimationExecuting[Change1Color] = true;
    this->mAnimationFinished[Change1Color] = false;
}