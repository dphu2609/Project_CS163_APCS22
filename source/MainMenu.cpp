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

    if (mSceneLayers[ThemeButton]->getChildren()[0]->isLeftClicked(mWindow, event)) {
        changeTheme();
    }

    handleHoverOfDataStructureButtons(event, AVL, AVLNodes, AVLEdges);
    handleHoverOfDataStructureButtons(event, Tree234, Tree234Nodes, Tree234Edges);
    handleHoverOfDataStructureButtons(event, Trie, TrieNodes, TrieEdges);
    handleHoverOfDataStructureButtons(event, HashTable, HashTableNodes);
    handleHoverOfDataStructureButtons(event, Graph, GraphNodes, GraphEdges);
    handleHoverOfDataStructureButtons(event, Heap, HeapNodes, HeapEdges);
}

void MainMenu::handleHoverOfDataStructureButtons(sf::Event &event, int indexOfDSLayer, int indexOfNodeLayer, int indexOfEdgeLayer) {
    if (mSceneLayers[DataStructureButtons]->getChildren()[indexOfDSLayer]->isHovered(mWindow, event)) {
        if (!mIsHovered[indexOfDSLayer]) {
            mIsHovered[indexOfDSLayer] = true;
            for (auto &child : mSceneLayers[indexOfNodeLayer]->getChildren()) child->resetAnimationVar();
            if (indexOfEdgeLayer != -1) for (auto &child : mSceneLayers[indexOfEdgeLayer]->getChildren()) child->resetAnimationVar();
            int index = 0;
            for (auto &child : mSceneLayers[indexOfNodeLayer]->getChildren()) {
                if (indexOfDSLayer == Trie && (index == 3 || index == 6)) {
                    child->change3Color(
                        Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, 
                        Color::NODE_HIGHLIGHT_OUTLINE_COLOR, 5 * (1 / Animation::SPEED)
                    );
                }
                else {
                    child->change3Color(
                        Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, 
                        Color::NODE_HIGHLIGHT_OUTLINE_COLOR, 5 * (1 / Animation::SPEED)
                    );
                }
                index++;
            }
            if (indexOfEdgeLayer != -1) for (auto &child : mSceneLayers[indexOfEdgeLayer]->getChildren()) child->change1Color(Color::NODE_HIGHLIGHT_COLOR, 5 * (1 / Animation::SPEED));
        }
    }
    else if (mIsHovered[indexOfDSLayer]) {
        mIsHovered[indexOfDSLayer] = false;
        for (auto &child : mSceneLayers[indexOfNodeLayer]->getChildren()) child->resetAnimationVar();
        if (indexOfEdgeLayer != -1) for (auto &child : mSceneLayers[indexOfEdgeLayer]->getChildren()) child->resetAnimationVar();
        int index = 0;
        for (auto &child : mSceneLayers[indexOfNodeLayer]->getChildren()) {
            if (indexOfDSLayer == Trie && (index == 3 || index == 6)) {
                child->change3Color(
                    Color::NODE_TEXT_COLOR, Color::NODE_COLOR, 
                    Color::NODE_OUTLINE_COLOR, 5 * (1 / Animation::SPEED)
                );
            }
            else {
                child->change3Color(
                    Color::NODE_COLOR, Color::NODE_TEXT_COLOR, Color::NODE_OUTLINE_COLOR, 5 * (1 / Animation::SPEED)
                );
            }
            index++;
        }
        if (indexOfEdgeLayer != -1) for (auto &child : mSceneLayers[indexOfEdgeLayer]->getChildren()) child->change1Color(Color::NODE_EDGE_COLOR, 5 * (1 / Animation::SPEED));
    }
}

void MainMenu::buildScene() {
    if (mFirstInit) {
        for (int i = 0; i < LayerCount; i++) {
            if (mSceneLayers[i] != nullptr) mSceneLayers[i]->getChildren().clear();
            mSceneLayers[i] = nullptr;
        }
        mSceneGraph.getChildren().clear();
        mIsHovered.clear();
    }
    else mFirstInit = true;
    for (int i = 0; i < LayerCount; i++) {
        SceneNode::Ptr layer = std::make_unique<SceneNode>();
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
        mIsHovered.push_back(false);
    }

    std::unique_ptr<RectangleButton> appName = std::make_unique<RectangleButton>();
    appName->set(
        sf::Vector2f(1800 * Constant::SCALE_X, 500 * Constant::SCALE_Y),
        sf::Vector2f(Constant::WINDOW_WIDTH / 2 - 900 * Constant::SCALE_X, 0),
        "DSVisual", ResourcesHolder::fontsHolder[Fonts::RussoOne],
        sf::Color::Transparent, Color::NODE_EDGE_COLOR, sf::Color::Transparent, Color::NODE_EDGE_COLOR
    );
    mSceneLayers[AppName]->attachChild(std::move(appName));

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
    avlPos.push_back(startPos + sf::Vector2f(Size::DATA_STRUCTURE_BUTTON.x / 2 + NODE_RADIUS * 2, NODE_RADIUS * 4));
    avlPos.push_back(avlPos[0] + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL * 2, NODE_DISTANCE_VERTICAL));
    avlPos.push_back(avlPos[0] + sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL));
    avlPos.push_back(avlPos[1] + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL));
    avlPos.push_back(avlPos[1] + sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL));
    
    for (int i = 0; i < avlPos.size(); i++) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        std::string content = "";
        if (i == 0) content = "V";
        else if (i == 1) content = "A";
        else if (i == 2) content = "L";
        node->set(true, content, avlPos[i], NODE_RADIUS);
        mSceneLayers[AVLNodes]->attachChild(std::move(node));
    }

    for (auto &pos : avlPos) pos += sf::Vector2f(NODE_RADIUS, NODE_RADIUS);

    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> avlEdgePos;
    avlEdgePos.push_back(std::make_pair(avlPos[0], avlPos[1]));
    avlEdgePos.push_back(std::make_pair(avlPos[0], avlPos[2]));
    avlEdgePos.push_back(std::make_pair(avlPos[1], avlPos[3]));
    avlEdgePos.push_back(std::make_pair(avlPos[1], avlPos[4]));

    for (int i = 0; i < avlEdgePos.size(); i++) {
        std::unique_ptr<Edge> edge = std::make_unique<Edge>();
        edge->set(avlEdgePos[i].first, avlEdgePos[i].second);
        mSceneLayers[AVLEdges]->attachChild(std::move(edge));
    }

    std::vector<sf::Vector2f> tree234Pos;
    tree234Pos.push_back(startPos + sf::Vector2f(Size::DATA_STRUCTURE_BUTTON.x / 2 - NODE_RADIUS + horizontalDistance, NODE_RADIUS * 6));
    tree234Pos.push_back(tree234Pos[0] - sf::Vector2f(2 * NODE_RADIUS + NODE_RADIUS / 4, 0));
    tree234Pos.push_back(tree234Pos[0] + sf::Vector2f(2 * NODE_RADIUS + NODE_RADIUS / 4, 0));
    tree234Pos.push_back(tree234Pos[0] + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL * 2, NODE_DISTANCE_VERTICAL));
    tree234Pos.push_back(tree234Pos[0] + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL * 2 / 3, NODE_DISTANCE_VERTICAL));
    tree234Pos.push_back(tree234Pos[0] + sf::Vector2f(NODE_DISTANCE_HORIZONTAL * 2 / 3, NODE_DISTANCE_VERTICAL));
    tree234Pos.push_back(tree234Pos[0] + sf::Vector2f(NODE_DISTANCE_HORIZONTAL * 2, NODE_DISTANCE_VERTICAL));

    std::string content = "324TREE";

    for (int i = 0; i < tree234Pos.size(); i++) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        std::string cont = " ";
        cont[0] = content[i];
        node->set(false, cont, tree234Pos[i], NODE_RADIUS);
        mSceneLayers[Tree234Nodes]->attachChild(std::move(node));
    }

    for (auto &pos : tree234Pos) pos += sf::Vector2f(NODE_RADIUS, NODE_RADIUS);
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> tree234EdgePos;
    tree234EdgePos.push_back(std::make_pair(tree234Pos[0], tree234Pos[3]));
    tree234EdgePos.push_back(std::make_pair(tree234Pos[0], tree234Pos[4]));
    tree234EdgePos.push_back(std::make_pair(tree234Pos[0], tree234Pos[5]));
    tree234EdgePos.push_back(std::make_pair(tree234Pos[0], tree234Pos[6]));

    for (int i = 0; i < tree234EdgePos.size(); i++) {
        std::unique_ptr<Edge> edge = std::make_unique<Edge>();
        edge->set(tree234EdgePos[i].first, tree234EdgePos[i].second);
        mSceneLayers[Tree234Edges]->attachChild(std::move(edge));
    }

    std::vector<sf::Vector2f> triePos;
    triePos.push_back(startPos + sf::Vector2f(Size::DATA_STRUCTURE_BUTTON.x / 2 + horizontalDistance * 2 - NODE_RADIUS, NODE_RADIUS * 2));
    triePos.push_back(triePos[0] + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL));
    triePos.push_back(triePos[0] + sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL));
    triePos.push_back(triePos[1] + sf::Vector2f(0, NODE_DISTANCE_VERTICAL / 1.2));
    triePos.push_back(triePos[2] + sf::Vector2f(0, NODE_DISTANCE_VERTICAL / 1.2));
    triePos.push_back(triePos[4] + sf::Vector2f(0, NODE_DISTANCE_VERTICAL / 1.2));
    triePos.push_back(triePos[5] + sf::Vector2f(0, NODE_DISTANCE_VERTICAL / 1.2));

    content = " DTPRIE";

    for (int i = 0; i < triePos.size(); i++) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        std::string cont = " ";
        cont[0] = content[i];
        if (i == 3 || i == 6) {
            node->set(true, cont, triePos[i], NODE_RADIUS, Color::NODE_TEXT_COLOR, Color::NODE_COLOR, Color::NODE_OUTLINE_COLOR);
        }
        else node->set(true, cont, triePos[i], NODE_RADIUS);
        mSceneLayers[TrieNodes]->attachChild(std::move(node));
    }

    for (auto &pos : triePos) pos += sf::Vector2f(NODE_RADIUS, NODE_RADIUS);
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> trieEdgePos;
    trieEdgePos.push_back(std::make_pair(triePos[0], triePos[1]));
    trieEdgePos.push_back(std::make_pair(triePos[0], triePos[2]));
    trieEdgePos.push_back(std::make_pair(triePos[1], triePos[3]));
    trieEdgePos.push_back(std::make_pair(triePos[2], triePos[4]));
    trieEdgePos.push_back(std::make_pair(triePos[4], triePos[5]));
    trieEdgePos.push_back(std::make_pair(triePos[5], triePos[6]));

    for (int i = 0; i < trieEdgePos.size(); i++) {
        std::unique_ptr<Edge> edge = std::make_unique<Edge>();
        edge->set(trieEdgePos[i].first, trieEdgePos[i].second);
        mSceneLayers[TrieEdges]->attachChild(std::move(edge));
    }

    std::vector<sf::Vector2f> hashTablePos;
    hashTablePos.push_back(startPos + sf::Vector2f(Size::DATA_STRUCTURE_BUTTON.x / 2 - NODE_RADIUS, verticalDistance + NODE_RADIUS * 6));
    hashTablePos.push_back(hashTablePos[0] + sf::Vector2f(-4 * NODE_RADIUS * 2, NODE_DISTANCE_VERTICAL));
    hashTablePos.push_back(hashTablePos[1] + sf::Vector2f(NODE_RADIUS * 2 + NODE_RADIUS / 8, 0));
    hashTablePos.push_back(hashTablePos[2] + sf::Vector2f(NODE_RADIUS * 2 + NODE_RADIUS / 8, 0));
    hashTablePos.push_back(hashTablePos[3] + sf::Vector2f(NODE_RADIUS * 2 + NODE_RADIUS / 8, 0));
    hashTablePos.push_back(hashTablePos[4] + sf::Vector2f(NODE_RADIUS * 2 + NODE_RADIUS / 8, 0));
    hashTablePos.push_back(hashTablePos[5] + sf::Vector2f(NODE_RADIUS * 2 + NODE_RADIUS / 8, 0));
    hashTablePos.push_back(hashTablePos[6] + sf::Vector2f(NODE_RADIUS * 2 + NODE_RADIUS / 8, 0));
    hashTablePos.push_back(hashTablePos[7] + sf::Vector2f(NODE_RADIUS * 2 + NODE_RADIUS / 8, 0));
    hashTablePos.push_back(hashTablePos[8] + sf::Vector2f(NODE_RADIUS * 2 + NODE_RADIUS / 8, 0));

    content = " HASHTABLE";

    for (int i = 0; i < hashTablePos.size(); i++) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        bool isCircle = (i == 0 ? true : false);
        std::string cont = " ";
        cont[0] = content[i];
        if (i == 0) cont = "21";
        node->set(isCircle, cont, hashTablePos[i], NODE_RADIUS);
        if (i != 0) node->setLabel(std::to_string(i - 1));
        mSceneLayers[HashTableNodes]->attachChild(std::move(node));
    }

    std::vector<sf::Vector2f> graphPos;
    graphPos.push_back(startPos + sf::Vector2f(Size::DATA_STRUCTURE_BUTTON.x / 2 - NODE_RADIUS + horizontalDistance, verticalDistance + NODE_RADIUS * 2));
    graphPos.push_back(graphPos[0] + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL * 2, NODE_DISTANCE_VERTICAL));
    graphPos.push_back(graphPos[0] + sf::Vector2f(NODE_DISTANCE_HORIZONTAL * 2, NODE_DISTANCE_VERTICAL));
    graphPos.push_back(graphPos[1] + sf::Vector2f(0, NODE_DISTANCE_VERTICAL * 2));
    graphPos.push_back(graphPos[2] + sf::Vector2f(0, NODE_DISTANCE_VERTICAL * 2));

    content = "GRAPH";

    for (int i = 0; i < graphPos.size(); i++) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        std::string cont = " ";
        cont[0] = content[i];
        node->set(true, cont, graphPos[i], NODE_RADIUS);
        mSceneLayers[GraphNodes]->attachChild(std::move(node));
    }

    for (auto &pos : graphPos) pos += sf::Vector2f(NODE_RADIUS, NODE_RADIUS);
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> graphEdgePos;
    graphEdgePos.push_back(std::make_pair(graphPos[0], graphPos[1]));
    graphEdgePos.push_back(std::make_pair(graphPos[0], graphPos[2]));
    graphEdgePos.push_back(std::make_pair(graphPos[1], graphPos[2]));
    graphEdgePos.push_back(std::make_pair(graphPos[1], graphPos[3]));
    graphEdgePos.push_back(std::make_pair(graphPos[2], graphPos[4]));
    graphEdgePos.push_back(std::make_pair(graphPos[3], graphPos[4]));

    for (int i = 0; i < graphEdgePos.size(); i++) {
        std::unique_ptr<Edge> edge = std::make_unique<Edge>();
        edge->set(graphEdgePos[i].first, graphEdgePos[i].second);
        mSceneLayers[GraphEdges]->attachChild(std::move(edge));
    }

    std::vector<sf::Vector2f> heapPos;
    heapPos.push_back(startPos + sf::Vector2f(Size::DATA_STRUCTURE_BUTTON.x / 2 - NODE_RADIUS + horizontalDistance * 2, verticalDistance + NODE_RADIUS * 2));
    heapPos.push_back(heapPos[0] + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL * 2 * 0.75, NODE_DISTANCE_VERTICAL));
    heapPos.push_back(heapPos[0] + sf::Vector2f(NODE_DISTANCE_HORIZONTAL * 2 * 0.75, NODE_DISTANCE_VERTICAL));
    heapPos.push_back(heapPos[1] + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL * 0.75, NODE_DISTANCE_VERTICAL));
    heapPos.push_back(heapPos[1] + sf::Vector2f(NODE_DISTANCE_HORIZONTAL * 0.75, NODE_DISTANCE_VERTICAL));
    heapPos.push_back(heapPos[2] + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL * 0.75, NODE_DISTANCE_VERTICAL));
    heapPos.push_back(heapPos[2] + sf::Vector2f(NODE_DISTANCE_HORIZONTAL * 0.75, NODE_DISTANCE_VERTICAL));

    content = "   HEAP";

    for (int i = 0; i < heapPos.size(); i++) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        std::string cont = " ";
        cont[0] = content[i];
        node->set(true, cont, heapPos[i], NODE_RADIUS);
        node->setLabel(std::to_string(i + 1));
        mSceneLayers[HeapNodes]->attachChild(std::move(node));
    }

    for (auto &pos : heapPos) pos += sf::Vector2f(NODE_RADIUS, NODE_RADIUS);
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> heapEdgePos;
    heapEdgePos.push_back(std::make_pair(heapPos[0], heapPos[1]));
    heapEdgePos.push_back(std::make_pair(heapPos[0], heapPos[2]));
    heapEdgePos.push_back(std::make_pair(heapPos[1], heapPos[3]));
    heapEdgePos.push_back(std::make_pair(heapPos[1], heapPos[4]));
    heapEdgePos.push_back(std::make_pair(heapPos[2], heapPos[5]));
    heapEdgePos.push_back(std::make_pair(heapPos[2], heapPos[6]));

    for (int i = 0; i < heapEdgePos.size(); i++) {
        std::unique_ptr<Edge> edge = std::make_unique<Edge>();
        edge->set(heapEdgePos[i].first, heapEdgePos[i].second);
        mSceneLayers[HeapEdges]->attachChild(std::move(edge));
    }

    std::unique_ptr<RectangleButton> themeButton = std::make_unique<RectangleButton>();
    themeButton->set(
        sf::Vector2f(300 * Constant::SCALE_X, 60 * Constant::SCALE_Y),
        sf::Vector2f(Constant::WINDOW_WIDTH / 2 - 150 * Constant::SCALE_X, Constant::WINDOW_HEIGHT - 200 * Constant::SCALE_Y),
        "SWITCH THEME", ResourcesHolder::fontsHolder[Fonts::RobotoRegular],
        Color::THEME_BUTTON_COLOR, Color::THEME_BUTTON_TEXT_COLOR,
        Color::THEME_BUTTON_HOVERED_COLOR, Color::THEME_BUTTON_HOVERED_TEXT_COLOR,
        5 * Constant::SCALE_Y, Color::THEME_BUTTON_OUTLINE_COLOR, Color::THEME_BUTTON_HOVERED_OUTLINE_COLOR
    );
    mSceneLayers[ThemeButton]->attachChild(std::move(themeButton));
}

void MainMenu::changeTheme() {
    if (!Color::IS_DARK_THEME) {
        Color::IS_DARK_THEME = true;
        Color::BACKGROUND_COLOR = sf::Color(44, 54, 57);
        Color::NODE_COLOR = sf::Color(44, 54, 57);
        Color::NODE_TEXT_COLOR = sf::Color::White;
        Color::NODE_OUTLINE_COLOR = sf::Color::White;
        Color::NODE_EDGE_COLOR = sf::Color::White;
        Color::NODE_LABEL_COLOR = sf::Color(232, 54, 0);
        Color::NODE_HIGHLIGHT_COLOR = sf::Color(255, 171, 25);
        Color::NODE_HIGHLIGHT_TEXT_COLOR = sf::Color(44, 54, 57);
        Color::NODE_HIGHLIGHT_OUTLINE_COLOR = sf::Color(255, 181, 25);
        Color::SETTINGS_BUTTON_COLOR = sf::Color(255, 244, 214);
        Color::SETTINGS_BUTTON_TEXT_COLOR = sf::Color::Black;
        Color::SETTINGS_BUTTON_OUTLINE_COLOR = sf::Color::Black;
        Color::SETTINGS_BUTTON_HOVERED_COLOR = sf::Color(224, 134, 7);
        Color::SETTINGS_BUTTON_HOVERED_TEXT_COLOR = sf::Color::Black;
        Color::SETTINGS_BUTTON_HOVERED_OUTLINE_COLOR = sf::Color::Black;

        Color::INPUTBOX_COLOR = sf::Color(44, 54, 57);
        Color::INPUTBOX_TEXT_COLOR = sf::Color::White;
        Color::INPUTBOX_OUTLINE_COLOR = sf::Color::White;
        Color::INPUTBOX_CURSOR_COLOR = sf::Color::White;


        Color::MATRIX_SLOT_COLOR = sf::Color(44, 54, 57);
        Color::MATRIX_SLOT_TEXT_COLOR = sf::Color::White;
        Color::MATRIX_SLOT_OUTLINE_COLOR = sf::Color::White;

        Color::CODEBLOCK_BOX_COLOR = sf::Color(255, 244, 214);
        Color::CODEBLOCK_TEXT_COLOR = sf::Color::Black;
        Color::CODEBLOCK_BOX_ACTIVATED_COLOR = sf::Color(255, 217, 115);
        Color::CODEBLOCK_TEXT_ACTIVATED_COLOR = sf::Color::Black;

        Color::DATA_STRUCTURE_BUTTON_OUTLINE_COLOR = sf::Color::White;
        Color::DATA_STRUCTURE_BUTTON_OUTLINE_HOVERED_COLOR = sf::Color(255, 171, 25);

        Color::ERROR_MESSAGE_BOX_COLOR = sf::Color(44, 54, 57);
        Color::ERROR_MESSAGE_BOX_TEXT_COLOR = sf::Color::White;
        Color::ERROR_MESSAGE_BOX_OUTLINE_COLOR = sf::Color::Transparent;

        Color::THEME_BUTTON_COLOR = sf::Color(44, 54, 57);
        Color::THEME_BUTTON_TEXT_COLOR = sf::Color::White;
        Color::THEME_BUTTON_OUTLINE_COLOR = sf::Color::White;
        Color::THEME_BUTTON_HOVERED_COLOR = sf::Color::White;
        Color::THEME_BUTTON_HOVERED_TEXT_COLOR = sf::Color(116, 155, 194, 255);
        Color::THEME_BUTTON_HOVERED_OUTLINE_COLOR = sf::Color::White;
        buildScene();
    }

    else {
        Color::IS_DARK_THEME = false;
        Color::BACKGROUND_COLOR = sf::Color::White;
        Color::NODE_COLOR = sf::Color::White;
        Color::NODE_TEXT_COLOR = sf::Color(116, 155, 194, 255);
        Color::NODE_OUTLINE_COLOR = sf::Color(116, 155, 194, 255);
        Color::NODE_EDGE_COLOR = sf::Color(116, 155, 194);
        Color::NODE_LABEL_COLOR = sf::Color(232, 54, 0);
        Color::NODE_HIGHLIGHT_COLOR = sf::Color(255, 171, 25);
        Color::NODE_HIGHLIGHT_TEXT_COLOR = sf::Color::White;
        Color::NODE_HIGHLIGHT_OUTLINE_COLOR = sf::Color(255, 181, 25);
        Color::SETTINGS_BUTTON_COLOR = sf::Color(145, 174, 226);
        Color::SETTINGS_BUTTON_TEXT_COLOR = sf::Color::Black;
        Color::SETTINGS_BUTTON_OUTLINE_COLOR = sf::Color::Black;
        Color::SETTINGS_BUTTON_HOVERED_COLOR = sf::Color(224, 134, 7);
        Color::SETTINGS_BUTTON_HOVERED_TEXT_COLOR = sf::Color::Black;
        Color::SETTINGS_BUTTON_HOVERED_OUTLINE_COLOR = sf::Color::Black;

        Color::INPUTBOX_COLOR = sf::Color::White;
        Color::INPUTBOX_TEXT_COLOR = sf::Color::Black;
        Color::INPUTBOX_OUTLINE_COLOR = sf::Color::Black;
        Color::INPUTBOX_CURSOR_COLOR = sf::Color::Black;


        Color::MATRIX_SLOT_COLOR = sf::Color::White;
        Color::MATRIX_SLOT_TEXT_COLOR = sf::Color::Black;
        Color::MATRIX_SLOT_OUTLINE_COLOR = sf::Color::Black;

        Color::CODEBLOCK_BOX_COLOR = sf::Color(145, 174, 226);
        Color::CODEBLOCK_TEXT_COLOR = sf::Color::Black;
        Color::CODEBLOCK_BOX_ACTIVATED_COLOR = sf::Color(86, 114, 163);
        Color::CODEBLOCK_TEXT_ACTIVATED_COLOR = sf::Color::Black;

        Color::DATA_STRUCTURE_BUTTON_OUTLINE_COLOR = sf::Color(116, 155, 194);
        Color::DATA_STRUCTURE_BUTTON_OUTLINE_HOVERED_COLOR = sf::Color(255, 171, 25);

        Color::ERROR_MESSAGE_BOX_COLOR = sf::Color::White;
        Color::ERROR_MESSAGE_BOX_TEXT_COLOR = sf::Color::Black;
        Color::ERROR_MESSAGE_BOX_OUTLINE_COLOR = sf::Color::Transparent;

        Color::THEME_BUTTON_COLOR = sf::Color::White;
        Color::THEME_BUTTON_TEXT_COLOR = sf::Color(116, 155, 194, 255);
        Color::THEME_BUTTON_OUTLINE_COLOR = sf::Color(116, 155, 194, 255);
        Color::THEME_BUTTON_HOVERED_COLOR = sf::Color(44, 54, 57);
        Color::THEME_BUTTON_HOVERED_TEXT_COLOR = sf::Color::White;
        Color::THEME_BUTTON_HOVERED_OUTLINE_COLOR = sf::Color::Black;
        buildScene();
    }
}



