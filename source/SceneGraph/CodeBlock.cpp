#include <SceneGraph/CodeBlock.hpp>

void CodeBlock::set(
    const std::vector<std::string> &codeContent, double textSize,
    sf::Color boxColor, sf::Color textColor, sf::Color boxColorActivated, sf::Color textColorActivated
) {
    double maxWidth = 0;
    mCodeContent.clear();
    mCodeContentActivated.clear();
    mBox.clear();
    mBoxActivated.clear();
    mLineActivated.clear();
    mCodeContent.resize(codeContent.size());
    mCodeContentActivated.resize(codeContent.size());
    mBox.resize(codeContent.size());
    mBoxActivated.resize(codeContent.size());
    for (int i = 0; i < codeContent.size(); i++) {
        mCodeContent[i].setFont(ResourcesHolder::fontsHolder[Fonts::FiraMonoRegular]);
        mCodeContent[i].setString(codeContent[i]);
        mCodeContent[i].setCharacterSize(textSize);
        mCodeContent[i].setFillColor(textColor);

        mCodeContentActivated[i].setFont(ResourcesHolder::fontsHolder[Fonts::FiraMonoRegular]);
        mCodeContentActivated[i].setString(codeContent[i]);
        mCodeContentActivated[i].setCharacterSize(textSize);
        mCodeContentActivated[i].setFillColor(textColorActivated);
        maxWidth = std::max(maxWidth, (double)mCodeContent[i].getLocalBounds().width);

        mLineActivated.push_back(false);
    }

    sf::Vector2f initialPos(
        sf::VideoMode::getDesktopMode().width - maxWidth - 35 - 150 * Constant::SCALE_X, 
        sf::VideoMode::getDesktopMode().height - textSize/0.5*codeContent.size() - 70 - 50*Constant::SCALE_Y
    );

    for (int i = 0; i < codeContent.size(); i++) {
        mBox[i].setSize(sf::Vector2f(maxWidth + 100 * Constant::SCALE_X, textSize/0.5));
        mBox[i].setPosition(initialPos);
        mBox[i].setFillColor(boxColor);

        mBoxActivated[i].setSize(sf::Vector2f(maxWidth + 100 * Constant::SCALE_X, textSize/0.5));
        mBoxActivated[i].setPosition(initialPos);
        mBoxActivated[i].setFillColor(boxColorActivated);

        mCodeContent[i].setPosition(initialPos + sf::Vector2f(50 * Constant::SCALE_X, textSize/2));
        mCodeContentActivated[i].setPosition(initialPos + sf::Vector2f(50 * Constant::SCALE_X, textSize/2));

        initialPos += sf::Vector2f(0, textSize/0.5);
    }
}

void CodeBlock::setMultipleContent(const std::vector<std::string> &codeContent) {
    set(codeContent);
}

void CodeBlock::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    for (int i = 0; i < mBox.size(); i++) {
        if (mLineActivated[i]) {
            target.draw(mBoxActivated[i]);
            target.draw(mCodeContentActivated[i]);
        } else {
            target.draw(mBox[i]);
            target.draw(mCodeContent[i]);
        }
    }
}

void CodeBlock::updateCurrent() {}

void CodeBlock::activateLine(std::vector<int> index) {
    resetLine();
    for (auto &i : index) {
        mLineActivated[i] = 1;
    }
}

void CodeBlock::resetLine() {
    for (int i = 0; i < mLineActivated.size(); i++) {
        mLineActivated[i] = false;
    }
}