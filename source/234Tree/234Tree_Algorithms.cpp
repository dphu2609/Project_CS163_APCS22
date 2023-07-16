#include <State/Tree234.hpp>

void Tree234::clear(Node *&root) {
    if (root == nullptr) return;
    for (int i = 0; i < 4; i++) {
        clear(root->child[i]);
    }
    delete root->tempLeft;
    delete root->tempRight;
    delete root;
    root = nullptr;
}

void Tree234::insert(Node* &root, std::vector<Node*> &nodeList, int data) {
    for (auto &node : nodeList) {
        if (node->val == data) {
            node->duplicate++;
            mOperationNode = node;
            return;
        }
    }
    insertNonDuplicateNode(root, nodeList, data);
}

bool Tree234::Node::isLeaf() {
    for (int i = 0; i < 4; i++) {
        if (child[i] != nullptr) return false;
    }
    return true;
}

Tree234::Node::Node(int data, std::vector<Node*> &nodeList) {
    val = data;
    numKeys = 1;
    depth = 1;
    nodeIndex = nodeList.size();
    tempLeft = nullptr;
    tempRight = nullptr;
    parent = nullptr;
    for (int i = 0; i < 4; i++) {
        child[i] = nullptr;
        isEdgeHighlighted[i] = false;
        orderOfNode[i] = false;
    }
    isNodeHighlighted = false;
    isInsertNode = false;
    isAttached = false;
    duplicate = 1;
}

void Tree234::insertNonDuplicateNode(Node *&root, std::vector<Node*> &nodeList, int data) {
    if (root == nullptr) {
        Node* newRoot = new Node(data, nodeList);
        root = newRoot;
        nodeList.push_back(newRoot);
        mOperationNode = newRoot;
        return;
    }

    Node* cur = root;
    Node* parent = root;
    int t = 0;
    while (cur) {
        if (cur->numKeys == 3) {
            splitNode(root, nodeList, cur);
        }
        if (cur->isLeaf()) {
            break;
        }
        parent = cur;
        cur = cur->child[getIndexNextMove(cur, data)];
    }

    if (cur) {
        insertInternalNode(cur, nodeList, data);
    }
}

Tree234::Node* Tree234::getInternalNode(Node* root, int data) {
    Node* cur = root;
    while (cur) {
        if (cur->isLeaf()) {
            return cur;
        }
        cur = cur->child[getIndexNextMove(cur, data)];
    }
    return nullptr;
}

Tree234::Node* Tree234::insertInternalNode(Node* &root, std::vector<Node*> &nodeList, int data) {
    Node* newNode = new Node(data, nodeList);
    newNode->isAttached = true;
    nodeList.push_back(newNode);
    if (root->numKeys == 2) {
        root->tempRight = newNode;
        if (newNode->val > root->val) {
            mOperationNode = root;
        }
        else if (newNode->val > root->tempLeft->val) {
            mOperationNode = root;
            std::swap(newNode->val, root->val);
        }
        else {
            mOperationNode = newNode;
            std::swap(newNode->val, root->val);
            std::swap(root->val, root->tempLeft->val);
        }
    }
    else if (root->numKeys == 1) {
        root->tempLeft = newNode;
        if (newNode->val > root->val) {
            mOperationNode = root;
            std::swap(newNode->val, root->val);
        }
        else {
            mOperationNode = newNode;
        }
    }
    root->numKeys++;
    newNode->parent = root;
    return newNode;
}

void Tree234::splitNode(Node* &root, std::vector<Node*> &nodeList, Node *&node) {
    node->tempLeft->isAttached = false;
    node->tempRight->isAttached = false;
    node->tempLeft->child[0] = node->child[0];
    node->tempLeft->child[1] = node->child[1];
    node->tempRight->child[0] = node->child[2];
    node->tempRight->child[1] = node->child[3];
    for (int i = 0; i < 4; i++) {
        if (node->child[i]) node->child[i]->orderOfNode[i] = false;
    }
    if (node->child[0]) {
        node->child[0]->parent = node->tempLeft;
        node->child[0]->orderOfNode[0] = true;
    }
    if (node->child[1]) {
        node->child[1]->parent = node->tempLeft;
        node->child[1]->orderOfNode[1] = true;
    }
    if (node->child[2]) {
        node->child[2]->parent = node->tempRight;
        node->child[2]->orderOfNode[0] = true;
    }
    if (node->child[3]) {
        node->child[3]->parent = node->tempRight;
        node->child[3]->orderOfNode[1] = true;
    }
    node->tempLeft->numKeys = 1;
    node->tempRight->numKeys = 1;
    if (node->parent) {
        node->tempLeft->parent = node->parent;
        node->tempRight->parent = node->parent;
        if (node->parent->numKeys == 2) {
            if (node->val > node->parent->val) {
                node->parent->child[3] = node->tempRight;
                node->parent->child[2] = node->tempLeft;
                node->tempRight->orderOfNode[3] = true;
                node->tempLeft->orderOfNode[2] = true;
                node->parent->tempRight = node;
            }
            else if (node->val > node->parent->tempLeft->val) {
                node->parent->child[3] = node->parent->child[2];
                node->parent->child[3]->orderOfNode[3] = true;
                node->parent->child[3]->orderOfNode[2] = false;
                node->parent->child[2] = node->tempRight;
                node->parent->child[1] = node->tempLeft;
                node->tempRight->orderOfNode[2] = true;
                node->tempLeft->orderOfNode[1] = true;
                node->parent->tempRight = node;
                std::swap(node->parent->val, node->val);    
            }
            else {
                node->parent->child[3] = node->parent->child[2];
                node->parent->child[3]->orderOfNode[3] = true;
                node->parent->child[3]->orderOfNode[2] = false;
                node->parent->child[2] = node->parent->child[1];
                node->parent->child[2]->orderOfNode[2] = true;
                node->parent->child[2]->orderOfNode[1] = false;
                node->parent->child[1] = node->tempRight;
                node->parent->child[0] = node->tempLeft;
                node->tempRight->orderOfNode[1] = true;
                node->tempLeft->orderOfNode[0] = true;
                node->parent->tempRight = node;
                std::swap(node->parent->val, node->val);
                std::swap(node->parent->val, node->parent->tempLeft->val);
            }
        }
        else if (node->parent->numKeys == 1) {
            if (node->val > node->parent->val) {
                node->parent->child[2] = node->tempRight;
                node->parent->child[1] = node->tempLeft;
                node->tempRight->orderOfNode[2] = true;
                node->tempLeft->orderOfNode[1] = true;
                node->parent->tempLeft = node;
                std::swap(node->parent->val, node->val);
            }
            else {
                node->parent->child[2] = node->parent->child[1];
                node->parent->child[2]->orderOfNode[2] = true;
                node->parent->child[2]->orderOfNode[1] = false;
                node->parent->child[1] = node->tempRight;
                node->parent->child[0] = node->tempLeft;
                node->tempRight->orderOfNode[1] = true;
                node->tempLeft->orderOfNode[0] = true;
                node->parent->tempLeft = node;
            }
        }
        for (int i = 0; i < 4; i++) {
            node->child[i] = nullptr;
            node->orderOfNode[i] = false;
        }
        node->numKeys = 1;
        node->isAttached = true;
        node->tempLeft = nullptr;
        node->tempRight = nullptr;
        node->parent->numKeys++;
    }
    else {
        node->child[0] = node->tempLeft;
        node->child[1] = node->tempRight;
        node->child[2] = nullptr;
        node->child[3] = nullptr;
        node->tempRight->orderOfNode[1] = true;
        node->tempLeft->orderOfNode[0] = true;
        node->tempLeft->parent = node;
        node->tempRight->parent = node;
        node->tempLeft->numKeys = 1;
        node->tempRight->numKeys = 1; 
        node->tempLeft = nullptr;
        node->tempRight = nullptr;
        node->numKeys = 1;
        root = node;
    }
    if (node->parent) node = node->parent;
}

void Tree234::moveTree(Node* root, bool isLeft) {
    if (root == nullptr) return;
    root->position += sf::Vector2f(isLeft ? -NODE_DISTANCE_HORIZONTAL : NODE_DISTANCE_HORIZONTAL, 0);
    if (root->tempLeft) root->tempLeft->position = root->position + sf::Vector2f(-2 * Size::NODE_RADIUS_X - Size::EDGE_THICKNESS, 0);
    if (root->tempRight) root->tempRight->position = root->position + sf::Vector2f(2 * Size::NODE_RADIUS_X + Size::EDGE_THICKNESS, 0);
    root->position.y = (root->depth - mRoot->depth) * NODE_DISTANCE_VERTICAL + 150 * Constant::SCALE_Y;
    for (int i = 0; i < 4; i++) {
        if (root->child[i] != nullptr) 
            moveTree(root->child[i], isLeft);
    }
}

int Tree234::getIndexNextMove(Node* root, int data) {
    if (root->tempLeft && data == root->tempLeft->val) return -1;
    else if (data == root->val) return -1;
    else if (root->tempRight && data == root->tempRight->val) return -1;
    if (root->numKeys == 3) {
        if (data < root->tempLeft->val) return 0;
        else if (data < root->val) return 1;
        else if (data < root->tempRight->val) return 2;
        else return 3;
    }
    else if (root->numKeys == 2) {
        if (data < root->tempLeft->val) return 0;
        else if (data < root->val) return 1;
        else return 2;
    }
    else if (root->numKeys == 1) {
        if (data < root->val) return 0;
        else return 1;
    }
    return 0;
}

void Tree234::balanceTree() {
    std::vector<Node*> nodeList;
    std::queue<Node*> q;
    q.push(mRoot);
    mRoot->depth = 1;
    while (!q.empty()) {
        Node* temp = q.front();
        q.pop();
        if (temp->isAttached) temp->depth = temp->parent->depth;
        else if (temp->parent) temp->depth = temp->parent->depth + 1;
        nodeList.push_back(temp);
        if (temp->tempLeft) q.push(temp->tempLeft);
        if (temp->tempRight) q.push(temp->tempRight);
        for (int i = 0; i < 4; i++) {
            if (temp->child[i]) q.push(temp->child[i]);
        }
    }

    for (auto &node : nodeList) {
        if (node == mRoot) {
            node->position = sf::Vector2f(Constant::WINDOW_WIDTH/2 - Size::NODE_RADIUS, 150 * Constant::SCALE_Y);
        }
        else {
            if (node->isAttached) {
                if (node->val < node->parent->val) node->position = node->parent->position + sf::Vector2f(-2*Size::NODE_RADIUS - Size::EDGE_THICKNESS, 0);
                else node->position = node->parent->position + sf::Vector2f(2 * Size::NODE_RADIUS + Size::EDGE_THICKNESS, 0);
                continue;
            }
            Node* cur = mRoot;
            while (cur) {
                if (cur == node) break;
                if (cur->child[0] && getIndexNextMove(cur, node->val) == 0) {
                    if (getIndexNextMove(cur->child[0], node->val) == cur->child[0]->numKeys) {
                        moveTree(cur->child[0], true);
                    }
                }
                else if (cur->child[1] && getIndexNextMove(cur, node->val) == 1) {
                    if (getIndexNextMove(cur->child[1], node->val) == 0) {
                        if (cur->numKeys == 3 || cur->numKeys == 2) moveTree(cur->child[0], true);
                        else if (cur->numKeys == 1) moveTree(cur->child[1], false);
                    }
                    else if (getIndexNextMove(cur->child[1], node->val) == cur->child[1]->numKeys) {
                        if (cur->numKeys == 3) {
                            moveTree(cur->child[1], true);
                            moveTree(cur->child[0], true);
                        }
                        else if (cur->numKeys == 2) {
                            moveTree(cur->child[2], false);
                        }
                    }
                }
                else if (cur->child[2] && getIndexNextMove(cur, node->val) == 2) {
                    if (getIndexNextMove(cur->child[2], node->val) == 0) {
                        moveTree(cur->child[2], false);
                        if (cur->numKeys == 3) moveTree(cur->child[3], false);
                    }
                    else if (getIndexNextMove(cur->child[2], node->val) == cur->child[2]->numKeys) {
                        if (cur->numKeys == 3) moveTree(cur->child[3], false);
                    }
                }
                else if (cur->child[3] && getIndexNextMove(cur, node->val) == 3) {
                    if (getIndexNextMove(cur->child[3], node->val) == 0) {
                        moveTree(cur->child[3], false);
                    }
                }
                cur = cur->child[getIndexNextMove(cur, node->val)];
            }
            if (node->orderOfNode[0]) {
                node->position = node->parent->position + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL);
                if (node->parent->numKeys == 3 || node->parent->numKeys == 2) {
                    if (node->numKeys == 3) node->position += sf::Vector2f(-NODE_DISTANCE_HORIZONTAL / 3, 0);
                }
            }
            else if (node->orderOfNode[1]) {
                if (node->parent->numKeys == 3) {
                    node->position = node->parent->position + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL / 3, NODE_DISTANCE_VERTICAL);
                    if (node->numKeys == 3 || node->numKeys == 2) {
                        node->position += sf::Vector2f(-NODE_DISTANCE_HORIZONTAL / 6, 0);
                        node->parent->child[0]->position += sf::Vector2f(-NODE_DISTANCE_HORIZONTAL / 3, 0);
                    }
                }
                else if (node->parent->numKeys == 2) {
                    node->position = node->parent->position + sf::Vector2f(0, NODE_DISTANCE_VERTICAL);
                    if (node->numKeys == 3 || node->numKeys == 2) {
                        node->parent->child[0]->position += sf::Vector2f(-NODE_DISTANCE_HORIZONTAL / 3, 0);
                        node->parent->child[2]->position += sf::Vector2f(NODE_DISTANCE_HORIZONTAL / 3, 0);
                    }
                    if (node->numKeys == 1 || node->numKeys == 3) {
                        node->position -= sf::Vector2f(Size::NODE_RADIUS + Size::EDGE_THICKNESS, 0);
                    }
                }
                else if (node->parent->numKeys == 1) node->position = node->parent->position + sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL);
            }
            else if (node->orderOfNode[2]) {
                if (node->parent->numKeys == 3) { 
                    node->position = node->parent->position + sf::Vector2f(NODE_DISTANCE_HORIZONTAL / 3, NODE_DISTANCE_VERTICAL);
                    if (node->numKeys == 3 || node->numKeys == 2) {
                        node->position += sf::Vector2f(NODE_DISTANCE_HORIZONTAL / 6, 0);
                        node->parent->child[3]->position += sf::Vector2f(NODE_DISTANCE_HORIZONTAL / 3, 0);
                    }
                }
                else if (node->parent->numKeys == 2) {
                    node->position = node->parent->position + sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL);
                    if (node->numKeys == 3 || node->numKeys == 2) {
                        node->position += sf::Vector2f(NODE_DISTANCE_HORIZONTAL / 3, 0);
                    }
                }
            }
            else if (node->orderOfNode[3]) {
                node->position = node->parent->position + sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL);
                if (node->numKeys == 3) node->position += sf::Vector2f(NODE_DISTANCE_HORIZONTAL / 3, 0);
            }
        }
    }
}

void Tree234::createRandomTree() {
    mInputData.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);

    for (int i = 0; i < mInputSize; i++) {
        mInputData.push_back(dis(gen));
    }
    if (mInputSize < 20) {
        NODE_DISTANCE_HORIZONTAL = 250.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 200.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 40.f * Constant::SCALE_X;
    }
    else if (mInputSize < 40) {
        NODE_DISTANCE_HORIZONTAL = 250.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 150.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 30.f * Constant::SCALE_X;
    }
    else {
        NODE_DISTANCE_HORIZONTAL = 200.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 100.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 28.f * Constant::SCALE_X;
    }
    clear(mRoot);
    mNodeList.clear();
    
    for (int i = 0; i < mInputData.size(); i++) {
        insert(mRoot, mNodeList, mInputData[i]);
    }

    balanceTree();
    createTree();
}

void Tree234::returnToPreviousStep() {
    resetNodeState();
    clear(mRoot);
    mNodeList.clear();
    mRoot = mTreeForBackward.top()->root;
    mOperationNode = mTreeForBackward.top()->nodeForOperation;
    mReplaceNode = mTreeForBackward.top()->nodeForReplace;
    mNodeList = mTreeForBackward.top()->nodeList;
    mAnimationStep = mTreeForBackward.top()->animationIndex;
    mTreeForBackward.pop();
    createTree();
}

Tree234::Node* Tree234::copyNodeProperties(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    Node* newNode = new Node;
    newNode->val = node->val;
    newNode->numKeys = node->numKeys;
    newNode->depth = node->depth;
    newNode->nodeIndex = node->nodeIndex;
    for (int i = 0; i < 4; i++) {
        newNode->edgeIndex[i] = node->edgeIndex[i];
        newNode->orderOfNode[i] = node->orderOfNode[i];
        newNode->isEdgeHighlighted[i] = node->isEdgeHighlighted[i];
        newNode->child[i] = nullptr;
    }
    newNode->isAttached = node->isAttached;
    newNode->position = node->position;
    newNode->isNodeHighlighted = node->isNodeHighlighted;
    newNode->isInsertNode = node->isInsertNode;
    newNode->duplicate = node->duplicate;
    newNode->parent = nullptr;
    newNode->tempLeft = nullptr;
    newNode->tempRight = nullptr;
    return newNode;
}

Tree234::Node* Tree234::copyTree234(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }   
    
    Node* newNode = copyNodeProperties(root);
    if (root->tempLeft) {
        Node* newTempLeft = copyNodeProperties(root->tempLeft);
        newNode->tempLeft = newTempLeft;
        newTempLeft->parent = newNode;
    }

    if (root->tempRight) {
        Node* newTempRight = copyNodeProperties(root->tempRight);
        newNode->tempRight = newTempRight;
        newTempRight = root->tempRight;
    }

    for (int i = 0; i < 4; i++) {
        newNode->child[i] = copyTree234(root->child[i]);
        if (newNode->child[i]) {
            newNode->child[i]->parent = newNode;
        }
    }
    return newNode;
}

void Tree234::getTravelPath(Node* root, int data) {
    mTravelPath.clear();
    Node* cur = root;
    while (cur) {
        mTravelPath.push_back(std::make_pair(cur, getIndexNextMove(cur, data)));
        if (getIndexNextMove(cur, data) == -1) {
            break;
        }
        cur = cur->child[getIndexNextMove(cur, data)];
    }
    mTravelIndex = 0;
}

void Tree234::getSplitCheckpoint(Node* root, int data) {
    mSplitCheckpoint.clear();
    mSplitCheckpointIndex = 0;
    Node* cur = root;
    while (cur) {
        if (cur->numKeys == 3) {
            mSplitCheckpoint.push_back(cur);
        }
        if (getIndexNextMove(cur, data) == -1) {
            break;
        }
        cur = cur->child[getIndexNextMove(cur, data)];
    }
}