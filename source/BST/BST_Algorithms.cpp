#include <State/BST.hpp>

void BST::insert(int data) {
    for (auto &node : mNodeList) {
        if (node->val == data) {
            node->duplicate++;
            mOperationNode = node;
            return;
        }
    }
    insertNonDuplicateNode(mRoot, nullptr, data);
}

BST::Node* BST::insertNonDuplicateNode(Node *&root, Node* parent, int data) {
    if (root == nullptr) {
        if (parent == nullptr) {
            root = new Node{
                data, 1, (int)mNodeList.size(), false, 
                sf::Vector2f(Constant::WINDOW_WIDTH/2 - Size::NODE_RADIUS, 150 * Constant::SCALE_Y), 
                nullptr, nullptr, nullptr, 1
            };
        }
        else {
            bool isLeft = data < parent->val;
            root = new Node{
                data, parent->height + 1, (int)mNodeList.size(), isLeft, 
                sf::Vector2f(parent->position.x + (isLeft ? -NODE_DISTANCE_HORIZONTAL : NODE_DISTANCE_HORIZONTAL), parent->position.y + NODE_DISTANCE_VERTICAL), 
                nullptr, nullptr, parent, 1
            };
        }
        mNodeList.push_back(root);
        mOperationNode = root;
        return root;
    }
    if (data < root->val) {
        if (root->left != nullptr && data > root->left->val) moveTree(root->left, true);
        root->left = insertNonDuplicateNode(root->left, root, data);
    }
    else if (data > root->val) {
        if (root->right != nullptr && data < root->right->val) moveTree(root->right, false);
        root->right = insertNonDuplicateNode(root->right, root, data);
    }
    return root;
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

void BST::createRandomTree() {
    mInputData.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);

    for (int i = 0; i < mInputSize; i++) {
        mInputData.push_back(dis(gen));
    }

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
        NODE_DISTANCE_HORIZONTAL = 38.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 80.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 30.f * Constant::SCALE_X;
    }
    else {
        NODE_DISTANCE_HORIZONTAL = 35.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 80.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 28.f * Constant::SCALE_X;
    }

    clear(mRoot);
    mNodeList.clear();

    for (int i = 0; i < mInputData.size(); i++) {
        insert(mInputData[i]);
    }


    createTree();
}

void BST::getTravelPath(Node* root, int data) {
    Node* cur = root;
    while (cur != nullptr) {
        mTravelPath.push_back(cur);
        mIsLeftPath.push_back(data < cur->val);
        if (data < cur->val) cur = cur->left;
        else if (data > cur->val) cur = cur->right;
        else break;
    }
}

void BST::find2NodesForDelete(int data) {
    Node* cur = mRoot;
    while (cur != nullptr) {
        if (data < cur->val) {
            cur = cur->left;
        }
        else if (data > cur->val) {
            cur = cur->right;
        }
        else {
            mOperationNode = cur;
            break;
        }
    }
    if (cur == nullptr) return;
    if (!cur->left && !cur->right) mReplaceNode = nullptr;
    else if (!cur->left) {
        mReplaceNode = cur->right;
    }
    else if (!cur->right) {
        mReplaceNode = cur->left;
    }
    else {
        mReplaceNode = cur->right;
        while (mReplaceNode->left != nullptr) {
            mReplaceNode = mReplaceNode->left;
        }
    }
}

void BST::swapNode(Node* &a, Node* &b) {
    std::swap(a->val, b->val);
    std::swap(a->height, b->height);
    std::swap(a->duplicate, b->duplicate);
    std::swap(a->position, b->position);
    std::swap(a->nodeIndex, b->nodeIndex);
    
    std::swap(mSceneLayers[Nodes]->getChildren()[a->nodeIndex], mSceneLayers[Nodes]->getChildren()[b->nodeIndex]);
    std::swap(mSceneLayers[LeftEdges]->getChildren()[a->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[b->nodeIndex]);
    std::swap(mSceneLayers[RightEdges]->getChildren()[a->nodeIndex], mSceneLayers[RightEdges]->getChildren()[b->nodeIndex]);
}