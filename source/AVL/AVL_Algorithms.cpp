#include <State/AVL.hpp>

void AVL::insert(Node* &root, std::vector<Node*> &nodeList, int data, bool isNeedRotating) {
    for (auto &node : nodeList) {
        if (node->val == data) {
            node->duplicate++;
            mOperationNode = node;
            return;
        }
    }
    insertNonDuplicateNode(root, nodeList, nullptr, data, isNeedRotating);
}

AVL::Node* AVL::insertNonDuplicateNode(Node *&root, std::vector<Node*> &nodeList, Node* parent, int data, bool isNeedRotating) {
    if (root == nullptr) {
        if (parent == nullptr) {
            root = new Node{
                data, 1, 1, 0,(int)nodeList.size(), false, 
                sf::Vector2f(Constant::WINDOW_WIDTH/2 - Size::NODE_RADIUS, 150 * Constant::SCALE_Y), 
                nullptr, nullptr, nullptr, 1
            };
        }
        else {
            bool isLeft = data < parent->val;
            root = new Node{
                data, parent->depth + 1, 1, 0,(int)nodeList.size(), isLeft, 
                sf::Vector2f(parent->position.x + (isLeft ? -NODE_DISTANCE_HORIZONTAL : NODE_DISTANCE_HORIZONTAL), parent->position.y + NODE_DISTANCE_VERTICAL), 
                nullptr, nullptr, parent, 1
            };
        }
        nodeList.push_back(root);
        mOperationNode = root;
        return root;
    }
    if (data < root->val) {
        root->left = insertNonDuplicateNode(root->left, nodeList, root, data, isNeedRotating);
    }
    else if (data > root->val) {
        root->right = insertNonDuplicateNode(root->right, nodeList, root, data, isNeedRotating);
    }
    
    if (isNeedRotating) {
        root->balanceFactor = getHeight(root->right) - getHeight(root->left);
        root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
        if (root->balanceFactor > 1) {
            if (data > root->right->val) {
                return root = rotateLeft(root);
            } else {
                root->right = rotateRight(root->right);
                return root = rotateLeft(root);
            }
        }
        else if (root->balanceFactor < -1) {
            if (data < root->left->val) {
                return root = rotateRight(root);
            } else {
                root->left = rotateLeft(root->left);
                return root = rotateRight(root);
            }
        }
    }
    return root;
}

int AVL::getHeight(Node* root) {
    if (root == nullptr) return 0;
    return root->height;
}

AVL::Node* AVL::rotateLeft(Node* &root) {
    Node* newRoot = root->right;
    Node* newChild = newRoot->left;

    newRoot->parent = root->parent;

    newRoot->left = root;
    root->parent = newRoot;

    root->right = newChild;
    if (newChild) newChild->parent = root;

    newRoot->isLeft = root->isLeft;
    if (newChild) newChild->isLeft = false;
    root->isLeft = true;
    
    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    newRoot->height = std::max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

    std::swap(mNodeList[root->nodeIndex], mNodeList[newRoot->nodeIndex]);
    for (int i = 0; i < mNodeList.size(); i++) mNodeList[i]->nodeIndex = i;

    return newRoot;
}

AVL::Node* AVL::rotateRight(Node* &root) {
    Node* newRoot = root->left;
    Node* newChild = newRoot->right;

    newRoot->parent = root->parent;

    root->left = newChild;
    if (newChild) newChild->parent = root;

    newRoot->right = root;
    root->parent = newRoot;

    newRoot->isLeft = root->isLeft;
    if (newChild) newChild->isLeft = true;
    root->isLeft = false;

    // Update heights and balance factors
    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    newRoot->height = std::max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

    std::swap(mNodeList[root->nodeIndex], mNodeList[newRoot->nodeIndex]);
    for (int i = 0; i < mNodeList.size(); i++) mNodeList[i]->nodeIndex = i;

    return newRoot;
}

void AVL::moveTree(Node* root, bool isLeft) {
    if (root == nullptr) return;
    sf::Vector2f dis;
    dis = sf::Vector2f(isLeft ? -NODE_DISTANCE_HORIZONTAL : NODE_DISTANCE_HORIZONTAL, 0);
    root->position += dis;
    root->position.y = (root->depth - mRoot->depth) * NODE_DISTANCE_VERTICAL + 150 * Constant::SCALE_Y;
    moveTree(root->left, isLeft);
    moveTree(root->right, isLeft);
}

void AVL::balanceTree() {
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
            node->depth = 1;
        }
        else {
            Node* cur = mRoot;
            int depth = 1;
            while (cur) {
                if (node->val != cur->val) node->depth = ++depth;
                if (node->val < cur->val) {
                    if (cur->left && node->val > cur->left->val) moveTree(cur->left, true);
                    cur = cur->left;
                }
                else if (node->val > cur->val) {
                    if (cur->right && node->val < cur->right->val) moveTree(cur->right, false);
                    cur = cur->right;
                }
                else {
                    if (node->isLeft) node->position = node->parent->position + sf::Vector2f(-NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL);
                    else node->position = node->parent->position + sf::Vector2f(NODE_DISTANCE_HORIZONTAL, NODE_DISTANCE_VERTICAL); 
                    break;
                }
            }
        }
    }
}

void AVL::clear(Node *&root) {
    if (root == nullptr) return;
    clear(root->left);
    clear(root->right);
    delete root;
    root = nullptr;
}

void AVL::createRandomTree() {
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

    balanceTree();

    createTree();
}

void AVL::getTravelPath(Node* root, int data) {
    mTravelPath.clear();
    mIsLeftPath.clear();
    Node* cur = root;
    while (cur != nullptr) {
        mTravelPath.push_back(cur);
        mIsLeftPath.push_back(data < cur->val);
        if (data < cur->val) cur = cur->left;
        else if (data > cur->val) cur = cur->right;
        else break;
    }
    if (!mIsReversed) mTravelIndex = 0;
    else mTravelIndex = mTravelPath.size() - 1;
}

void AVL::find2NodesForDelete(int data) {
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
    mOperationValue = mOperationNode->val;
    mOperationIndex = mOperationNode->nodeIndex;
    if (mReplaceNode) {
        mReplaceValue = mReplaceNode->val;
        mReplaceIndex = mReplaceNode->nodeIndex;
    }
    else {
        mReplaceValue = -1;
        mReplaceIndex = -1;
    }
}

void AVL::deleteNode() {
    if (mOperationNode == nullptr) return;
    if (mReplaceNode) {
        mOperationNode->val = mReplaceNode->val;
        mOperationNode->position = mNodeListForBackup[mOperationIndex]->position;
        if (mOperationNode != mReplaceNode->parent) {
            if (mReplaceNode->isLeft) {
                mReplaceNode->parent->left = mReplaceNode->right;
                if (mReplaceNode->right) {
                    mReplaceNode->right->parent = mReplaceNode->parent;
                    mReplaceNode->right->isLeft = true;
                }
            }
            else {
                mReplaceNode->parent->right = mReplaceNode->left;
                if (mReplaceNode->left) {
                    mReplaceNode->left->parent = mReplaceNode->parent;
                    mReplaceNode->left->isLeft = false;
                }
            }
        }
        else {
            if (mReplaceNode->isLeft) {
                mReplaceNode->parent->left = mReplaceNode->left;
                if (mReplaceNode->left) {
                    mReplaceNode->left->parent = mReplaceNode->parent;
                }
            }
            else {
                mReplaceNode->parent->right = mReplaceNode->right;
                if (mReplaceNode->right) {
                    mReplaceNode->right->parent = mReplaceNode->parent;
                }
            }
        }
        std::swap(mSceneLayers[Nodes]->getChildren()[mOperationIndex], mSceneLayers[Nodes]->getChildren()[mReplaceIndex]);
        mSceneLayers[Nodes]->getChildren().erase(mSceneLayers[Nodes]->getChildren().begin() + mReplaceIndex);
        mSceneLayers[LeftEdges]->getChildren().erase(mSceneLayers[LeftEdges]->getChildren().begin() + mReplaceIndex); 
        mSceneLayers[RightEdges]->getChildren().erase(mSceneLayers[RightEdges]->getChildren().begin() + mReplaceIndex);
        mNodeList.erase(mNodeList.begin() + mReplaceIndex);
        for (int i = 0; i < mNodeList.size(); i++) {
            mNodeList[i]->nodeIndex = i;
        }
        delete mReplaceNode;
    }
    else {
        if (mOperationNode->isLeft) mOperationNode->parent->left = nullptr;
        else mOperationNode->parent->right = nullptr;
        mSceneLayers[Nodes]->getChildren().erase(mSceneLayers[Nodes]->getChildren().begin() + mOperationIndex);
        mSceneLayers[LeftEdges]->getChildren().erase(mSceneLayers[LeftEdges]->getChildren().begin() + mOperationIndex);
        mSceneLayers[RightEdges]->getChildren().erase(mSceneLayers[RightEdges]->getChildren().begin() + mOperationIndex);
        mNodeList.erase(mNodeList.begin() + mOperationIndex);
        for (int i = 0; i < mNodeList.size(); i++) {
            mNodeList[i]->nodeIndex = i;
        }
        delete mOperationNode;
    }
    balanceTree();
}

AVL::Node* AVL::getRotateNode() {
    updateHeightAndBalanceFactor(mRoot);
    Node* result = nullptr;
    int maxDepth = 0;
    for (auto &child : mNodeList) {
        if (child->balanceFactor > 1 || child->balanceFactor < -1) {
            if (child->depth > maxDepth) {
                maxDepth = child->depth;
                result = child;
            }
        }
    }
    return result;
}

void AVL::updateHeightAndBalanceFactor(Node*& root) {
    if (root == nullptr) return;
    updateHeightAndBalanceFactor(root->left);
    updateHeightAndBalanceFactor(root->right);
    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    root->balanceFactor = getHeight(root->right) - getHeight(root->left);
}

void AVL::changeLink() {
    if (mReplaceNode) {
        mOperationNode->position = mReplaceNode->position;
        if (mReplaceNode->isLeft && mReplaceNode->right) {
            mReplaceNode->position = mReplaceNode->right->position;
        }
        else if (mReplaceNode->left) {
            mReplaceNode->position = mReplaceNode->left->position;
        }
    }
    else if (mOperationNode->parent) mOperationNode->position = mOperationNode->parent->position;
}

void AVL::changeLinkReversed() {
    if (mReplaceNode) {
        mOperationNode->position = mNodeListForBackup[mOperationIndex]->position;
    }
}

void AVL::deleteNodeReversed() {
    restoreTree();
    std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
    node->set(
        std::to_string(mOperationNode->val), mOperationNode->position, 0,
        Color::NODE_COLOR, Color::NODE_TEXT_COLOR, Color::NODE_OUTLINE_COLOR
    );
    std::unique_ptr<Edge> leftEdge = std::make_unique<Edge>();
    if (mReplaceNode)
        leftEdge->set(mReplaceNode->position, mReplaceNode->position);
    else 
        leftEdge->set(mOperationNode->position, mOperationNode->position);

    std::unique_ptr<Edge> rightEdge = std::make_unique<Edge>();
    if (mReplaceNode)
        rightEdge->set(mReplaceNode->position, mReplaceNode->position);
    else 
        rightEdge->set(mOperationNode->position, mOperationNode->position);
    if (mReplaceNode != nullptr) {
        mSceneLayers[Nodes]->getChildren().insert(mSceneLayers[Nodes]->getChildren().begin() + mReplaceIndex, std::move(node));
        mSceneLayers[LeftEdges]->getChildren().insert(mSceneLayers[LeftEdges]->getChildren().begin() + mReplaceIndex, std::move(leftEdge));
        mSceneLayers[RightEdges]->getChildren().insert(mSceneLayers[RightEdges]->getChildren().begin() + mReplaceIndex, std::move(rightEdge));
        std::swap(mSceneLayers[Nodes]->getChildren()[mOperationIndex], mSceneLayers[Nodes]->getChildren()[mReplaceIndex]); 
        mOperationNode->position = mReplaceNode->position;
    }
    else {
        mSceneLayers[Nodes]->getChildren().insert(mSceneLayers[Nodes]->getChildren().begin() + mOperationIndex, std::move(node));
        mSceneLayers[LeftEdges]->getChildren().insert(mSceneLayers[LeftEdges]->getChildren().begin() + mOperationIndex, std::move(leftEdge));
        mSceneLayers[RightEdges]->getChildren().insert(mSceneLayers[RightEdges]->getChildren().begin() + mOperationIndex, std::move(rightEdge));
    }
}

void AVL::createBackupTree() {
    mNodeListForBackup.clear();
    clear(mRootForBackup);
    for (int i = 0; i < mNodeList.size(); i++) {
        insert(mRootForBackup, mNodeListForBackup, mNodeList[i]->val);
    }
}

void AVL::restoreTree() {
    int indexOfOperationNode = mOperationIndex;
    int indexOfReplaceNode = mReplaceIndex;
    clear(mRoot);
    mNodeList.clear();
    for (int i = 0; i < mNodeListForBackup.size(); i++) {
        insert(mRoot, mNodeList, mNodeListForBackup[i]->val);
    }
    mOperationNode = ((indexOfOperationNode == -1 || indexOfOperationNode >= mNodeList.size()) ? nullptr : mNodeList[indexOfOperationNode]);
    mReplaceNode = ((indexOfReplaceNode == -1 || indexOfReplaceNode >= mNodeList.size()) ? nullptr : mNodeList[indexOfReplaceNode]);
    balanceTree();
}