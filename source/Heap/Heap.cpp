#include <State/Heap.hpp>

#include <State/Heap.hpp>

Heap::Heap(StateStack &stack, sf::RenderWindow &window) : State(stack, window) {
    buildScene();
}

Heap::~Heap() {
    clear(mRoot);
    clear(mRootForBackup);
    while (!mTreeForBackward.empty()) {
        clear(mTreeForBackward.top()->root);
        delete mTreeForBackward.top();
        mTreeForBackward.pop();
    }
}

void Heap::draw() {
    mWindow.draw(mSceneGraph);
}

void Heap::update() {
    mSceneGraph.update();
    if (mInsertAnimation && !mIsReversed) insertAnimation();
    if (mDeleteAnimation && !mIsReversed) deleteAnimation();
    // if (mUpdateAnimation && !mIsReversed) updateAnimation();
    if (mSearchAnimation && !mIsReversed) searchAnimation();
    mIsAnimationPaused = mIsStepByStepMode;
}

void Heap::handleEvent(sf::Event &event) {
    mSceneGraph.handleEvent(mWindow, event);

    if (mSceneLayers[Buttons]->getChildren()[ToggleMaxHeap]->isLeftClicked(mWindow, event)) {
        if (mIsMaxHeap) {
            mIsMaxHeap = false;
            createRandomTree();
            mSceneLayers[Buttons]->getChildren()[ToggleMaxHeap]->setContent("MIN HEAP");
        }
        else {
            mIsMaxHeap = true;
            createRandomTree();
            mSceneLayers[Buttons]->getChildren()[ToggleMaxHeap]->setContent("MAX HEAP");
        }
    }

    if (mSceneLayers[Buttons]->getChildren()[Create]->isLeftClicked(mWindow, event)) {
        for (auto &child : mSceneLayers[CreateOptions]->getChildren()) {
            if (child->isActive()) {
                child->deactivate();
            } else {
                child->activate();
            }
        }
    }

    if (mSceneLayers[Buttons]->getChildren()[Insert]->isLeftClicked(mWindow, event)) {
        for (auto &child : mSceneLayers[InsertOptions]->getChildren()) {
            if (child->isActive()) {
                child->deactivate();
            } else {
                child->activate();
            }
        }
    }

    if (mSceneLayers[Buttons]->getChildren()[Delete]->isLeftClicked(mWindow, event)) {
        for (auto &child : mSceneLayers[DeleteOptions]->getChildren()) {
            if (child->isActive()) {
                child->deactivate();
            } else {
                child->activate();
            }
        }
    }

    if (mSceneLayers[Buttons]->getChildren()[Search]->isLeftClicked(mWindow, event)) {
        for (auto &child : mSceneLayers[SearchOptions]->getChildren()) {
            if (child->isActive()) {
                child->deactivate();
            } else {
                child->activate();
            }
        }
    }


    if (mSceneLayers[CreateOptions]->getChildren()[RamdomButton]->isLeftClicked(mWindow, event)) {
        mInputSize = mSceneLayers[CreateOptions]->getChildren()[SizeInputBox]->getIntArrayData()[0];
        createRandomTree();
    }

    if (mSceneLayers[InsertOptions]->getChildren()[InsertStart]->isLeftClicked(mWindow, event)) {
        std::vector<int> inputList = mSceneLayers[InsertOptions]->getChildren()[InsertInput]->getIntArrayData();
        while (!mInputQueue.empty()) mInputQueue.pop();
        for (auto &input : inputList) {
            mInputQueue.push(input);
        }
        mInsertAnimation = true;
        mDeleteAnimation = false;
        mUpdateAnimation = false;
        mSearchAnimation = false;
        mSceneLayers[ControlBox]->getChildren()[Play]->deactivate();
        mSceneLayers[ControlBox]->getChildren()[Pause]->activate();
        mSceneLayers[ControlBox]->getChildren()[Replay]->deactivate();
        mIsAnimationPaused = false;
        mIsStepByStepMode = false;
        mIsReplay = false;
        mAnimationStep = 1;
    }

    if (mSceneLayers[DeleteOptions]->getChildren()[DeleteStart]->isLeftClicked(mWindow, event)) {
        std::vector<int> inputList = mSceneLayers[DeleteOptions]->getChildren()[DeleteInput]->getIntArrayData();
        while (!mInputQueue.empty()) mInputQueue.pop();
        for (auto &input : inputList) {
            mInputQueue.push(input);
        }
        mInsertAnimation = false;
        mDeleteAnimation = true;
        mUpdateAnimation = false;
        mSearchAnimation = false;
        mSceneLayers[ControlBox]->getChildren()[Play]->deactivate();
        mSceneLayers[ControlBox]->getChildren()[Pause]->activate();
        mSceneLayers[ControlBox]->getChildren()[Replay]->deactivate();
        mIsAnimationPaused = false;
        mIsStepByStepMode = false;
        mIsReplay = false;
        mAnimationStep = 1;
    }

    if (mSceneLayers[SearchOptions]->getChildren()[SearchStart]->isLeftClicked(mWindow, event)) {
        std::vector<int> inputList = mSceneLayers[SearchOptions]->getChildren()[SearchInput]->getIntArrayData();
        while (!mInputQueue.empty()) mInputQueue.pop();
        for (auto &input : inputList) {
            mInputQueue.push(input);
        }
        mInsertAnimation = false;
        mDeleteAnimation = false;
        mUpdateAnimation = false;
        mSearchAnimation = true;
        mSceneLayers[ControlBox]->getChildren()[Play]->deactivate();
        mSceneLayers[ControlBox]->getChildren()[Pause]->activate();
        mIsAnimationPaused = false;
        mIsStepByStepMode = false;
        mIsReplay = false;
        mAnimationStep = 1;
    }

    //ControlBox
    if (mSceneLayers[ControlBox]->getChildren()[Play]->isActive()) {
        if (mSceneLayers[ControlBox]->getChildren()[Play]->isLeftClicked(mWindow, event)) {
            mSceneLayers[ControlBox]->getChildren()[Play]->deactivate();
            mSceneLayers[ControlBox]->getChildren()[Pause]->activate();
            mIsAnimationPaused = false;
            mIsStepByStepMode = false;
            if (mIsPendingReversed && !isProcessingAnimation()) {
                mIsReversed = false;
                mIsPendingReversed = false;
                resetNodeState();
            }
            if (mIsReversed) {
                if (!isProcessingAnimation()) {
                    mIsReversed = false;
                    resetNodeState();
                }
                else mIsPendingReversed = true;
            }
        }
    }
    else if (mSceneLayers[ControlBox]->getChildren()[Pause]->isActive()) {
        if (mSceneLayers[ControlBox]->getChildren()[Pause]->isLeftClicked(mWindow, event)) {
            mSceneLayers[ControlBox]->getChildren()[Pause]->deactivate();
            mSceneLayers[ControlBox]->getChildren()[Play]->activate();
            mIsAnimationPaused = true;
            mIsStepByStepMode = true;
        }
    }

    if (mSceneLayers[ControlBox]->getChildren()[Next]->isLeftClicked(mWindow, event)) {
        if (mSceneLayers[ControlBox]->getChildren()[Pause]->isActive()) {
            mSceneLayers[ControlBox]->getChildren()[Pause]->deactivate();
            mSceneLayers[ControlBox]->getChildren()[Play]->activate();
        }
        if (mIsPendingReversed && !isProcessingAnimation()) {
            mIsReversed = false;
            mIsPendingReversed = false;
            resetNodeState();
        }
        if (mIsReversed) {
            if (!isProcessingAnimation()) {
                mIsReversed = false;
                resetNodeState();
            }
            else mIsPendingReversed = true;
        }
        mIsAnimationPaused = false;
        mIsStepByStepMode = true;
    }

    if (mSceneLayers[ControlBox]->getChildren()[Previous]->isLeftClicked(mWindow, event)) {
        if (mSceneLayers[ControlBox]->getChildren()[Pause]->isActive()) {
            mSceneLayers[ControlBox]->getChildren()[Pause]->deactivate();
            mSceneLayers[ControlBox]->getChildren()[Play]->activate();
        }
        if (mSceneLayers[ControlBox]->getChildren()[Replay]->isActive()) {
            mIsReplay = false;
            mSceneLayers[ControlBox]->getChildren()[Replay]->deactivate();
            mSceneLayers[ControlBox]->getChildren()[Play]->activate();
        }
        mIsAnimationPaused = true;
        mIsStepByStepMode = true;
        mIsReversed = true;
        returnToPreviousStep();
    }

    if (!mSceneLayers[ControlBox]->getChildren()[Replay]->isActive() && mIsReplay) {
        mSceneLayers[ControlBox]->getChildren()[Replay]->activate();
        mSceneLayers[ControlBox]->getChildren()[Pause]->deactivate();
        mSceneLayers[ControlBox]->getChildren()[Play]->deactivate();
    } 

    if (mSceneLayers[ControlBox]->getChildren()[Replay]->isActive() && mSceneLayers[ControlBox]->getChildren()[Replay]->isLeftClicked(mWindow, event)) {
        resetNodeState();
        mAnimationStep = 1;
        restoreTree();
        mIsReplay = false;
        mSceneLayers[ControlBox]->getChildren()[Replay]->deactivate();
        mSceneLayers[ControlBox]->getChildren()[Play]->deactivate();
        mSceneLayers[ControlBox]->getChildren()[Pause]->activate();
        mIsAnimationPaused = false;
        mIsStepByStepMode = false;
    } 
}

void Heap::buildScene() {
    for (int i = 0; i < LayerCount; i++) {
        SceneNode::Ptr layer = std::make_unique<SceneNode>();
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    std::unique_ptr<RectangleButton> toggleButton = std::make_unique<RectangleButton>();
    toggleButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 680 * Constant::SCALE_Y), "MAX HEAP", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(toggleButton));

    std::unique_ptr<RectangleButton> createButton = std::make_unique<RectangleButton>();
    createButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 600 * Constant::SCALE_Y), "Create", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(createButton));

    std::unique_ptr<RectangleButton> insertButton = std::make_unique<RectangleButton>();
    insertButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 520 * Constant::SCALE_Y), "Insert", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(insertButton));

    std::unique_ptr<RectangleButton> deleteButton = std::make_unique<RectangleButton>();
    deleteButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 440 * Constant::SCALE_Y), "Delete", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(deleteButton));

    std::unique_ptr<RectangleButton> updateButton = std::make_unique<RectangleButton>();
    updateButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 360 * Constant::SCALE_Y), "Update", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(updateButton));

    std::unique_ptr<RectangleButton> searchButton = std::make_unique<RectangleButton>();
    searchButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 280 * Constant::SCALE_Y), "Search", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(searchButton));   

    std::unique_ptr<RectangleButton> sizeButton = std::make_unique<RectangleButton>();
    sizeButton->set(
        sf::Vector2f(60 * Constant::SCALE_X, 80 * Constant::SCALE_Y), sf::Vector2f(80 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 600 * Constant::SCALE_Y), "N =",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], sf::Color::Transparent, sf::Color::Black,
        sf::Color::Transparent, sf::Color::Black
    );
    sizeButton->deactivate();
    mSceneLayers[CreateOptions]->attachChild(std::move(sizeButton)); 

    std::unique_ptr<InputBox> sizeInput = std::make_unique<InputBox>();
    sizeInput->set(sf::Vector2f(150 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 585 * Constant::SCALE_Y), sf::Vector2f(100 * Constant::SCALE_X, 50 * Constant::SCALE_Y));
    sizeInput->deactivate();
    sizeInput->setContent(std::to_string(mInputSize));
    mSceneLayers[CreateOptions]->attachChild(std::move(sizeInput));

    std::unique_ptr<RectangleButton> randomButton = std::make_unique<RectangleButton>();
    randomButton->set(
        sf::Vector2f(150 * Constant::SCALE_X, 60 * Constant::SCALE_Y), sf::Vector2f(280 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 590 * Constant::SCALE_Y), "Random",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    randomButton->deactivate();
    mSceneLayers[CreateOptions]->attachChild(std::move(randomButton));

    std::unique_ptr<RectangleButton> fileButton = std::make_unique<RectangleButton>();
    fileButton->set(
        sf::Vector2f(200 * Constant::SCALE_X, 60 * Constant::SCALE_Y), sf::Vector2f(450 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 590 * Constant::SCALE_Y), "Init from file",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    fileButton->deactivate();
    mSceneLayers[CreateOptions]->attachChild(std::move(fileButton));

    std::unique_ptr<InputBox> insertInput = std::make_unique<InputBox>();
    insertInput->set(sf::Vector2f(100 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 505 * Constant::SCALE_Y));
    insertInput->deactivate();
    mSceneLayers[InsertOptions]->attachChild(std::move(insertInput));

    std::unique_ptr<RectangleButton> startInsertButton = std::make_unique<RectangleButton>();
    startInsertButton->set(
        sf::Vector2f(150 * Constant::SCALE_X, 60 * Constant::SCALE_Y), sf::Vector2f(430 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 510 * Constant::SCALE_Y), "Start",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    startInsertButton->deactivate();
    mSceneLayers[InsertOptions]->attachChild(std::move(startInsertButton));

    std::unique_ptr<InputBox> deleteInput = std::make_unique<InputBox>();
    deleteInput->set(sf::Vector2f(100 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 425 * Constant::SCALE_Y));
    deleteInput->deactivate();
    mSceneLayers[DeleteOptions]->attachChild(std::move(deleteInput));

    std::unique_ptr<RectangleButton> startDeleteButton = std::make_unique<RectangleButton>();
    startDeleteButton->set(
        sf::Vector2f(150 * Constant::SCALE_X, 60 * Constant::SCALE_Y), sf::Vector2f(430 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 430 * Constant::SCALE_Y), "Start",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    startDeleteButton->deactivate();
    mSceneLayers[DeleteOptions]->attachChild(std::move(startDeleteButton));

    std::unique_ptr<InputBox> searchInput = std::make_unique<InputBox>();
    searchInput->set(sf::Vector2f(100 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 265 * Constant::SCALE_Y));
    searchInput->deactivate();
    mSceneLayers[SearchOptions]->attachChild(std::move(searchInput));

    std::unique_ptr<RectangleButton> startSearchButton = std::make_unique<RectangleButton>();
    startSearchButton->set(
        sf::Vector2f(150 * Constant::SCALE_X, 60 * Constant::SCALE_Y), sf::Vector2f(430 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 270 * Constant::SCALE_Y), "Start",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    startSearchButton->deactivate();
    mSceneLayers[SearchOptions]->attachChild(std::move(startSearchButton));

    std::unique_ptr<ImageButton> playButton = std::make_unique<ImageButton>();
    playButton->set(
        ResourcesHolder::texturesHolder[Textures::PlayButton], ResourcesHolder::texturesHolder[Textures::PlayButtonHovered],
        sf::Vector2f(Constant::WINDOW_WIDTH / 2, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
    );
    mSceneLayers[ControlBox]->attachChild(std::move(playButton));

    std::unique_ptr<ImageButton> pauseButton = std::make_unique<ImageButton>();
    pauseButton->set(
        ResourcesHolder::texturesHolder[Textures::PauseButton], ResourcesHolder::texturesHolder[Textures::PauseButtonHovered],
        sf::Vector2f(Constant::WINDOW_WIDTH / 2, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
    );
    pauseButton->deactivate();
    mSceneLayers[ControlBox]->attachChild(std::move(pauseButton));

    std::unique_ptr<ImageButton> nextButton = std::make_unique<ImageButton>();
    nextButton->set(
        ResourcesHolder::texturesHolder[Textures::NextButton], ResourcesHolder::texturesHolder[Textures::NextButtonHovered],
        sf::Vector2f(Constant::WINDOW_WIDTH / 2 + 100, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
    );
    mSceneLayers[ControlBox]->attachChild(std::move(nextButton));

    std::unique_ptr<ImageButton> prevButton = std::make_unique<ImageButton>();
    prevButton->set(
        ResourcesHolder::texturesHolder[Textures::PrevButton], ResourcesHolder::texturesHolder[Textures::PrevButtonHovered],
        sf::Vector2f(Constant::WINDOW_WIDTH / 2 - 100, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
    );
    mSceneLayers[ControlBox]->attachChild(std::move(prevButton));

    std::unique_ptr<ImageButton> replayButton = std::make_unique<ImageButton>();
    replayButton->set(
        ResourcesHolder::texturesHolder[Textures::ReplayButton], ResourcesHolder::texturesHolder[Textures::ReplayButtonHovered],
        sf::Vector2f(Constant::WINDOW_WIDTH / 2, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
    );
    replayButton->deactivate();
    mSceneLayers[ControlBox]->attachChild(std::move(replayButton));

    std::unique_ptr<CodeBlock> codeBlock = std::make_unique<CodeBlock>();
    mSceneLayers[CodeBox]->attachChild(std::move(codeBlock));
    //---------------
    createRandomTree();
}



