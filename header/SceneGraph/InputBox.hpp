#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP

#include <SFML/Graphics.hpp>
#include <GlobalVar.hpp>
#include <SceneGraph/SceneNode.hpp>
#include <ResourceHolder.hpp>

class InputBox : public SceneNode {
private:
    sf::RectangleShape mBox;
    sf::Text mContent;
    sf::RectangleShape mCursor;
    std::string mData = "";
private:
    bool mIsActivated = false;
    bool mIsCursorVisible = false;
    sf::Clock mClock;
    float mElapsedTime;
    int mOverBoundChar = 0;
public: 
    void set(
        const sf::Vector2f &position, const sf::Vector2f &size = sf::Vector2f(300 * Constant::SCALE_X, 50 * Constant::SCALE_Y), 
        const float &outlineThickness = 2.f * Constant::SCALE_X,
        const sf::Font &font = ResourcesHolder::fontsHolder[Fonts::FiraSansRegular],
        const sf::Color &boxColor = Color::INPUTBOX_COLOR, const sf::Color &textColor = Color::INPUTBOX_TEXT_COLOR,
        const sf::Color &outlineColor = Color::INPUTBOX_OUTLINE_COLOR,
        const sf::Color &cursorColor = Color::INPUTBOX_CURSOR_COLOR
    );
public: 
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent();
    virtual void handleCurrentEvent(sf::RenderWindow &window, sf::Event &event);  
public:
    virtual bool isHovered(sf::RenderWindow &window, sf::Event &event);
    virtual bool isLeftClicked(sf::RenderWindow &window, sf::Event &event);
public:
    virtual std::vector<int> getIntArrayData();
    virtual void setContent(const std::string &content);
    virtual std::string getContent();
    virtual bool isContainOnlyOneNumber();
private: 
    virtual void setVarForMove(const sf::Vector2f &positionAfterMove, const float speed);
};

#endif