#include <Program.hpp>

Program::Program() : 
    mWindow(sf::VideoMode(
        sf::VideoMode::getDesktopMode().width - 35, sf::VideoMode::getDesktopMode().height - 70), 
        "DSVisual", sf::Style::Default 
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
    loadCode();
}

void Program::registerStates() {
    mStateStack.registerState<AVL>(States::AVL);
    mStateStack.registerState<Tree234>(States::Tree234);
    mStateStack.registerState<Heap>(States::Heap);
    mStateStack.registerState<Trie>(States::Trie);
    mStateStack.registerState<Graph>(States::Graph);
    mStateStack.registerState<HashTable>(States::HashTable);
    mStateStack.registerState<MainMenu>(States::MainMenu);
}

void Program::loadFonts() {
    ResourcesHolder::fontsHolder.load(Fonts::FiraSansRegular, "resources/fonts/FiraSans-Regular.ttf");
    ResourcesHolder::fontsHolder.load(Fonts::RobotoRegular, "resources/fonts/Roboto/Roboto-Regular.ttf");
    ResourcesHolder::fontsHolder.load(Fonts::RobotoBold, "resources/fonts/Roboto/Roboto-Bold.ttf");
    ResourcesHolder::fontsHolder.load(Fonts::FiraMonoRegular, "resources/fonts/Fira_Mono/FiraMono-Regular.ttf");
    ResourcesHolder::fontsHolder.load(Fonts::RussoOne, "resources/fonts/RussoOne-Regular.ttf");
}

void Program::loadTextures() {
    ResourcesHolder::texturesHolder.load(Textures::PlayButton, "resources/img/playButtonBlack.png");
    ResourcesHolder::texturesHolder.load(Textures::PlayButtonWhite, "resources/img/playButton.png");
    ResourcesHolder::texturesHolder.load(Textures::PlayButtonHovered, "resources/img/playButtonHovered.png");
    ResourcesHolder::texturesHolder.load(Textures::PauseButton, "resources/img/pauseButtonBlack.png");
    ResourcesHolder::texturesHolder.load(Textures::PauseButtonWhite, "resources/img/pauseButton.png");
    ResourcesHolder::texturesHolder.load(Textures::PauseButtonHovered, "resources/img/pauseButtonHovered.png");
    ResourcesHolder::texturesHolder.load(Textures::NextButton, "resources/img/nextButtonBlack.png");
    ResourcesHolder::texturesHolder.load(Textures::NextButtonWhite, "resources/img/nextButton.png");
    ResourcesHolder::texturesHolder.load(Textures::NextButtonHovered, "resources/img/nextButtonHovered.png");
    ResourcesHolder::texturesHolder.load(Textures::PrevButton, "resources/img/previousButtonBlack.png");
    ResourcesHolder::texturesHolder.load(Textures::PrevButtonWhite, "resources/img/prevButton.png");
    ResourcesHolder::texturesHolder.load(Textures::PrevButtonHovered, "resources/img/prevButtonHovered.png");
    ResourcesHolder::texturesHolder.load(Textures::ReplayButton, "resources/img/replayButtonBlack.png");
    ResourcesHolder::texturesHolder.load(Textures::ReplayButtonWhite, "resources/img/replayButton.png");
    ResourcesHolder::texturesHolder.load(Textures::ReplayButtonHovered, "resources/img/replayButtonHovered.png");
    ResourcesHolder::texturesHolder.load(Textures::ReturnButton, "resources/img/returnButtonBlack.png");
    ResourcesHolder::texturesHolder.load(Textures::ReturnButtonWhite, "resources/img/returnButton.png");
    ResourcesHolder::texturesHolder.load(Textures::ReturnButtonHovered, "resources/img/returnButtonHovered.png");
}

void Program::loadCode() {
    CodeContainer::codeHolder.load();
}

void Program::run() {
    registerStates();
    mStateStack.pushState(States::MainMenu);
    sf::Clock clock;    
    sf::Time timeSinceLastUpdate = sf::Time::Zero;  
    while(mWindow.isOpen()) {
        std::cout << "FPS: " << 1.f / clock.getElapsedTime().asSeconds() << std::endl;
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
