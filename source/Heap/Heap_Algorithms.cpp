#include <State/Heap.hpp>

void Heap::clear(Node *&root) {
    if (root == nullptr) return;
    clear(root->left);
    clear(root->right);
    delete root;
    root = nullptr;
}

void Heap::insert(Node* &root, std::vector<Node*> &nodeList, int data, bool isNeedCorrectingHeap) {
    if (root == nullptr) {
        root = new Node{
            data, 1, 0, 1, 
            sf::Vector2f(0, 0), 
            nullptr, nullptr, nullptr, false, false
        };
        nodeList.push_back(root);
        return;
    }
    Node* cur = root;
    int index = nodeList.size();
    if (index % 2 == 0) index = (index - 2) / 2;
    else index = (index - 1) / 2;
    Node* newNode = new Node{
        data, nodeList[index]->depth + 1, (int)nodeList.size(), (int)(nodeList.size() + 1),
        sf::Vector2f(0, 0), nullptr, nullptr, nodeList[index], false, false
    };
    nodeList.push_back(newNode);
    if (newNode->order % 2 == 0) {
        nodeList[index]->left = newNode;
    }
    else {
        nodeList[index]->right = newNode;
    }
    if (isNeedCorrectingHeap) {
        if (mIsMaxHeap) {
            while (newNode->parent && newNode->val > newNode->parent->val) {
                std::swap(newNode->val, newNode->parent->val);
                newNode = newNode->parent;
            }
        }
        else {
            while (newNode->parent && newNode->val < newNode->parent->val) {
                std::swap(newNode->val, newNode->parent->val);
                newNode = newNode->parent;
            }
        }
    }
}

void Heap::moveTree(Node* root, bool isLeft) {
    if (root == nullptr) return;
    root->position += sf::Vector2f(isLeft ? -NODE_DISTANCE_HORIZONTAL : NODE_DISTANCE_HORIZONTAL, 0);
    root->position.y = (root->depth - mRoot->depth) * NODE_DISTANCE_VERTICAL + 150 * Constant::SCALE_Y;
    moveTree(root->left, isLeft);
    moveTree(root->right, isLeft);
}

void Heap::balanceTree() {
    if (!mRoot) return;
    std::vector<Node*> nodeList;
    std::queue<Node*> q;
    q.push(mRoot);
    while (!q.empty()) {
        Node* temp = q.front();
        q.pop();
        nodeList.push_back(temp);
        if (temp->left) q.push(temp->left);
        if (temp->right) q.push(temp->right);
    }

    for (auto &node : nodeList) {
        if (node == mRoot) {
            node->position = sf::Vector2f(Constant::WINDOW_WIDTH/2 - Size::NODE_RADIUS, 150 * Constant::SCALE_Y);
        }
        else {
            Node* cur = mRoot;
            std::vector<int> travelIndex = getTravelIndex(node->order);
            for (int i = 1; i < travelIndex.size() - 1; i++) {
                if (travelIndex[i] % 2 == 0) {
                    if (travelIndex[i + 1] % 2 != 0) moveTree(cur->left, true);
                    cur = cur->left;
                }
                else {
                    if (travelIndex[i + 1] % 2 == 0) moveTree(cur->right, false);
                    cur = cur->right;
                }
            }
            if (node->order %2 == 0) 
                node->position = node->parent->position + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL);
            else 
                node->position = node->parent->position + sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL);
        }
    }
}

void Heap::setTreeScale(int treeSize) {
    if (treeSize < 16) {
        NODE_DISTANCE_HORIZONTAL = 150.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 100.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 40.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 40.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 40.f * Constant::SCALE_Y;
    }
    else if (treeSize < 20) {
        NODE_DISTANCE_HORIZONTAL = 120.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 100.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 35.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 35.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 35.f * Constant::SCALE_Y;
    }
    else if (treeSize < 32) {
        NODE_DISTANCE_HORIZONTAL = 80.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 80.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 30.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 30.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 30.f * Constant::SCALE_Y;
    }
    else if (treeSize < 40) {
        NODE_DISTANCE_HORIZONTAL = 60.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 80.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 30.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 30.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 30.f * Constant::SCALE_Y;
    }
    else if (treeSize < 64) {
        NODE_DISTANCE_HORIZONTAL = 43.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 80.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 30.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 30.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 30.f * Constant::SCALE_Y;
    }
    else {
        NODE_DISTANCE_HORIZONTAL = 25.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 80.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 20.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 20.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 20.f * Constant::SCALE_Y;
    }
}

void Heap::createRandomTree() {
    mInputData.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 199);

    for (int i = 0; i < mInputSize; i++) {
        mInputData.push_back(dis(gen));
    }

    clear(mRoot);
    mNodeList.clear();

    for (int i = 0; i < mInputData.size(); i++) {
        insert(mRoot, mNodeList, mInputData[i]);
    }

    setTreeScale(mInputSize);
    balanceTree();
    createTree();
}

Heap::Node* Heap::copyHeap(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }
    
    Node* newNode = copyNodeProperties(root);

    newNode->left = copyHeap(root->left);
    if (newNode->left != nullptr) {
        newNode->left->parent = newNode;
    }

    newNode->right = copyHeap(root->right);
    if (newNode->right != nullptr) {
        newNode->right->parent = newNode;
    }

    return newNode;
}


std::vector<int> Heap::getTravelIndex(int index) {
    std::vector<int> travelIndex;
    while (index >= 1) {
        travelIndex.push_back(index);
        index /= 2;
    }
    std::sort(travelIndex.begin(), travelIndex.end());
    return travelIndex;
}

Heap::Node* Heap::findNode(Node* root, int index) {
    std::vector<int> travelIndex = getTravelIndex(index);
    Node* cur = root;
    for (int i = 1; i < travelIndex.size(); i++) {
        if (travelIndex[i] % 2 == 0) cur = cur->left;
        else cur = cur->right;
    }
    return cur;
}

Heap::Node* Heap::copyNodeProperties(Node* node) {
    Node* newNode = new Node;
    newNode->val = node->val;
    newNode->depth = node->depth;
    newNode->order = node->order;
    newNode->nodeIndex = node->nodeIndex;
    newNode->position = node->position;
    newNode->isInsertNode = node->isInsertNode;
    newNode->isNodeHighlighted = node->isNodeHighlighted;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = nullptr;
    return newNode;
}

void Heap::createBackupTree() {
    mNodeListForBackup.clear();
    clear(mRootForBackup);
    mRootForBackup = copyHeap(mRoot);
    for (auto &child : mNodeList) {
        mNodeListForBackup.push_back(findNode(mRootForBackup, child->order));
    }
}

void Heap::restoreTree() {
    mNodeList.clear();
    clear(mRoot);
    mRoot = copyHeap(mRootForBackup);
    for (auto &child : mNodeListForBackup) {
        mNodeList.push_back(findNode(mRoot, child->order));
    }
}

Heap::TreeState* Heap::createTreeState(int animationIndex) {
    Node* newRoot = copyHeap(mRoot);
    Node* nodeForOperation = nullptr;
    if (mOperationNode) nodeForOperation = findNode(newRoot, mOperationNode->order);
    std::vector<Node*> newNodeList;
    for (auto &child : mNodeList) {
        newNodeList.push_back(findNode(newRoot, child->order));
    }
    TreeState* newTreeState = new TreeState{newRoot, nodeForOperation, newNodeList, animationIndex};
    return newTreeState;
}

void Heap::returnToPreviousStep() {
    if (mTreeForBackward.empty()) return;
    resetNodeState();
    clear(mRoot);
    mNodeList.clear();
    mRoot = mTreeForBackward.top()->root;
    mOperationNode = mTreeForBackward.top()->nodeForOperation;
    mNodeList = mTreeForBackward.top()->nodeList;
    mAnimationStep = mTreeForBackward.top()->animationIndex;
    mTreeForBackward.pop();
    createTree();
}