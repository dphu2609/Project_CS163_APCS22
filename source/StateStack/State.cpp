#include <State/StateStack.hpp>

State::State(StateStack& stack, sf::RenderWindow &window) : mStack(&stack), mWindow(window) {}

void State::requestStackPush(States::ID stateID) {
    mStack->pushState(stateID);
}

void State::requestStackPop() {
    mStack->popState();
}

void State::requestStateClear() {
    mStack->clearStates();
}