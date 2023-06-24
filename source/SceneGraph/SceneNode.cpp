#include <SceneGraph/SceneNode.hpp>

SceneNode::SceneNode() {
    this->mParent = nullptr;
    for (int i = 0; i < AnimationCount; i++) {
        mAnimationExecuting[i] = false;
        mAnimationFinished[i] = false;
    }
}

void SceneNode::attachChild(Ptr child) {
    child->mParent = this;
    mChildren.emplace_back(std::move(child));
}

void SceneNode::clearChildren() {
    mChildren.clear();
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    if (mIsActive) drawCurrent(target, states);
    for (const auto& child : mChildren) {
        if (child) 
            child->draw(target, states);
    }
}

void SceneNode::update() {
    if (mIsActive) updateCurrent();
    for (const auto& child : mChildren) child->update();
}

void SceneNode::handleEvent(sf::RenderWindow &window, sf::Event &event) {
    if (mIsActive) handleCurrentEvent(window, event);
    for (const auto& child : mChildren) child->handleEvent(window, event);
}

void SceneNode::resetAnimationVar() {
    for (int i = 0; i < AnimationCount; i++) {
        mAnimationExecuting[i] = false;
        mAnimationFinished[i] = false;
    }
    for (const auto& child : mChildren) child->resetAnimationVar();
}

void SceneNode::activate() {
    mIsActive = true;
}

void SceneNode::deactivate() {
    mIsActive = false;
}

std::vector<std::unique_ptr<SceneNode>>& SceneNode::getChildren() {
    return mChildren;
}

int SceneNode::getChildrenCount() {
    return mChildren.size();
}

bool SceneNode::isZoomFinished() {
    if (!mAnimationExecuting[Zoom] && mAnimationFinished[Zoom]) return true;
    return false;
}

bool SceneNode::isMoveFinished() {
    if (!mAnimationExecuting[Move] && mAnimationFinished[Move]) return true;
    return false;
}

bool SceneNode::isMoveBy2PointsFinished() {
    if (!mAnimationExecuting[MoveBy2Points] && mAnimationFinished[MoveBy2Points]) return true;
    return false;
}

bool SceneNode::isChange1ColorFinished() {
    if (!mAnimationExecuting[Change1Color] && mAnimationFinished[Change1Color]) return true;
    return false;
}

bool SceneNode::isChange2ColorFinished() {
    if (!mAnimationExecuting[Change2Color] && mAnimationFinished[Change2Color]) return true;
    return false;
}

bool SceneNode::isChange3ColorFinished() {
    if (!mAnimationExecuting[Change3Color] && mAnimationFinished[Change3Color]) return true;
    return false;
}

void SceneNode::zoom(const sf::Vector2f &sizeAfterZoom, const float speed) {
    if (!mAnimationExecuting[Zoom] && !mAnimationFinished[Zoom]) {
        setVarForZoom(sizeAfterZoom, speed);
    }
}

void SceneNode::move(const sf::Vector2f &positionAfterMove, const float speed) {
    if (!mAnimationExecuting[Move] && !mAnimationFinished[Move]) {
        setVarForZoom(positionAfterMove, speed);
    }
}

void SceneNode::moveBy2Points(const sf::Vector2f &position1AfterMove, const sf::Vector2f &position2AfterMove, const float speed) {
    if (!mAnimationExecuting[MoveBy2Points] && !mAnimationFinished[MoveBy2Points]) {
        setVarForMoveBy2Points(position1AfterMove, position2AfterMove, speed);
    }
}    

void SceneNode::change1Color(const sf::Color &objectColorAfterChange, const float speed) {
    if (!mAnimationExecuting[Change1Color] && !mAnimationFinished[Change1Color]) {
        setVarForChange1Color(objectColorAfterChange, speed);
    }
}

void SceneNode::change2Color(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const float speed) {
    if (!mAnimationExecuting[Change2Color] && !mAnimationFinished[Change2Color]) {
        setVarForChange2Color(objectColorAfterChange, textColorAfterChange, speed);
    }
}

void SceneNode::change3Color(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const sf::Color &objectOutlineColorAfterChange, const float speed) {
    if (!mAnimationExecuting[Change3Color] && !mAnimationFinished[Change3Color]) {
        setVarForChange3Color(objectColorAfterChange, textColorAfterChange, objectOutlineColorAfterChange, speed);
    }
}