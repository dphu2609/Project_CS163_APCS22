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
    // testAnimation();
}

void BST::handleEvent(sf::Event &event) {
    mSceneGraph.handleEvent(mWindow, event);
}

void BST::buildScene() {
    for (int i = 0; i < LayerCount; i++) {
        SceneNode::Ptr layer = std::make_unique<SceneNode>();
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
    for (int i = 0; i < sampleData.size(); i++) {
        insert(mRoot, 1, 0, sf::Vector2f(Constant::WINDOW_WIDTH/2 - NODE_RADIUS, 150 * Constant::SCALE_Y), sampleData[i]);
    }
    std::unique_ptr<RectangleButton> createButton = std::make_unique<RectangleButton>();
    createButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(100, Constant::WINDOW_HEIGHT - 500), "Create", 
        ResourcesHolder::fontsHolder[Fonts::FiraSansRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::White
    );
    mSceneLayers[Buttons]->attachChild(std::move(createButton));

    std::unique_ptr<InputBox> inputBox = std::make_unique<InputBox>();
    inputBox->set(sf::Vector2f(100, Constant::WINDOW_HEIGHT - 300));
    mSceneLayers[InputBoxes]->attachChild(std::move(inputBox));
}

BST::Node* BST::insert(Node *&root, int height, bool isLeft, const sf::Vector2f &parentPos, int data) {
    if (root == nullptr) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        sf::Vector2f position = parentPos;
        if (height != 1) {
            if (isLeft) position += sf::Vector2f(-NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL);
            else position += sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL);
        }
        node->set(std::to_string(data), position);
        mSceneLayers[Nodes]->attachChild(std::move(node));
        root = new Node{data, height, 0, 0, position, mNodeCount, mEdgeLeftCount, mEdgeRightCount, isLeft, nullptr, nullptr};
        mNodeCount++;
        if (height != 1) {
            if (isLeft) mEdgeLeftCount++;
            else mEdgeRightCount++;
        }
        return root;
    }
    if (data < root->val) {
        root->widthLeft++;
        if (root->left == nullptr) {
            std::unique_ptr<Edge> edgeLeft = std::make_unique<Edge>();
            edgeLeft->setPositionByPoints(root->position + sf::Vector2f(NODE_RADIUS, NODE_RADIUS), sf::Vector2f(root->position.x + NODE_DISTANCE_HORIZONTAL * Constant::SCALE_X, root->position.y - NODE_DISTANCE_VERTICAL) + sf::Vector2f(NODE_RADIUS, NODE_RADIUS));
            root->edgeLeftIndex = mSceneLayers[LeftEdges]->getChildren().size();
            mSceneLayers[LeftEdges]->attachChild(std::move(edgeLeft));
        } else if (data > root->left->val) {
            mSceneLayers[LeftEdges]->getChildren()[root->edgeLeftIndex]->setPositionByPoints(root->position + sf::Vector2f(NODE_RADIUS, NODE_RADIUS), root->left->position + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL, 0) + sf::Vector2f(NODE_RADIUS, NODE_RADIUS));
            moveTree(root->left, sf::Vector2f(-NODE_DISTANCE_HORIZONTAL, 0));
        }
        root->left = insert(root->left, height + 1, 1, root->position, data);
    } else if (data > root->val) {
        root->widthRight++;
        if (root->right == nullptr) {
            std::unique_ptr<Edge> edgeRight = std::make_unique<Edge>();
            edgeRight->setPositionByPoints(root->position + sf::Vector2f(NODE_RADIUS, NODE_RADIUS), sf::Vector2f(root->position.x + NODE_DISTANCE_HORIZONTAL, root->position.y + NODE_DISTANCE_VERTICAL) + sf::Vector2f(NODE_RADIUS, NODE_RADIUS));
            root->edgeRightIndex = mSceneLayers[RightEdges]->getChildren().size();
            mSceneLayers[RightEdges]->attachChild(std::move(edgeRight));
        } else if (data < root->right->val) {
            mSceneLayers[RightEdges]->getChildren()[root->edgeRightIndex]->setPositionByPoints(root->position + sf::Vector2f(NODE_RADIUS, NODE_RADIUS), root->right->position + sf::Vector2f(NODE_DISTANCE_HORIZONTAL, 0) + sf::Vector2f(NODE_RADIUS, NODE_RADIUS));
            moveTree(root->right, sf::Vector2f(NODE_DISTANCE_HORIZONTAL, 0));
        }
        root->right = insert(root->right, height + 1, 0, root->position, data);
    }

    return root;
}

void BST::moveTree(Node* root, const sf::Vector2f &dis) {
    if (root == nullptr) return;
    mSceneLayers[Nodes]->getChildren()[root->nodeIndex]->setPosition(root->position + dis);
    if (root->left) mSceneLayers[LeftEdges]->getChildren()[root->edgeLeftIndex]->setPositionByPoints(root->position + dis + sf::Vector2f(NODE_RADIUS, NODE_RADIUS), root->left->position + dis + sf::Vector2f(NODE_RADIUS, NODE_RADIUS));
    if (root->right)  mSceneLayers[RightEdges]->getChildren()[root->edgeRightIndex]->setPositionByPoints(root->position + dis + sf::Vector2f(NODE_RADIUS, NODE_RADIUS), root->right->position + dis + sf::Vector2f(NODE_RADIUS, NODE_RADIUS));
    root->position += dis;
    moveTree(root->left, dis);
    moveTree(root->right, dis);
}

void BST::clear(Node *&root) {
    if (root == nullptr) return;
    clear(root->left);
    clear(root->right);
    delete root;
    root = nullptr;
}

std::vector<int> BST::getTravelPath(int data) {
    std::vector<int> nodeIndex = {};
    getEdgeTravelPath.clear();
    Node* root = mRoot;
    while (root != nullptr) {
        nodeIndex.push_back(root->nodeIndex);
        if (data < root->val) {
            getEdgeTravelPath.push_back({true, root->edgeLeftIndex});
            root = root->left;
        }
        else if (data > root->val) {
            getEdgeTravelPath.push_back({false, root->edgeRightIndex});
            root = root->right;
        }
        else {
            break;
        }
    }
    return nodeIndex;
}

void BST::testAnimation() { 
    if (isReset) {
        for (auto &child : mSceneLayers[Nodes]->getChildren()) child->change3Color(Color::NODE_COLOR, Color::NODE_TEXT_COLOR, Color::NODE_OUTLINE_COLOR, 3);
        for (auto &child : mSceneLayers[LeftEdges]->getChildren()) child->change1Color(Color::NODE_EDGE_COLOR, 3);
        for (auto &child : mSceneLayers[RightEdges]->getChildren()) child->change1Color(Color::NODE_EDGE_COLOR, 3);
        if (mSceneLayers[Nodes]->getChildren()[0]->isChange3ColorFinished()) {
            isReset = false;
            indexTravel = 0;
            for (int i = 0; i < LayerCount; i++) mSceneLayers[i]->resetAnimationVar();
        }
        return;
    }
    if (path.empty()) path = getTravelPath(292);
    if (!isNodeHighlight1) mSceneLayers[Nodes]->getChildren()[path[indexTravel]]->change3Color(sf::Color(250, 160, 42), sf::Color(255, 255, 255), sf::Color(250, 160, 42), 3);
    else if (!isNodeHighlight2) mSceneLayers[Nodes]->getChildren()[path[indexTravel]]->change3Color(sf::Color(255, 255, 255), sf::Color(250, 160, 42), sf::Color(250, 160, 42), 3);
    else if (indexTravel < getEdgeTravelPath.size()) {
        if (getEdgeTravelPath[indexTravel].first) mSceneLayers[LeftEdges]->getChildren()[getEdgeTravelPath[indexTravel].second]->change1Color(sf::Color(250, 160, 42), 3);
        else mSceneLayers[RightEdges]->getChildren()[getEdgeTravelPath[indexTravel].second]->change1Color(sf::Color(250, 160, 42), 3);
    }
    if (mSceneLayers[Nodes]->getChildren()[path[indexTravel]]->isChange3ColorFinished()) {
        if (!isNodeHighlight1) {
            isNodeHighlight1 = true;
            mSceneLayers[Nodes]->getChildren()[path[indexTravel]]->resetAnimationVar();
        }
        else if (!isNodeHighlight2) {
            isNodeHighlight2 = true;
            mSceneLayers[Nodes]->getChildren()[path[indexTravel]]->resetAnimationVar();
        }
    }
    else if (mSceneLayers[LeftEdges]->getChildren()[getEdgeTravelPath[indexTravel].second]->isChange1ColorFinished() || mSceneLayers[RightEdges]->getChildren()[getEdgeTravelPath[indexTravel].second]->isChange1ColorFinished()) {
        isNodeHighlight1 = false;
        isNodeHighlight2 = false;
        if (getEdgeTravelPath[indexTravel].first) mSceneLayers[LeftEdges]->getChildren()[getEdgeTravelPath[indexTravel].second]->resetAnimationVar();
        else mSceneLayers[RightEdges]->getChildren()[getEdgeTravelPath[indexTravel].second]->resetAnimationVar();
        if (indexTravel < path.size() - 1) indexTravel++;
        else {
            isReset = true;
        }
    }
}


// optimize code later

