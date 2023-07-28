#include <SceneGraph/InputBox.hpp>

void InputBox::set(
    const sf::Vector2f &position, const sf::Vector2f &size, const float &outlineThickness, const sf::Font &font,
    const sf::Color &boxColor, const sf::Color &textColor, const sf::Color &outlineColor, const sf::Color &cursorColor
) {
    mContent.setString("");
    mContent.setFont(font);
    mContent.setCharacterSize(size.y * 0.7);
    mContent.setPosition(position + sf::Vector2f(5 * Constant::SCALE_X, size.y * 0.05));
    mContent.setFillColor(textColor);

    mBox.setSize(size);
    mBox.setPosition(position);
    mBox.setFillColor(boxColor);
    mBox.setOutlineThickness(outlineThickness);
    mBox.setOutlineColor(outlineColor);

    mCursor.setSize(sf::Vector2f(3 * Constant::SCALE_X, size.y * 0.9));
    mCursor.setPosition(mBox.getPosition() + sf::Vector2f(7 * Constant::SCALE_X + mContent.getLocalBounds().width, mBox.getSize().y*0.05));
    mCursor.setFillColor(cursorColor);
}

void InputBox::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mBox);
    target.draw(mContent);
    if (mIsActivated && mIsCursorVisible) {
        target.draw(mCursor);
    }
}

void InputBox::updateCurrent() {
    if (mIsActivated) {
        if (mElapsedTime >= 0.75) {
            if (mIsCursorVisible) mIsCursorVisible = false;
            else mIsCursorVisible = 1;
            mElapsedTime = 0;
        }
        mElapsedTime += mClock.restart().asSeconds();
    }

    if (mAnimationExecuting[Move]) {
        sf::Vector2f newPosition = mBox.getPosition();
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

        mBox.setPosition(newPosition);
        mContent.setPosition(newPosition + sf::Vector2f(5 * Constant::SCALE_X, mBox.getSize().y * 0.05));
        mCursor.setPosition(mBox.getPosition() + sf::Vector2f(7 * Constant::SCALE_X + mContent.getLocalBounds().width, mBox.getSize().y*0.05));

        if (mCurrentPosition.x > 0) mCurrentPosition.x -= mPositionJumpStep.x;
        if (mCurrentPosition.y > 0) mCurrentPosition.y -= mPositionJumpStep.y;

        if (mCurrentPosition.x <= 0 && mCurrentPosition.y <= 0) {
            mAnimationExecuting[Move] = false;
            mAnimationFinished[Move] = true;
        }
    }
}

void InputBox::handleCurrentEvent(sf::RenderWindow &window, sf::Event &event) {
    if (isLeftClicked(window, event)) {
        mIsActivated = true;
        mIsCursorVisible = true;
        mElapsedTime = 0;
        mClock.restart();
    } 
    else if (event.type == sf::Event::MouseButtonPressed) {
        mIsActivated = false;
    }
    if (mIsActivated) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128 && event.text.unicode != 8) {
                mData += static_cast<char>(event.text.unicode);
                if (mContent.getLocalBounds().width >= mBox.getSize().x - mContent.getCharacterSize()) {
                    mOverBoundChar++;
                    std::string str = "";
                    for (int i = mOverBoundChar; i < mData.size(); i++) {
                        str += mData[i];
                    }
                    mContent.setString(str);
                }
                else mContent.setString(mData);
                mCursor.setPosition(mBox.getPosition() + sf::Vector2f(7 * Constant::SCALE_X + mContent.getLocalBounds().width, mBox.getSize().y*0.05));
                mIsCursorVisible = true;
                mElapsedTime = 0;
            }
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Backspace && mContent.getString().getSize() > 0) {
                mData.pop_back();
                if (mContent.getLocalBounds().width >= mBox.getSize().x - mContent.getCharacterSize()*1.5) {
                    if (mOverBoundChar > 0) mOverBoundChar--;
                    std::string str = "";
                    for (int i = mOverBoundChar; i < mData.size(); i++) {
                        str += mData[i];
                    }
                    mContent.setString(str);
                }
                else mContent.setString(mData);
                mCursor.setPosition(mBox.getPosition() + sf::Vector2f(7 * Constant::SCALE_X + mContent.getLocalBounds().width, mBox.getSize().y*0.05));
                mIsCursorVisible = true;
                mElapsedTime = 0;
            }
        }
    }
}

bool InputBox::isHovered(sf::RenderWindow &window, sf::Event& event) {
    sf::Vector2i localPosition(sf::Mouse::getPosition(window));
    sf::Vector2f localPositionF(static_cast<float>(localPosition.x), static_cast<float>(localPosition.y));
    localPositionF = window.mapPixelToCoords(localPosition);
    sf::FloatRect dataBounds = mBox.getGlobalBounds();
    sf::Vector2f startPoint(dataBounds.left, dataBounds.top);
    sf::Vector2f endPoint(dataBounds.left + dataBounds.width, dataBounds.top + dataBounds.height);
    return (
        localPositionF.x >= dataBounds.left && 
        localPositionF.x <= dataBounds.left + dataBounds.width && 
        localPositionF.y >= dataBounds.top && 
        localPositionF.y <= dataBounds.top + dataBounds.height
    );
}

bool InputBox::isLeftClicked(sf::RenderWindow &window, sf::Event &event) {
    return (isHovered(window, event) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.type == sf::Event::MouseButtonPressed);
}

void InputBox::setVarForMove(const sf::Vector2f &positionAfterMove, const float speed) {
    mStartPosition = mBox.getPosition();
    mDeltaPosition = positionAfterMove - mStartPosition;
    mCurrentPosition = sf::Vector2f(std::sqrt(std::abs(mDeltaPosition.x)), std::sqrt(std::abs(mDeltaPosition.y)));
    mPositionJumpStep = sf::Vector2f(mCurrentPosition * Constant::TIME_PER_FRAME.asSeconds() * Animation::SPEED * speed);
    mAnimationExecuting[Move] = true;
    mAnimationFinished[Move] = false;
}

std::vector<int> InputBox::getIntArrayData() {
    if (mData.size() == 0) return {};
    std::vector<int> result = {};
    std::string temp = "";
    bool isValid = true;
    for (int i = 0; i < mData.size(); i++) {
        if ((mData[i] >= '0' && mData[i] <= '9') || (mData[i] == '-' && temp.size() == 0)) {
            temp += mData[i];
            if (i == mData.size() - 1 && !(temp.size() == 1 && temp[0] == '-') && isValid) {
                result.push_back(std::stoi(temp));
            }
        } else if (mData[i] == ',') {
            if (isValid && !(temp.size() == 1 && temp[0] == '-')) result.push_back(std::stoi(temp));
            temp = "";
            isValid = true;
        } else if (mData[i] != ' ' || (temp.size() == 1 && temp[0] == '-')) {
            isValid = false;
        }   
    }
    return result;
}

void InputBox::setContent(const std::string &content) {
    mData = content;
    mContent.setString(mData);
    mCursor.setPosition(mBox.getPosition() + sf::Vector2f(7 * Constant::SCALE_X + mContent.getLocalBounds().width, mBox.getSize().y*0.05));
}

std::string InputBox::getContent() {
    return mData;
}

bool InputBox::isContainOnlyOneNumber() {
    if (mData.size() == 0) return false;
    bool isContainOnlyOneNumber = true;
    for (int i = 0; i < mData.size(); i++) {
        if (mData[i] < '0' || mData[i] > '9') {
            isContainOnlyOneNumber = false;
            break;
        }
    }
    return isContainOnlyOneNumber;
}