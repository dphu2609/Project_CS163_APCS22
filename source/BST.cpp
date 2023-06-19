#include <State/BST.hpp>

BST::BST(sf::RenderWindow &window) : State(window) {
    loadTextures();
    loadFonts();
    buildScene();
}

BST::~BST() {
    clear(mRoot);
}

void BST::loadTextures() {}

void BST::loadFonts() {
    mFontsHolder.load(Fonts::FiraSansRegular, "resources/fonts/FiraSans-Regular.ttf");
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
    std::cout << mRoot->widthLeft << ' ' << mRoot->widthRight << std::endl;
}

BST::Node* BST::insert(Node *&root, int height, bool isLeft, const sf::Vector2f &parentPos, int data) {
    if (root == nullptr) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        sf::Vector2f position = parentPos;
        if (height != 1) {
            if (isLeft) position += sf::Vector2f(-NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL);
            else position += sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL);
        }
        node->set(std::to_string(data), mFontsHolder[Fonts::FiraSansRegular], position);
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

void BST::testAnimation() {
    int index = 0;
    for (auto &child : mSceneLayers[Nodes]->getChildren()) {
        if (!repeat) {
            child->zoom(sf::Vector2f(200, 0));
            child->change3Color(sf::Color(233, 102, 160), sf::Color(149, 117, 222), sf::Color(149, 117, 222));
        }
        else {
            child->zoom(sf::Vector2f(0, 0));
            child->change3Color(Color::NODE_COLOR, Color::NODE_TEXT_COLOR, Color::NODE_OUTLINE_COLOR);
        }
        if (child->isZoomFinished()) {
            child->resetAnimationVar();
            if (index == mSceneLayers[Nodes]->getChildren().size() - 1) {
                repeat = !repeat;
            }
        }
        index++;
    }
}
