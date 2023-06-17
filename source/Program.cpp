#include <Program.hpp>

Program::Program() : 
    mWindow(sf::VideoMode(
        sf::VideoMode::getDesktopMode().width - 35, sf::VideoMode::getDesktopMode().height - 70), 
        "Data Visual", sf::Style::Default 
    ) , mBST(mWindow)
{
    mWindow.setPosition(sf::Vector2i(0, 0));
    sf::Image icon;
    icon.loadFromFile("resources/img/icon.png");
    mWindow.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    mWindow.setFramerateLimit(144);
    mWindow.setVerticalSyncEnabled(true);
}

void Program::run() {
    sf::Clock clock;    
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::RectangleShape rect;
    rect.setFillColor(Color::NODE_COLOR);
    rect.setSize(sf::Vector2f(100, 100));
    rect.setPosition(sf::Vector2f(100, 100));
    while(mWindow.isOpen()) {
        processEvents();
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while(timeSinceLastUpdate > Constant::TIME_PER_FRAME) {
            timeSinceLastUpdate -= Constant::TIME_PER_FRAME;
            //update funciton here
        }
        mWindow.clear();
        mBST.draw();
        mWindow.display();
    }
}

void Program::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}
