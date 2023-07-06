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
    int getChildrenCount();
    void clearChildren();

    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void handleEvent(sf::RenderWindow &window, sf::Event &event);
private: 
    SceneNode* mParent;
    std::vector<Ptr> mChildren;
private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {}
    virtual void updateCurrent() {}
    virtual void handleCurrentEvent(sf::RenderWindow &window, sf::Event &event) {}
public:
    virtual void setPosition(const sf::Vector2f &position) {}
    virtual void setPositionByPoints(const sf::Vector2f &position1, const sf::Vector2f &position2) {}
    virtual sf::Vector2f getPosition() {return sf::Vector2f(0, 0);}
public:
    virtual bool isLeftClicked(sf::RenderWindow &window, sf::Event &event) {return false;}
    virtual bool isRightClicked(sf::RenderWindow &window, sf::Event &event) {return false;}
    virtual bool isHovered(sf::RenderWindow &window, sf::Event &event) {return false;}
    virtual bool isHolded(sf::RenderWindow &window, sf::Event &event) {return false;}
public:
    virtual std::vector<int> getIntArrayData() {return {};} 
    virtual void setContent(const std::string &content) {}
    virtual void setLabel(const std::string &label) {}
    virtual std::string getContent() {return "";}
private:
    bool mIsActive = true;
public:
    void activate();
    void deactivate();
    bool isActive() {return mIsActive;}
//animations
public:
    enum AnimationType {
        Zoom, 
        Move,
        MoveBy2Points,
        Rotate,
        Change1Color,
        Change2Color,
        Change3Color,
        AnimationCount
    };
    std::map<int, bool> mAnimationFinished;
    std::map<int, bool> mAnimationExecuting;
public:
    bool isProcessing();
    bool isZoomFinished();
    bool isMoveFinished();
    bool isMoveBy2PointsFinished();
    bool isChange1ColorFinished();
    bool isChange2ColorFinished();
    bool isChange3ColorFinished();

    void resetAnimationVar();

    void zoom(const sf::Vector2f &sizeAfterZoom, const float speed = 1.f);
    void move(const sf::Vector2f &positionAfterMove, const float speed = 1.f);
    void moveBy2Points(const sf::Vector2f &position1AfterMove, const sf::Vector2f &position2AfterMove, const float speed = 1.f);    
    void change1Color(const sf::Color &objectColorAfterChange, const float speed = 1.f);
    void change2Color(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const float speed = 1.f);
    void change3Color(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const sf::Color &objectOutlineColorAfterChange, const float speed = 1.f);
private:
    virtual void setVarForZoom(const sf::Vector2f &sizeAfterZoom, const float speed) {}
    virtual void setVarForMove(const sf::Vector2f &positionAfterMove, const float speed) {}
    virtual void setVarForMoveBy2Points(const sf::Vector2f &position1AfterMove, const sf::Vector2f &position2AfterMove, const float speed) {}
    virtual void setVarForChange1Color(const sf::Color &objectColorAfterChange, const float speed) {}
    virtual void setVarForChange2Color(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const float speed) {}
    virtual void setVarForChange3Color(const sf::Color &objectColorAfterChange, const sf::Color &textColorAfterChange, const sf::Color &objectOutlineColorAfterChange, const float speed) {}
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

    struct RGBA {
        float r;
        float g;
        float b;
        float a;

        RGBA(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f)
        : r(r), g(g), b(b), a(a) {}
        RGBA& operator = (const sf::Color &color) {
            r = color.r;
            g = color.g;
            b = color.b;
            a = color.a;
            return *this;
        }
        RGBA& operator = (const RGBA &color) {
            r = color.r;
            g = color.g;
            b = color.b;
            a = color.a;
            return *this;
        }

        RGBA operator-(const RGBA& other) const {
            return RGBA(r - other.r, g - other.g, b - other.b, a - other.a);
        }
    };

    RGBA mStartObjectColor;
    RGBA mCurrentObjectColor;
    RGBA mDeltaObjectColor;
    RGBA mObjectColorJumpStep;

    RGBA mStartTextColor;
    RGBA mCurrentTextColor;
    RGBA mDeltaTextColor;
    RGBA mTextColorJumpStep;

    RGBA mStartOutlineColor;
    RGBA mCurrentOutlineColor;
    RGBA mDeltaOutlineColor;
    RGBA mOutlineColorJumpStep;
};

#endif