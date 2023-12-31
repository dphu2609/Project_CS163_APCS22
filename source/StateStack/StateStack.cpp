#include <State/StateStack.hpp>

StateStack::StateStack(sf::RenderWindow &window) : mWindow(window) {}

template <typename T>
void StateStack::registerState(States::ID stateID) {
    mFactories[stateID] = [this]() {
        return State::Ptr(new T(*this, mWindow));
    };
}

template void StateStack::registerState<AVL>(States::ID stateID);
template void StateStack::registerState<Tree234>(States::ID stateID);
template void StateStack::registerState<Heap>(States::ID stateID);
template void StateStack::registerState<Trie>(States::ID stateID);
template void StateStack::registerState<Graph>(States::ID stateID);
template void StateStack::registerState<HashTable>(States::ID stateID);
template void StateStack::registerState<MainMenu>(States::ID stateID);

State::Ptr StateStack::createState(States::ID stateID) {
    auto found = mFactories.find(stateID);
    assert(found != mFactories.end());
    return found->second();
}

void StateStack::handleEvent(sf::Event& event) {
    if(!mStack.empty()) mStack.back()->handleEvent(event);
    applyPendingChanges();
}

void StateStack::update() {
    if (!mStack.empty()) mStack.back()->update();
    applyPendingChanges();
}

void StateStack::draw() {
    if (!mStack.empty()) mStack.back()->draw();
}

void StateStack::pushState(States::ID stateID) {
    mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState() {
    mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates() {
    mPendingList.push_back(PendingChange(Clear));
}

StateStack::PendingChange::PendingChange(StateStack::Action action, States::ID stateID) : action(action), stateID(stateID) {}

void StateStack::applyPendingChanges() {
    for (auto &change : mPendingList) {
        switch (change.action) {
            case Push:
                mStack.push_back(createState(change.stateID));
                break;
            case Pop:
                mStack.pop_back();
                break;
            case Clear:
                mStack.clear();
                break;
        }
    }
    mPendingList.clear();
}

bool StateStack::isEmpty() const {
    return mStack.empty();
}
