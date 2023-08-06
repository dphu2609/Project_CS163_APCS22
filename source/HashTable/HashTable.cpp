#include <State/HashTable.hpp>

HashTable::HashTable(StateStack &stack, sf::RenderWindow &window) : State(stack, window) {
    buildScene();
}

HashTable::~HashTable() {}

void HashTable::draw() {
    mWindow.draw(mSceneGraph);
}

void HashTable::update() {
    mSceneGraph.update();
    if (mInsertAnimation && !mIsReversed) insertAnimation();
    if (mDeleteAnimation && !mIsReversed) deleteAnimation();
    if (mSearchAnimation && !mIsReversed) searchAnimation();
    mIsAnimationPaused = mIsStepByStepMode;
}

void HashTable::handleEvent(sf::Event &event) {
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
        if (mInputSize < 7 || mInputSize > 30) {
            annouceError("Size must be in range [7, 30]");
        }
        else createRandomTree();
    }

    if (mSceneLayers[CreateOptions]->getChildren()[FromFileButton]->isLeftClicked(mWindow, event)) {
        annouceError("File format must be size + array of integers");
        mIsInitFromFile = true;
    }

    if (mSceneLayers[InsertOptions]->getChildren()[InsertStart]->isLeftClicked(mWindow, event)) {
        std::vector<int> inputList = mSceneLayers[InsertOptions]->getChildren()[InsertInput]->getIntArrayData();
        bool isInputValid = true;
        for (int input : inputList) {
            if (input < -999999 || input > 999999) {
                annouceError("Input must be in range [-999999, 999999]");
                isInputValid = false;
            }
        }
        if (isInputValid) {
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
            for (auto &child : mSceneLayers[CreateOptions]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[InsertOptions]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[DeleteOptions]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[SearchOptions]->getChildren()) child->deactivate();
        }
    }

    if (mSceneLayers[DeleteOptions]->getChildren()[DeleteStart]->isLeftClicked(mWindow, event)) {
        std::vector<int> inputList = mSceneLayers[DeleteOptions]->getChildren()[DeleteInput]->getIntArrayData();
        bool isInputValid = true;
        for (int input : inputList) {
            if (input < -999999 || input > 999999) {
                annouceError("Input must be in range [-999999, 999999]");
                isInputValid = false;
            }
        }
        if (isInputValid) {
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
            for (auto &child : mSceneLayers[CreateOptions]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[InsertOptions]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[DeleteOptions]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[SearchOptions]->getChildren()) child->deactivate();
        }
    }

    if (mSceneLayers[SearchOptions]->getChildren()[SearchStart]->isLeftClicked(mWindow, event)) {
        std::vector<int> inputList = mSceneLayers[SearchOptions]->getChildren()[SearchInput]->getIntArrayData();
        bool isInputValid = true;
        for (int input : inputList) {
            if (input < -999999 || input > 999999) {
                annouceError("Input must be in range [-999999, 999999]");
                isInputValid = false;
            }
        }
        if (isInputValid) {
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
            for (auto &child : mSceneLayers[CreateOptions]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[InsertOptions]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[DeleteOptions]->getChildren()) child->deactivate();
            for (auto &child : mSceneLayers[SearchOptions]->getChildren()) child->deactivate();
        }
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

    if (mSceneLayers[ControlBox]->getChildren()[Replay]->isActive() && !mIsReplay) {
        mSceneLayers[ControlBox]->getChildren()[Replay]->deactivate();
    }

    if (mSceneLayers[ControlBox]->getChildren()[Replay]->isActive() && mSceneLayers[ControlBox]->getChildren()[Replay]->isLeftClicked(mWindow, event)) {
        mAnimationStep = 1;
        mIsReplay = false;
        mSceneLayers[ControlBox]->getChildren()[Replay]->deactivate();
        mSceneLayers[ControlBox]->getChildren()[Play]->deactivate();
        mSceneLayers[ControlBox]->getChildren()[Pause]->activate();
        mIsAnimationPaused = false;
        mIsStepByStepMode = false;
        applyTreeState(mTreeStateForReplay);
    } 

    if (mSceneLayers[ErrorConfirmButton]->getChildren()[0]->isLeftClicked(mWindow, event)) {
        if (mIsInitFromFile) initFromFile();
        mSceneLayers[ErrorContainer]->getChildren()[0]->deactivate();
        mSceneLayers[ErrorConfirmButton]->getChildren()[0]->deactivate();
    }

    if (mSceneLayers[ReturnButton]->getChildren()[0]->isLeftClicked(mWindow, event)) {
        requestStackPop();
    }
}

void HashTable::buildScene() {
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

    std::unique_ptr<RectangleButton> searchButton = std::make_unique<RectangleButton>();
    searchButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 360 * Constant::SCALE_Y), "Search", 
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(searchButton));   

    std::unique_ptr<RectangleButton> sizeButton = std::make_unique<RectangleButton>();
    sizeButton->set(
        sf::Vector2f(60 * Constant::SCALE_X, 80 * Constant::SCALE_Y), sf::Vector2f(80 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 600 * Constant::SCALE_Y), "N =",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], sf::Color::Transparent, (!Color::IS_DARK_THEME ? sf::Color::Black : sf::Color::White),
        sf::Color::Transparent, (!Color::IS_DARK_THEME ? sf::Color::Black : sf::Color::White)
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
    searchInput->set(sf::Vector2f(100 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 345 * Constant::SCALE_Y));
    searchInput->deactivate();
    mSceneLayers[SearchOptions]->attachChild(std::move(searchInput));

    std::unique_ptr<RectangleButton> startSearchButton = std::make_unique<RectangleButton>();
    startSearchButton->set(
        sf::Vector2f(150 * Constant::SCALE_X, 60 * Constant::SCALE_Y), sf::Vector2f(430 * Constant::SCALE_X + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 350 * Constant::SCALE_Y), "Start",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    startSearchButton->deactivate();
    mSceneLayers[SearchOptions]->attachChild(std::move(startSearchButton));

    std::unique_ptr<ImageButton> playButton = std::make_unique<ImageButton>();
    std::unique_ptr<ImageButton> pauseButton = std::make_unique<ImageButton>();
    std::unique_ptr<ImageButton> nextButton = std::make_unique<ImageButton>();
    std::unique_ptr<ImageButton> prevButton = std::make_unique<ImageButton>();
    std::unique_ptr<ImageButton> replayButton = std::make_unique<ImageButton>();
    std::unique_ptr<ImageButton> returnButton = std::make_unique<ImageButton>();

    if (!Color::IS_DARK_THEME) {
        playButton->set(
            ResourcesHolder::texturesHolder[Textures::PlayButton], ResourcesHolder::texturesHolder[Textures::PlayButtonHovered],
            sf::Vector2f(Constant::WINDOW_WIDTH / 2, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
        );
        pauseButton->set(
            ResourcesHolder::texturesHolder[Textures::PauseButton], ResourcesHolder::texturesHolder[Textures::PauseButtonHovered],
            sf::Vector2f(Constant::WINDOW_WIDTH / 2, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
        );
        nextButton->set(
            ResourcesHolder::texturesHolder[Textures::NextButton], ResourcesHolder::texturesHolder[Textures::NextButtonHovered],
            sf::Vector2f(Constant::WINDOW_WIDTH / 2 + 100, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
        );
        prevButton->set(
            ResourcesHolder::texturesHolder[Textures::PrevButton], ResourcesHolder::texturesHolder[Textures::PrevButtonHovered],
            sf::Vector2f(Constant::WINDOW_WIDTH / 2 - 100, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
        );
        replayButton->set(
            ResourcesHolder::texturesHolder[Textures::ReplayButton], ResourcesHolder::texturesHolder[Textures::ReplayButtonHovered],
            sf::Vector2f(Constant::WINDOW_WIDTH / 2, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
        );
        returnButton->set(
            ResourcesHolder::texturesHolder[Textures::ReturnButton], ResourcesHolder::texturesHolder[Textures::ReturnButtonHovered],
            sf::Vector2f(100 * Constant::SCALE_X, 100 * Constant::SCALE_Y)
        );
    }
    else {
        playButton->set(
            ResourcesHolder::texturesHolder[Textures::PlayButtonWhite], ResourcesHolder::texturesHolder[Textures::PlayButtonHovered],
            sf::Vector2f(Constant::WINDOW_WIDTH / 2, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
        );
        pauseButton->set(
            ResourcesHolder::texturesHolder[Textures::PauseButtonWhite], ResourcesHolder::texturesHolder[Textures::PauseButtonHovered],
            sf::Vector2f(Constant::WINDOW_WIDTH / 2, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
        );
        nextButton->set(
            ResourcesHolder::texturesHolder[Textures::NextButtonWhite], ResourcesHolder::texturesHolder[Textures::NextButtonHovered],
            sf::Vector2f(Constant::WINDOW_WIDTH / 2 + 100, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
        );
        prevButton->set(
            ResourcesHolder::texturesHolder[Textures::PrevButtonWhite], ResourcesHolder::texturesHolder[Textures::PrevButtonHovered],
            sf::Vector2f(Constant::WINDOW_WIDTH / 2 - 100, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
        );
        replayButton->set(
            ResourcesHolder::texturesHolder[Textures::ReplayButtonWhite], ResourcesHolder::texturesHolder[Textures::ReplayButtonHovered],
            sf::Vector2f(Constant::WINDOW_WIDTH / 2, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y)
        );
        returnButton->set(
            ResourcesHolder::texturesHolder[Textures::ReturnButtonWhite], ResourcesHolder::texturesHolder[Textures::ReturnButtonHovered],
            sf::Vector2f(100 * Constant::SCALE_X, 100 * Constant::SCALE_Y)
        );
    }
    mSceneLayers[ControlBox]->attachChild(std::move(playButton));
    pauseButton->deactivate();
    mSceneLayers[ControlBox]->attachChild(std::move(pauseButton));
    mSceneLayers[ControlBox]->attachChild(std::move(nextButton));
    mSceneLayers[ControlBox]->attachChild(std::move(prevButton));
    replayButton->deactivate();
    mSceneLayers[ControlBox]->attachChild(std::move(replayButton));
    mSceneLayers[ReturnButton]->attachChild(std::move(returnButton));

    std::unique_ptr<RectangleButton> errorContainer = std::make_unique<RectangleButton>();
    errorContainer->set(
        sf::Vector2f(1200 * Constant::SCALE_X, 100 * Constant::SCALE_Y), 
        sf::Vector2f(Constant::WINDOW_WIDTH / 2 - 600 * Constant::SCALE_X, Constant::WINDOW_HEIGHT / 2 - 50 * Constant::SCALE_Y), "",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::ERROR_MESSAGE_BOX_COLOR, Color::ERROR_MESSAGE_BOX_TEXT_COLOR,
        Color::ERROR_MESSAGE_BOX_COLOR, Color::ERROR_MESSAGE_BOX_TEXT_COLOR, 5 * Constant::SCALE_X, Color::ERROR_MESSAGE_BOX_OUTLINE_COLOR,
        Color::ERROR_MESSAGE_BOX_OUTLINE_COLOR
    );
    errorContainer->deactivate();   
    mSceneLayers[ErrorContainer]->attachChild(std::move(errorContainer));

    std::unique_ptr<RectangleButton> errorConfirmButton = std::make_unique<RectangleButton>();
    errorConfirmButton->set(
        sf::Vector2f(150 * Constant::SCALE_X, 60 * Constant::SCALE_Y), 
        sf::Vector2f(Constant::WINDOW_WIDTH / 2 - 60 * Constant::SCALE_X, Constant::WINDOW_HEIGHT / 2 + 70 * Constant::SCALE_Y), "OK",
        ResourcesHolder::fontsHolder[Fonts::RobotoRegular], Color::SETTINGS_BUTTON_COLOR, sf::Color::Black,
        Color::SETTINGS_BUTTON_HOVERED_COLOR, sf::Color::Black
    );
    errorConfirmButton->deactivate();
    mSceneLayers[ErrorConfirmButton]->attachChild(std::move(errorConfirmButton));

    std::unique_ptr<CodeBlock> codeBlock = std::make_unique<CodeBlock>();
    mSceneLayers[CodeBox]->attachChild(std::move(codeBlock));
    //---------------
    createRandomTree();
}

void HashTable::annouceError(std::string error) {
    mSceneLayers[ErrorContainer]->getChildren()[0]->activate();
    mSceneLayers[ErrorConfirmButton]->getChildren()[0]->activate();
    mSceneLayers[ErrorContainer]->getChildren()[0]->setContent(error);
}

void HashTable::initFromFile() {
    const char *path = tinyfd_openFileDialog(
        "Open file", "", 0, nullptr, nullptr, 0
    );
    mIsInitFromFile = false;
    std::ifstream fin;
    fin.open(path);
    if (!fin.is_open()) {
        annouceError("Cannot open file");
        return;
    }
    fin >> mInputSize;
    if (fin.fail()) {
        annouceError("Invalid input");
        return;
    }
    if (mInputSize > 30) {
        annouceError("Too many input, maximum is 30");
        return;
    }
    while (!fin.eof()) {
        int x;
        fin >> x;
        if (fin.fail()) {
            annouceError("Invalid input");
            return;
        }
        mInputData.push_back(x);
        if (mInputData.size() >= mInputSize) {
            break;
        }
    }
    mNodeList.clear();
    mNodeList.resize(mInputSize);
    for (auto &node : mNodeList) {
        node.val = -1;
        node.isNodeHighlighted = false;
        node.position = sf::Vector2f(0, 0);
    }
    mSceneLayers[CreateOptions]->getChildren()[SizeInputBox]->setContent(std::to_string(mInputSize));
    for (auto &input : mInputData) insert(input);
    setTreeScale(mInputSize);
    balanceTree();
    createTree();
}   



