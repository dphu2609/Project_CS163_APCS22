#include <State/MainMenu.hpp>

MainMenu::MainMenu(StateStack &stack, sf::RenderWindow &window) : State(stack, window) {
    buildScene();
}

void MainMenu::draw() {
    mWindow.draw(mSceneGraph);
}

void MainMenu::update() {
    mSceneGraph.update();
}

void MainMenu::handleEvent(sf::Event &event) {
    mSceneGraph.handleEvent(mWindow, event);
    if (mSceneLayers[DataStructureButtons]->getChildren()[AVL]->isLeftClicked(mWindow, event)) {
        requestStackPush(States::AVL);
    }

    if (mSceneLayers[DataStructureButtons]->getChildren()[Tree234]->isLeftClicked(mWindow, event)) {
        requestStackPush(States::Tree234);
    }

    if (mSceneLayers[DataStructureButtons]->getChildren()[Trie]->isLeftClicked(mWindow, event)) {
        requestStackPush(States::Trie);
    }

    if (mSceneLayers[DataStructureButtons]->getChildren()[HashTable]->isLeftClicked(mWindow, event)) {
        requestStackPush(States::HashTable);
    }

    if (mSceneLayers[DataStructureButtons]->getChildren()[Graph]->isLeftClicked(mWindow, event)) {
        requestStackPush(States::Graph);
    }

    if (mSceneLayers[DataStructureButtons]->getChildren()[Heap]->isLeftClicked(mWindow, event)) {
        requestStackPush(States::Heap);
    }
}

void MainMenu::buildScene() {
    for (int i = 0; i < LayerCount; i++) {
        SceneNode::Ptr layer = std::make_unique<SceneNode>();
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    sf::Vector2f startPos = sf::Vector2f(500 * Constant::SCALE_X, Constant::WINDOW_HEIGHT / 2 - Size::DATA_STRUCTURE_BUTTON.y);
    float horizontalDistance = Size::DATA_STRUCTURE_BUTTON.x + (Constant::WINDOW_WIDTH - (1000 * Constant::SCALE_X) - 3 * Size::DATA_STRUCTURE_BUTTON.x) / 2;
    float verticalDistance = Size::DATA_STRUCTURE_BUTTON.y * 1.5;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            sf::Vector2f pos = startPos + sf::Vector2f(j * horizontalDistance, 0) + sf::Vector2f(0, i * verticalDistance);
            std::unique_ptr<RectangleButton> DSbutton = std::make_unique<RectangleButton>();
            DSbutton->set(
                Size::DATA_STRUCTURE_BUTTON,
                pos, "", ResourcesHolder::fontsHolder[Fonts::RobotoRegular],
                sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent, 
                Size::DATA_STRUCTURE_BUTTON_EDGE_THICKNESS, 
                Color::DATA_STRUCTURE_BUTTON_OUTLINE_COLOR, Color::DATA_STRUCTURE_BUTTON_OUTLINE_HOVERED_COLOR
            );
            mSceneLayers[DataStructureButtons]->attachChild(std::move(DSbutton));
        }
    }

    std::vector<sf::Vector2f> avlPos;
    avlPos.push_back(startPos + sf::Vector2f(Size::DATA_STRUCTURE_BUTTON.x / 2 - NODE_RADIUS, NODE_RADIUS));
    avlPos.push_back(avlPos[0] + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL * 2, NODE_DISTANCE_VERTICAL));
    avlPos.push_back(avlPos[0] + sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL));
    avlPos.push_back(avlPos[1] + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL));
    avlPos.push_back(avlPos[1] + sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL));
}



