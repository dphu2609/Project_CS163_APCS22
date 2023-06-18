#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <GlobalVar.hpp>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable {
public:
    typedef std::unique_ptr<SceneNode> Ptr;
    SceneNode();

    void attachChild(Ptr child);
    std::vector<std::unique_ptr<SceneNode>>& getChildren();

    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void handleEvent(sf::Event &event);
private: 
    SceneNode* mParent;
    std::vector<Ptr> mChildren;
private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {}
    virtual void updateCurrent() {}
    virtual void handleCurrentEvent(sf::Event &event) {}
public:
    virtual void setPosition(const sf::Vector2f &position) {}
    virtual void setPositionByPoints(const sf::Vector2f &position1, const sf::Vector2f &position2) {}
    // virtual sf::Vector2f getPosition() {}
//animations
public:
    enum AnimationType {
        Zoom, 
        Move,
        MoveBy2Points,
        Rotate,
        ChangeColor,
        AnimationCount
    };
    std::map<int, bool> mAnimationFinished;
    std::map<int, bool> mAnimationExecuting;
public:
    bool isFinished();
    bool isExecuting();
    void resetAnimation();
    virtual void zoom(const sf::Vector2f &sizeAfterZoom, const float speed = 1.f) {}
    virtual void move(const sf::Vector2f &positionAfterMove, const float speed = 1.f) {}
    virtual void moveBy2Points(const sf::Vector2f &position1AfterMove, const sf::Vector2f &position2AfterMove, const float speed = 1.f) {}
    virtual void rotate(const float &angleAfterRotate, const float speed = 1.f) {}
    virtual void changeColor(const sf::Color &objectColorAfterChange, const float speed = 1.f) {}
    virtual void changeColor(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const float speed = 1.f) {}
    virtual void changeColor(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const sf::Color &objectOutlineColorAfterChange, const float speed = 1.f) {}
public: //var for animations
    sf::Vector2f mStartSize;
    sf::Vector2f mCurrentSize;
    sf::Vector2f mDeltaSize;
    sf::Vector2f mSizeJumpStep;

    sf::Vector2f mStartPosition;
    sf::Vector2f mCurrentPosition;
    sf::Vector2f mDeltaPosition;
    sf::Vector2f mPositionJumpStep;

    sf::Vector2f mStartPosition1;
    sf::Vector2f mCurrentPosition1;
    sf::Vector2f mDeltaPosition1;
    sf::Vector2f mPositionJumpStep1;

    sf::Vector2f mStartPosition2;
    sf::Vector2f mCurrentPosition2;
    sf::Vector2f mDeltaPosition2;
    sf::Vector2f mPositionJumpStep2;

    float mAngleAfterRotate;
    float mDeltaAngle;
    float mAngleJumpStep;

    struct RGB {
        float red;
        float green;
        float blue;
    };

    sf::Color mObjectColorAfterChange;
    RGB mObjectJumpStep;
    sf::Color mTextColorAfterChange;
    RGB mTextJumpStep;
    sf::Color mObjectOutlineColorAfterChange;
    RGB mOutlineJumpStep;
};

#endif