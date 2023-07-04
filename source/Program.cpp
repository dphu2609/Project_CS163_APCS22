#include <Program.hpp>

Program::Program() : 
    mWindow(sf::VideoMode(
        sf::VideoMode::getDesktopMode().width - 35, sf::VideoMode::getDesktopMode().height - 70), 
        "Data Visual", sf::Style::Default 
    ) , mStateStack(mWindow)
{
    mWindow.setPosition(sf::Vector2i(0, 0));
    sf::Image icon;
    icon.loadFromFile("resources/img/icon.png");
    mWindow.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    mWindow.setFramerateLimit(144);
    mWindow.setVerticalSyncEnabled(true);
    loadFonts();
    loadTextures();
}

void Program::registerStates() {
    mStateStack.registerState<AVL>(States::AVL);
}

void Program::loadFonts() {
    ResourcesHolder::fontsHolder.load(Fonts::FiraSansRegular, "resources/fonts/FiraSans-Regular.ttf");
    ResourcesHolder::fontsHolder.load(Fonts::RobotoRegular, "resources/fonts/Roboto/Roboto-Regular.ttf");
}

void Program::loadTextures() {
    ResourcesHolder::texturesHolder.load(Textures::PlayButton, "resources/img/playButtonBlack.png");
    ResourcesHolder::texturesHolder.load(Textures::PlayButtonHovered, "resources/img/playButtonHovered.png");
    ResourcesHolder::texturesHolder.load(Textures::PauseButton, "resources/img/pauseButtonBlack.png");
    ResourcesHolder::texturesHolder.load(Textures::PauseButtonHovered, "resources/img/pauseButtonHovered.png");
    ResourcesHolder::texturesHolder.load(Textures::NextButton, "resources/img/nextButtonBlack.png");
    ResourcesHolder::texturesHolder.load(Textures::NextButtonHovered, "resources/img/nextButtonHovered.png");
    ResourcesHolder::texturesHolder.load(Textures::PrevButton, "resources/img/previousButtonBlack.png");
    ResourcesHolder::texturesHolder.load(Textures::PrevButtonHovered, "resources/img/prevButtonHovered.png");
    ResourcesHolder::texturesHolder.load(Textures::ReplayButton, "resources/img/replayButtonBlack.png");
    ResourcesHolder::texturesHolder.load(Textures::ReplayButtonHovered, "resources/img/replayButtonHovered.png");
}

void Program::run() {
    registerStates();
    mStateStack.pushState(States::AVL);
    sf::Clock clock;    
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(mWindow.isOpen()) {
        processEvents();
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while(timeSinceLastUpdate > Constant::TIME_PER_FRAME) {
            timeSinceLastUpdate -= Constant::TIME_PER_FRAME;
            mStateStack.update();
        }
        mWindow.clear(Color::BACKGROUND_COLOR);
        mStateStack.draw();
        mWindow.display();
    }
}

void Program::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        mStateStack.handleEvent(event);
        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}
