#include <State/BST.hpp>

void BST::insert(Node* &root, std::vector<Node*> &nodeList, int data) {
    for (auto &node : nodeList) {
        if (node->val == data) {
            node->duplicate++;
            mOperationNode = node;
            return;
        }
    }
    insertNonDuplicateNode(root, nodeList, nullptr, data);
}

BST::Node* BST::insertNonDuplicateNode(Node *&root, std::vector<Node*> &nodeList, Node* parent, int data) {
    if (root == nullptr) {
        if (parent == nullptr) {
            root = new Node{
                data, 1, (int)nodeList.size(), false, 
                sf::Vector2f(Constant::WINDOW_WIDTH/2 - Size::NODE_RADIUS, 150 * Constant::SCALE_Y), 
                nullptr, nullptr, nullptr, 1
            };
        }
        else {
            bool isLeft = data < parent->val;
            root = new Node{
                data, parent->height + 1, (int)nodeList.size(), isLeft, 
                sf::Vector2f(parent->position.x + (isLeft ? -NODE_DISTANCE_HORIZONTAL : NODE_DISTANCE_HORIZONTAL), parent->position.y + NODE_DISTANCE_VERTICAL), 
                nullptr, nullptr, parent, 1
            };
        }
        nodeList.push_back(root);
        mOperationNode = root;
        return root;
    }
    if (data < root->val) {
        if (root->left != nullptr && data > root->left->val) moveTree(root->left, true);
        root->left = insertNonDuplicateNode(root->left, nodeList, root, data);
    }
    else if (data > root->val) {
        if (root->right != nullptr && data < root->right->val) moveTree(root->right, false);
        root->right = insertNonDuplicateNode(root->right, nodeList, root, data);
    }
    return root;
}

void BST::moveTree(Node* root, bool isLeft) {
    if (root == nullptr) return;
    sf::Vector2f dis;
    dis = sf::Vector2f(isLeft ? -NODE_DISTANCE_HORIZONTAL : NODE_DISTANCE_HORIZONTAL, 0);
    root->position += dis;
    root->position.y = (root->height - mRoot->height) * NODE_DISTANCE_VERTICAL + 150 * Constant::SCALE_Y;
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
        insert(mRoot, mNodeList, mInputData[i]);
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
    mOperationNode = nullptr;
    mReplaceNode = nullptr;
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
    if (cur == nullptr || mOperationNode->duplicate > 1) return;
    if (!cur->left && !cur->right) mReplaceNode = nullptr;
    else if (!cur->right) {
        mReplaceNode = cur->left;
        while (mReplaceNode->right != nullptr) {
            mReplaceNode = mReplaceNode->right;
        }
    }
    else {
        mReplaceNode = cur->right;
        while (mReplaceNode->left != nullptr) {
            mReplaceNode = mReplaceNode->left;
        }
    }
}

void BST::deleteNode() {
    if (mOperationNode == nullptr) return;
    if (mReplaceNode != nullptr) reduceHeight(mReplaceNode);
    Node* cur = mRoot;
    int data = (mReplaceNode == nullptr ? mOperationNode->val : mReplaceNode->val);
    while (cur != nullptr) { //move tree
        if (data < cur->val) {
            if (cur->left != nullptr && data > cur->left->val) moveTree(cur->left, false); 
            cur = cur->left;
        }
        else if (data > cur->val) {
            if (cur->right != nullptr && data < cur->right->val) moveTree(cur->right, true);
            cur = cur->right;
        }
        else {
            break;
        }
    }
    if (mReplaceNode && mReplaceNode->parent == mOperationNode && mOperationNode->isLeft == mReplaceNode->isLeft) {
        if (mOperationNode->isLeft) moveTree(mReplaceNode, false);
        else moveTree(mReplaceNode, true);
    }
    else if (mReplaceNode && mReplaceNode->parent == mOperationNode && mOperationNode->isLeft != mReplaceNode->isLeft) {
        if (mOperationNode->isLeft) moveTree(mReplaceNode, true);
        else moveTree(mReplaceNode, false);
    }

    if (mOperationNode == nullptr) return;
    if (mReplaceNode == nullptr) {
        if (mOperationNode->parent) {
            if (mOperationNode->isLeft) mOperationNode->parent->left = nullptr;
            else mOperationNode->parent->right = nullptr;
        }
    }
    else if (mReplaceNode->parent == mOperationNode) {
        if (mOperationNode->left && mOperationNode->right) {
            mOperationNode->left->parent = mReplaceNode;
            mReplaceNode->left = mOperationNode->left;
            if (mOperationNode->parent) {
                if (mOperationNode->isLeft) mOperationNode->parent->left = mReplaceNode;
                else mOperationNode->parent->right = mReplaceNode;
            }
            mReplaceNode->parent = mOperationNode->parent;
            mReplaceNode->isLeft = mOperationNode->isLeft;
            mReplaceNode->position = mOperationNode->position;
            mReplaceNode->height = mOperationNode->height;
            std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mReplaceNode->nodeIndex]);
            std::swap(mSceneLayers[RightEdges]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mReplaceNode->nodeIndex]);
            std::swap(mNodeList[mOperationNode->nodeIndex], mNodeList[mReplaceNode->nodeIndex]);
            std::swap(mOperationNode->nodeIndex, mReplaceNode->nodeIndex);
        }
        else {
            mReplaceNode->parent = mOperationNode->parent;
            if (mOperationNode->parent) {
                if (mOperationNode->isLeft) mOperationNode->parent->left = mReplaceNode;
                else mOperationNode->parent->right = mReplaceNode;
            }
            mReplaceNode->isLeft = mOperationNode->isLeft;
            // mReplaceNode->position = mOperationNode->position;
            mReplaceNode->height = mOperationNode->height;
            std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mReplaceNode->nodeIndex]);
            std::swap(mSceneLayers[LeftEdges]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[mReplaceNode->nodeIndex]);
            std::swap(mSceneLayers[RightEdges]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mReplaceNode->nodeIndex]);
            std::swap(mNodeList[mOperationNode->nodeIndex], mNodeList[mReplaceNode->nodeIndex]);
            std::swap(mOperationNode->nodeIndex, mReplaceNode->nodeIndex);
        }
    }
    else {
        if (mOperationNode->parent) {
            if (mOperationNode->isLeft) mOperationNode->parent->left = mReplaceNode;
            else mOperationNode->parent->right = mReplaceNode;
        }
        if (mReplaceNode->right) {
            mReplaceNode->right->parent = mReplaceNode->parent;
            mReplaceNode->parent->left = mReplaceNode->right;
            mReplaceNode->right->isLeft = mReplaceNode->isLeft;
        }
        else if (mReplaceNode->left) {
            mReplaceNode->left->parent = mReplaceNode->parent;
            mReplaceNode->parent->right = mReplaceNode->left;
            mReplaceNode->left->isLeft = mReplaceNode->isLeft;
        }
        else {
            if (mReplaceNode->isLeft) mReplaceNode->parent->left = nullptr;
            else mReplaceNode->parent->right = nullptr;
        }
        mReplaceNode->parent = mOperationNode->parent;
        mReplaceNode->left = mOperationNode->left;
        mReplaceNode->right = mOperationNode->right;
        if (mOperationNode->left) mOperationNode->left->parent = mReplaceNode;
        if (mOperationNode->right) mOperationNode->right->parent = mReplaceNode;
        mReplaceNode->position = mOperationNode->position;
        mReplaceNode->height = mOperationNode->height;
        mReplaceNode->isLeft = mOperationNode->isLeft;
        std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mReplaceNode->nodeIndex]);
        std::swap(mNodeList[mOperationNode->nodeIndex], mNodeList[mReplaceNode->nodeIndex]);
        std::swap(mOperationNode->nodeIndex, mReplaceNode->nodeIndex);
    }
    for (int i = mOperationNode->nodeIndex + 1; i < mNodeList.size(); i++) {
        mNodeList[i]->nodeIndex--;
    }
    mNodeList.erase(mNodeList.begin() + mOperationNode->nodeIndex);
    mSceneLayers[Nodes]->getChildren().erase(mSceneLayers[Nodes]->getChildren().begin() + mOperationNode->nodeIndex);
    mSceneLayers[LeftEdges]->getChildren().erase(mSceneLayers[LeftEdges]->getChildren().begin() + mOperationNode->nodeIndex);
    mSceneLayers[RightEdges]->getChildren().erase(mSceneLayers[RightEdges]->getChildren().begin() + mOperationNode->nodeIndex);
    if (mOperationNode == mRoot) mRoot = mReplaceNode;
    delete mOperationNode;
}

void BST::reduceHeight(Node* root) {
    if (root == nullptr) return;    
    root->height--;
    reduceHeight(root->left);
    reduceHeight(root->right);
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

void BST::createBackupTree() {
    clear(mRootForBackup);
    for (int i = 0; i < mNodeList.size(); i++) {
        insert(mRootForBackup, mNodeListForBackup, mNodeList[i]->val);
    }
}

void BST::restoreTree() {
    int indexOfOperationNode = (mOperationNode ? mOperationNode->nodeIndex : -1);
    int indexOfReplaceNode = (mReplaceNode ? mReplaceNode->nodeIndex : -1);
    clear(mRoot);
    mNodeList.clear();
    for (int i = 0; i < mNodeListForBackup.size(); i++) {
        insert(mRoot, mNodeList, mNodeListForBackup[i]->val);
    }
    mOperationNode = ((indexOfOperationNode == -1 || indexOfOperationNode >= mNodeList.size()) ? nullptr : mNodeList[indexOfOperationNode]);
    mReplaceNode = ((indexOfOperationNode == -1 || indexOfOperationNode >= mNodeList.size()) ? nullptr : mNodeList[indexOfReplaceNode]);
}