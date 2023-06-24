#include <State/BST.hpp>

BST::BST(StateStack &stack, sf::RenderWindow &window) : State(stack, window) {
    buildScene();
}

BST::~BST() {
    clear(mRoot);
}

void BST::draw() {
    mWindow.draw(mSceneGraph);
}

void BST::update() {
    mSceneGraph.update();
}

void BST::handleEvent(sf::Event &event) {
    mSceneGraph.handleEvent(mWindow, event);
    if (mSceneLayers[Buttons]->getChildren()[Create]->isLeftClicked(mWindow, event)) {
        for (auto &child : mSceneLayers[CreateOptions]->getChildren()) {
            if (child->isActive()) {
                child->deactivate();
            } else {
                child->activate();
            }
        }
    }

    if (mSceneLayers[CreateOptions]->getChildren()[RamdomButton]->isLeftClicked(mWindow, event)) {
        mInputSize = mSceneLayers[CreateOptions]->getChildren()[SizeInputBox]->getIntArrayData()[0];
        if (mInputSize < 15) {
            NODE_DISTANCE_HORIZONTAL = 80.f * Constant::SCALE_X;
            NODE_DISTANCE_VERTICAL = 100.f * Constant::SCALE_Y;
            Size::NODE_RADIUS = 40.f * Constant::SCALE_X;
        }
        else if (mInputSize < 30) {
            NODE_DISTANCE_HORIZONTAL = 50.f * Constant::SCALE_X;
            NODE_DISTANCE_VERTICAL = 80.f * Constant::SCALE_Y;
            Size::NODE_RADIUS = 30.f * Constant::SCALE_X;
        }
        else if (mInputSize < 40) {
            NODE_DISTANCE_HORIZONTAL = 40.f * Constant::SCALE_X;
            NODE_DISTANCE_VERTICAL = 80.f * Constant::SCALE_Y;
            Size::NODE_RADIUS = 30.f * Constant::SCALE_X;
        }
        else {
            NODE_DISTANCE_HORIZONTAL = 30.f * Constant::SCALE_X;
            NODE_DISTANCE_VERTICAL = 80.f * Constant::SCALE_Y;
            Size::NODE_RADIUS = 25.f * Constant::SCALE_X;
        }
        createRandomTree();
    }
}

void BST::buildScene() {
    for (int i = 0; i < LayerCount; i++) {
        SceneNode::Ptr layer = std::make_unique<SceneNode>();
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
    std::unique_ptr<RectangleButton> createButton = std::make_unique<RectangleButton>();
    createButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 600), "Create", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(createButton));

    std::unique_ptr<RectangleButton> insertButton = std::make_unique<RectangleButton>();
    insertButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 520), "Insert", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(insertButton));

    std::unique_ptr<RectangleButton> deleteButton = std::make_unique<RectangleButton>();
    deleteButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 440), "Delete", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(deleteButton));

    std::unique_ptr<RectangleButton> updateButton = std::make_unique<RectangleButton>();
    updateButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 360), "Update", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(updateButton));

    std::unique_ptr<RectangleButton> searchButton = std::make_unique<RectangleButton>();
    searchButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 280), "Search", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(searchButton));   

    std::unique_ptr<RectangleButton> sizeButton = std::make_unique<RectangleButton>();
    sizeButton->set(
        sf::Vector2f(60 * Constant::SCALE_X, 80 * Constant::SCALE_Y), sf::Vector2f(80 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 600), "N =",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], sf::Color::Transparent, sf::Color::Black,
        sf::Color::Transparent, sf::Color::Black
    );
    sizeButton->deactivate();
    mSceneLayers[CreateOptions]->attachChild(std::move(sizeButton)); 

    std::unique_ptr<InputBox> sizeInput = std::make_unique<InputBox>();
    sizeInput->set(sf::Vector2f(150 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 585), sf::Vector2f(100 * Constant::SCALE_X, 50 * Constant::SCALE_Y));
    sizeInput->deactivate();
    sizeInput->setContent(std::to_string(mInputSize));
    mSceneLayers[CreateOptions]->attachChild(std::move(sizeInput));

    std::unique_ptr<RectangleButton> randomButton = std::make_unique<RectangleButton>();
    randomButton->set(
        sf::Vector2f(150 * Constant::SCALE_X, 60 * Constant::SCALE_Y), sf::Vector2f(280 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 590), "Random",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    randomButton->deactivate();
    mSceneLayers[CreateOptions]->attachChild(std::move(randomButton));

    std::unique_ptr<RectangleButton> fileButton = std::make_unique<RectangleButton>();
    fileButton->set(
        sf::Vector2f(200 * Constant::SCALE_X, 60 * Constant::SCALE_Y), sf::Vector2f(450 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 590), "Init from file",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    fileButton->deactivate();
    mSceneLayers[CreateOptions]->attachChild(std::move(fileButton));


    //---------------
    createRandomTree();
}



