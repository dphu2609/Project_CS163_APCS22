#include <State/Graph.hpp>

Graph::Graph(StateStack &stack, sf::RenderWindow &window) : State(stack, window) {
    buildScene();
}

Graph::~Graph() {
    clear();
    // clear(mRootForBackup);
    // while (!mGraphForBackward.empty()) {
    //     clear(mGraphForBackward.top()->root);
    //     delete mGraphForBackward.top();
    //     mGraphForBackward.pop();
    // }
}

void Graph::draw() {
    mWindow.draw(mSceneGraph);
}

void Graph::update() {
    mSceneGraph.update();
    if (mInsertAnimation && !mIsReversed) insertAnimation();
    if (mDeleteAnimation && !mIsReversed) deleteAnimation();
    // if (mUpdateAnimation && !mIsReversed) updateAnimation();
    if (mSearchAnimation && !mIsReversed) searchAnimation();
    mIsAnimationPaused = mIsStepByStepMode;
}

void Graph::handleEvent(sf::Event &event) {
    mSceneGraph.handleEvent(mWindow, event);
    updateGraphByMouse(event);
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

    if (mSceneLayers[CreateOptions]->getChildren()[FromMatrixButton]->isLeftClicked(mWindow, event)) {
        mInputSize = mSceneLayers[CreateOptions]->getChildren()[SizeInputBox]->getIntArrayData()[0];
        if (mInputSize != mPrevInputSize) {
            mPrevInputSize = mInputSize;
            for (auto &child : mSceneLayers[MatrixColumnIndex]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[MatrixRowIndex]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[Matrix]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[MatrixOptions]->getChildren()) child->deactivate();
        }
        int index = 0;
        for (auto &child : mSceneLayers[MatrixColumnIndex]->getChildren()) {
            if (index >= mInputSize) break;
            if (child->isActive()) {
                child->deactivate();
            } else {
                child->activate();
            }
            index++;
        }
        index = 0;
        for (auto &child : mSceneLayers[MatrixRowIndex]->getChildren()) {
            if (index >= mInputSize) break;
            if (child->isActive()) {
                child->deactivate();
            } else {
                child->activate();
            }
            index++;
        }
        for (int i = 0; i < mInputSize; i++) {
            for (int j = 0; j < mInputSize; j++) {
                if (mSceneLayers[Matrix]->getChildren()[i * 10 + j]->isActive()) {
                    mSceneLayers[Matrix]->getChildren()[i * 10 + j]->deactivate();
                } else {
                    mSceneLayers[Matrix]->getChildren()[i * 10 + j]->activate();
                }
            }
        }
        for (auto &child : mSceneLayers[MatrixOptions]->getChildren()) {
            if (child->isActive()) {
                child->deactivate();
            } else {
                child->activate();
            }
        }
    }

    if (mSceneLayers[MatrixOptions]->getChildren()[ConfirmMatrix]->isLeftClicked(mWindow, event)) {
        if (mSceneLayers[MatrixOptions]->getChildren()[ConfirmMatrix]->isActive()) {
            mConnections.clear();
            mDistance.clear();
            mEdgeIndex.clear();
            mConnections.resize(mInputSize);
            mDistance.resize(mInputSize);
            mEdgeIndex.resize(mInputSize);
            for (int i = 0; i < mInputSize; i++) {
                mConnections[i].resize(mInputSize, false);
                mDistance[i].resize(mInputSize, -1);
                mEdgeIndex[i].resize(mInputSize, {-1, false});
            } 
            for (int i = 0; i < mInputSize; i++) {
                for (int j = 0; j < mInputSize; j++) {
                    if (i == j) mSceneLayers[Matrix]->getChildren()[i * 10 + j]->setContent("");
                    if (mSceneLayers[Matrix]->getChildren()[i * 10 + j]->isContainOnlyOneNumber() && !mConnections[i][j] && !mConnections[j][i]) {
                        mConnections[i][j] = true;
                        mConnections[j][i] = true;
                        mDistance[i][j] = mSceneLayers[Matrix]->getChildren()[i * 10 + j]->getIntArrayData()[0];
                        mDistance[j][i] = mDistance[i][j];
                        mSceneLayers[Matrix]->getChildren()[j * 10 + i]->setContent(std::to_string(mDistance[i][j]));
                    }
                }
            }
            initGraph();
            setGraphScale(mInputSize);
            balanceGraph();
            createGraph();
        }   
    }

    if (mSceneLayers[MatrixOptions]->getChildren()[ClearMatrix]->isLeftClicked(mWindow, event)) {
        for (auto &child : mSceneLayers[Matrix]->getChildren()) child->setContent("");
    }


    if (mSceneLayers[CreateOptions]->getChildren()[RamdomButton]->isLeftClicked(mWindow, event)) {
        mInputSize = mSceneLayers[CreateOptions]->getChildren()[SizeInputBox]->getIntArrayData()[0];
        createRandomGraph();
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
        mIsReplay = false;
        mSceneLayers[ControlBox]->getChildren()[Replay]->deactivate();
        mSceneLayers[ControlBox]->getChildren()[Play]->deactivate();
        mSceneLayers[ControlBox]->getChildren()[Pause]->activate();
        mIsAnimationPaused = false;
        mIsStepByStepMode = false;
        applyGraphState(mBackupGraph);
    } 

    if (mSceneLayers[ReturnButton]->getChildren()[0]->isLeftClicked(mWindow, event)) {
        requestStackPop();
    }
}

void Graph::buildScene() {
    for (int i = 0; i < LayerCount; i++) {
        SceneNode::Ptr layer = std::make_unique<SceneNode>();
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
    std::unique_ptr<RectangleButton> createButton = std::make_unique<RectangleButton>();
    createButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 600 * Constant::SCALE_Y), "Create", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(createButton));

    std::unique_ptr<RectangleButton> insertButton = std::make_unique<RectangleButton>();
    insertButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 520 * Constant::SCALE_Y), "Dijkstra", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(insertButton));

    std::unique_ptr<RectangleButton> deleteButton = std::make_unique<RectangleButton>();
    deleteButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 440 * Constant::SCALE_Y), "Prim", 
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
    sizeInput->set(sf::Vector2f(150 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 585), sf::Vector2f(100 * Constant::SCALE_X, 50 * Constant::SCALE_Y));
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

    std::unique_ptr<RectangleButton> matrixButton = std::make_unique<RectangleButton>();
    matrixButton->set(
        sf::Vector2f(200 * Constant::SCALE_X, 60 * Constant::SCALE_Y), sf::Vector2f(670 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 590 * Constant::SCALE_Y), "Matrix",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    matrixButton->deactivate();
    mSceneLayers[CreateOptions]->attachChild(std::move(matrixButton));

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

    std::unique_ptr<ImageButton> returnButton = std::make_unique<ImageButton>();
    returnButton->set(
        ResourcesHolder::texturesHolder[Textures::ReturnButton], ResourcesHolder::texturesHolder[Textures::ReturnButtonHovered],
        sf::Vector2f(100 * Constant::SCALE_X, 100 * Constant::SCALE_Y)
    );
    mSceneLayers[ReturnButton]->attachChild(std::move(returnButton));

    sf::Vector2f startPos = sf::Vector2f(Constant::WINDOW_WIDTH / 2 - 100 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 650 * Constant::SCALE_Y);
    for (int i = 0; i < 10; i++) {
        std::unique_ptr<RectangleButton> matrixColumnIndex = std::make_unique<RectangleButton>();
        matrixColumnIndex->set(Size::MATRIX_SLOT_SIZE, startPos + sf::Vector2f((i + 1) * Size::MATRIX_SLOT_SIZE.x, 0), std::to_string(i),
            ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::MATRIX_SLOT_COLOR, Color::MATRIX_SLOT_TEXT_COLOR,
            Color::MATRIX_SLOT_COLOR, Color::MATRIX_SLOT_TEXT_COLOR, 2.f * Constant::SCALE_X, Color::MATRIX_SLOT_OUTLINE_COLOR, Color::MATRIX_SLOT_OUTLINE_COLOR
        );
        matrixColumnIndex->deactivate();
        mSceneLayers[MatrixColumnIndex]->attachChild(std::move(matrixColumnIndex));
    }
    for (int i = 0; i < 10; i++) {
        std::unique_ptr<RectangleButton> matrixRowIndex = std::make_unique<RectangleButton>();
        matrixRowIndex->set(Size::MATRIX_SLOT_SIZE, startPos + sf::Vector2f(0, (i + 1) * Size::MATRIX_SLOT_SIZE.y), std::to_string(i),
            ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::MATRIX_SLOT_COLOR, Color::MATRIX_SLOT_TEXT_COLOR,
            Color::MATRIX_SLOT_COLOR, Color::MATRIX_SLOT_TEXT_COLOR, 2.f * Constant::SCALE_X, Color::MATRIX_SLOT_OUTLINE_COLOR, Color::MATRIX_SLOT_OUTLINE_COLOR
        );
        matrixRowIndex->deactivate();
        mSceneLayers[MatrixRowIndex]->attachChild(std::move(matrixRowIndex));
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            std::unique_ptr<InputBox> matrixSlot = std::make_unique<InputBox>();
            matrixSlot->set(
                startPos + sf::Vector2f((j + 1) * Size::MATRIX_SLOT_SIZE.x, (i + 1) * Size::MATRIX_SLOT_SIZE.y),
                Size::MATRIX_SLOT_SIZE
            );
            matrixSlot->deactivate();
            mSceneLayers[Matrix]->attachChild(std::move(matrixSlot));
        }
    }
    std::unique_ptr<RectangleButton> confirmButton = std::make_unique<RectangleButton>();
    confirmButton->set(
        sf::Vector2f(150 * Constant::SCALE_X, 60 * Constant::SCALE_Y), sf::Vector2f(700 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 510 * Constant::SCALE_Y), "Comfirm",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    confirmButton->deactivate();
    mSceneLayers[MatrixOptions]->attachChild(std::move(confirmButton));

    std::unique_ptr<RectangleButton> clearButton = std::make_unique<RectangleButton>();
    clearButton->set(
        sf::Vector2f(150 * Constant::SCALE_X, 60 * Constant::SCALE_Y), sf::Vector2f(700 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 430 * Constant::SCALE_Y), "Clear",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    clearButton->deactivate();
    mSceneLayers[MatrixOptions]->attachChild(std::move(clearButton));

    std::unique_ptr<CodeBlock> codeBlock = std::make_unique<CodeBlock>();
    mSceneLayers[CodeBox]->attachChild(std::move(codeBlock));
    //---------------
    createRandomGraph();
}



