#include <State/State.hpp>

State::State(sf::RenderWindow &window) : mWindow(window) {}

void State::draw() {
    mWindow.draw(mSceneGraph);
}

void State::update() {
    mSceneGraph.update();
} 

void State::handleEvent(sf::Event &event) {
    mSceneGraph.handleEvent(event);
}