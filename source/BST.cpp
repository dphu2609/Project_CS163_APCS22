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
        createTree();
        if (mSceneLayers[CreateOptions]->getChildren()[0]->isActive())    
            mSceneLayers[CreateOptions]->getChildren()[0]->deactivate();
        else 
            mSceneLayers[CreateOptions]->getChildren()[0]->activate();
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
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50, Constant::WINDOW_HEIGHT - 600), "Create", 
        ResourcesHolder::fontsHolder[Fonts::FiraSansRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(createButton));

    std::unique_ptr<RectangleButton> insertButton = std::make_unique<RectangleButton>();
    insertButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50, Constant::WINDOW_HEIGHT - 500), "Insert", 
        ResourcesHolder::fontsHolder[Fonts::FiraSansRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(insertButton));

    std::unique_ptr<RectangleButton> deleteButton = std::make_unique<RectangleButton>();
    deleteButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50, Constant::WINDOW_HEIGHT - 400), "Delete", 
        ResourcesHolder::fontsHolder[Fonts::FiraSansRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(deleteButton));

    std::unique_ptr<RectangleButton> updateButton = std::make_unique<RectangleButton>();
    updateButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50, Constant::WINDOW_HEIGHT - 300), "Update", 
        ResourcesHolder::fontsHolder[Fonts::FiraSansRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(updateButton));

    std::unique_ptr<RectangleButton> searchButton = std::make_unique<RectangleButton>();
    searchButton->set(
        Size::SETTINGS_BUTTON_SIZE, sf::Vector2f(50, Constant::WINDOW_HEIGHT - 200), "Search", 
        ResourcesHolder::fontsHolder[Fonts::FiraSansRegular], sf::Color(46, 196, 0), sf::Color::Black,
        sf::Color(224, 134, 7), sf::Color::Black
    );
    mSceneLayers[Buttons]->attachChild(std::move(searchButton));   

    std::unique_ptr<RectangleButton> sizeButton = std::make_unique<RectangleButton>();
    sizeButton->set(
        sf::Vector2f(100 * Constant::SCALE_X, 100 * Constant::SCALE_Y), sf::Vector2f(150 + Size::SETTINGS_BUTTON_SIZE.x, Constant::WINDOW_HEIGHT - 600), "N =",
        ResourcesHolder::fontsHolder[Fonts::FiraSansRegular], sf::Color::Transparent, sf::Color::Black,
        sf::Color::Transparent, sf::Color::Black
    );
    sizeButton->deactivate();
    mSceneLayers[CreateOptions]->attachChild(std::move(sizeButton)); 
}

BST::Node* BST::insert(Node *&root, Node* parent, int data) {
    if (root == nullptr) {
        if (parent == nullptr) {
            root = new Node{
                data, 1, (int)mNodeList.size(), false, 
                sf::Vector2f(Constant::WINDOW_WIDTH/2 - NODE_RADIUS, 150 * Constant::SCALE_Y), 
                nullptr, nullptr, nullptr
            };
        }
        else {
            bool isLeft = data < parent->val;
            root = new Node{
                data, parent->height + 1, (int)mNodeList.size(), isLeft, 
                sf::Vector2f(parent->position.x + (isLeft ? -NODE_DISTANCE_HORIZONTAL : NODE_DISTANCE_HORIZONTAL), parent->position.y + NODE_DISTANCE_VERTICAL), 
                nullptr, nullptr, parent
            };
        }
        mNodeList.push_back(root);
        return root;
    }
    if (data < root->val) {
        if (root->left != nullptr && data > root->left->val) moveTree(root->left, true);
        root->left = insert(root->left, root, data);
    }
    else if (data > root->val) {
        if (root->right != nullptr && data < root->right->val) moveTree(root->right, false);
        root->right = insert(root->right, root, data);
    }
    return root;
}

void BST::createTree() {
    clear(mRoot);
    mNodeList.clear();
    for (int i = 0; i < sampleData.size(); i++) {
        insert(mRoot, nullptr, sampleData[i]);
    }
    for (int i = 0; i < mNodeList.size(); i++) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        node->set(std::to_string(mNodeList[i]->val), mNodeList[i]->position);
        mSceneLayers[Nodes]->attachChild(std::move(node));

        std::unique_ptr<Edge> leftEdge = std::make_unique<Edge>();
        leftEdge->setPositionByPoints(sf::Vector2f(mNodeList[i]->position.x + NODE_RADIUS, mNodeList[i]->position.y + NODE_RADIUS), (mNodeList[i]->left ? sf::Vector2f(mNodeList[i]->left->position.x + NODE_RADIUS, mNodeList[i]->left->position.y + NODE_RADIUS) : sf::Vector2f(mNodeList[i]->position.x + NODE_RADIUS, mNodeList[i]->position.y + NODE_RADIUS)));
        mSceneLayers[LeftEdges]->attachChild(std::move(leftEdge));

        std::unique_ptr<Edge> rightEdge = std::make_unique<Edge>();
        rightEdge->setPositionByPoints(sf::Vector2f(mNodeList[i]->position.x + NODE_RADIUS, mNodeList[i]->position.y + NODE_RADIUS), (mNodeList[i]->right ? sf::Vector2f(mNodeList[i]->right->position.x + NODE_RADIUS, mNodeList[i]->right->position.y + NODE_RADIUS) : sf::Vector2f(mNodeList[i]->position.x + NODE_RADIUS, mNodeList[i]->position.y + NODE_RADIUS)));
        mSceneLayers[RightEdges]->attachChild(std::move(rightEdge));
    }
}

void BST::moveTree(Node* root, bool isLeft) {
    if (root == nullptr) return;
    sf::Vector2f dis = sf::Vector2f(isLeft ? -NODE_DISTANCE_HORIZONTAL : NODE_DISTANCE_HORIZONTAL, 0);
    root->position += dis;
    moveTree(root->left, isLeft);
    moveTree(root->right, isLeft);
}

void BST::clear(Node *&root) {
    if (root == nullptr) return;
    clear(root->left);
    clear(root->right);
    delete root;
    root = nullptr;
}



