#ifndef CODEBLOCK_HPP
#define CODEBLOCK_HPP

#include <SceneGraph/SceneNode.hpp>
#include <GlobalVar.hpp>

class CodeBlock : public SceneNode {
private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent();
private:
    std::vector<sf::Text> mCodeContent;
    std::vector<sf::Text> mCodeContentActivated;
    std::vector<sf::RectangleShape> mBox;
    std::vector<sf::RectangleShape> mBoxActivated;
    std::vector<bool> mLineActivated = {};
public:
    void set(
        const std::vector<std::string> &codeContent = {}, double textSize = 23.f * Constant::SCALE_X,
        sf::Color boxColor = Color::CODEBLOCK_BOX_COLOR, sf::Color textColor = Color::CODEBLOCK_TEXT_COLOR, 
        sf::Color boxColorActivated = Color::CODEBLOCK_BOX_ACTIVATED_COLOR, sf::Color textColorActivated = Color::CODEBLOCK_TEXT_ACTIVATED_COLOR
    );
    virtual void setMultipleContent(const std::vector<std::string> &codeContent);
    virtual void activateLine(std::vector<int> index);
    virtual void resetLine();
};

#endif