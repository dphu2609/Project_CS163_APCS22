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

void BST::balanceTree() {
    for (auto &node : mNodeList) {
        if (node == mRoot) {
            node->position = sf::Vector2f(Constant::WINDOW_WIDTH/2 - Size::NODE_RADIUS, 150 * Constant::SCALE_Y);
            node->height = 1;
        }
        else {
            Node* cur = mRoot;
            int height = 1;
            while (cur) {
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
                node->height = ++height;
            }
        }
    }
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

void BST::deleteNode() {
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

void BST::changeLink() {
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

void BST::changeLinkReversed() {
    if (mReplaceNode) {
        mOperationNode->position = mNodeListForBackup[mOperationIndex]->position;
    }
}

void BST::deleteNodeReversed() {
    restoreTree();
    std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
    node->set(
        std::to_string(mOperationNode->val), mOperationNode->position, 0,
        sf::Color::White, sf::Color(255, 171, 25), sf::Color(255, 171, 25)
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
    mNodeListForBackup.clear();
    clear(mRootForBackup);
    for (int i = 0; i < mNodeList.size(); i++) {
        insert(mRootForBackup, mNodeListForBackup, mNodeList[i]->val);
    }
}

void BST::restoreTree() {
    int indexOfOperationNode = mOperationIndex;
    int indexOfReplaceNode = mReplaceIndex;
    clear(mRoot);
    mNodeList.clear();
    for (int i = 0; i < mNodeListForBackup.size(); i++) {
        insert(mRoot, mNodeList, mNodeListForBackup[i]->val);
    }
    mOperationNode = ((indexOfOperationNode == -1 || indexOfOperationNode >= mNodeList.size()) ? nullptr : mNodeList[indexOfOperationNode]);
    mReplaceNode = ((indexOfReplaceNode == -1 || indexOfReplaceNode >= mNodeList.size()) ? nullptr : mNodeList[indexOfReplaceNode]);
}