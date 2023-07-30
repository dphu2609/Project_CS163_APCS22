#include <State/Trie.hpp>

void Trie::clear(Node *&root) {
    if (root == nullptr) return;
    for (auto &node : root->child) {
        clear(node);
    }
    delete root;
    root = nullptr;
}

void Trie::insert(Node* &root, std::vector<Node*> &nodeList, std::string word) {
    if (root == nullptr) {
        Node* newNode = new Node{' ', 0, 0, {}, -1, sf::Vector2f(0, 0), {}, nullptr, false, false};
        root = newNode;
        nodeList.push_back(newNode);
    }

    Node* cur = root;
    int index = 0;
    while (index < word.size()) {
        bool isFound = false;
        for (int i = 0; i < cur->child.size(); i++) {
            if (cur->child[i]->val == word[index]) {
                cur = cur->child[i];
                index++;
                isFound = true;
                break;
            }
        }
        if (!isFound)
            break;
    }

    if (index == word.size()) {
        cur->isEndOfWord = true;
        return;
    }

    for (int i = index; i < word.size(); i++) {
        Node* newNode = new Node{word[i], cur->depth + 1, (int)nodeList.size(), {}, (int)cur->child.size(), sf::Vector2f(0, 0), {}, cur, false, false};
        nodeList.push_back(newNode);
        cur->child.push_back(newNode);
        cur = newNode;
        if (i == word.size() - 1) {
            newNode->isEndOfWord = true;
        }
    }
}

void Trie::add1Node(Node *&root, std::vector<Node*> &nodeList, std::string word, int &currentIndex) {
    Node* newNode = new Node{word[currentIndex], root->depth + 1, (int)nodeList.size(), {}, (int)root->child.size(), sf::Vector2f(0, 0), {}, root, false, true};
    if (newNode->orderOfNode == 0) {
        newNode->position = newNode->parent->position + sf::Vector2f(-((int)newNode->parent->child.size() - 1) * NODE_DISTANCE_HORIZONTAL / 2, NODE_DISTANCE_VERTICAL);
    }
    else {
        newNode->position = newNode->parent->child[0]->position + sf::Vector2f((newNode->orderOfNode) * NODE_DISTANCE_HORIZONTAL, 0);
    }
    nodeList.push_back(newNode);
    root->child.push_back(newNode);
    if (currentIndex == word.size() - 1) {
        newNode->isEndOfWord = true;
    }
    currentIndex++;
    
    std::unique_ptr<TreeNode> trieNode = std::make_unique<TreeNode>();
    std::string content = "a";
    content[0] = newNode->val;
    if (!newNode->isEndOfWord) {
        trieNode->set(true, 
            content, newNode->position, 0, 
            Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
        );
    }
    else {
        trieNode->set(true, 
            content, newNode->position, 0, 
            Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
        );
    }
    mSceneLayers[Nodes]->attachChild(std::move(trieNode));

    root->edgeIndex.push_back(mSceneLayers[Edges]->getChildren().size());
    std::unique_ptr<Edge> edge = std::make_unique<Edge>();
    edge->set(
        root->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), root->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
        Size::EDGE_THICKNESS, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
    );
    mSceneLayers[Edges]->attachChild(std::move(edge));
    mIsEdgeHighlighted.push_back(true);
    root = newNode;
}

void Trie::getTravelPath(std::string word) {
    mTravelPath.clear();
    mTravelIndex = 0;
    Node* cur = mRoot;
    int index = 0;
    while (index < word.size()) {
        bool isFound = false;
        for (int i = 0; i < cur->child.size(); i++) {
            if (cur->child[i]->val == word[index]) {
                mTravelPath.push_back({cur, i});
                cur = cur->child[i];
                index++;
                isFound = true;
                break;
            }
        }
        if (!isFound)
            break;
    }
    mTravelPath.push_back({cur, -1});
}

void Trie::moveTree(Node* root, bool isLeft) {
    if (root == nullptr) return;
    root->position += sf::Vector2f(isLeft ? -NODE_DISTANCE_HORIZONTAL / 2 : NODE_DISTANCE_HORIZONTAL / 2, 0);
    root->position.y = (root->depth - mRoot->depth) * NODE_DISTANCE_VERTICAL + 150 * Constant::SCALE_Y;
    for (auto &node : root->child) {
        moveTree(node, isLeft);
    }
}

int Trie::isLeftMidRight(Node* node) {
    if (!node->parent) return -1;
    if ((int)node->parent->child.size() % 2 == 0) {
        if (node->orderOfNode < (int)node->parent->child.size() / 2) {
            return -1;
        }
        else {
            return 1;
        }
    }
    else {
        if (node->orderOfNode < (int)node->parent->child.size() / 2) {
            return -1;
        }
        else if (node->orderOfNode > (int)node->parent->child.size() / 2) {
            return 1;
        }
        else {
            return 0;
        }
    }
}

void Trie::balanceTree() {
    if (mRoot == nullptr) return;
    std::vector<Node*> nodeList;
    std::queue<Node*> q;
    q.push(mRoot);
    while (!q.empty()) {
        Node* temp = q.front();
        q.pop();
        nodeList.push_back(temp);
        for (auto &node : temp->child) {
            q.push(node);
        }
    }

    for (auto &node : nodeList) {
        if (node == mRoot) {
            node->position = sf::Vector2f(Constant::WINDOW_WIDTH/2 - Size::NODE_RADIUS, 150 * Constant::SCALE_Y);
        }
        else {
            if (node->orderOfNode == 0) {
                node->position = node->parent->position + sf::Vector2f(-((int)node->parent->child.size() - 1) * NODE_DISTANCE_HORIZONTAL / 2, NODE_DISTANCE_VERTICAL);
            }
            else {
                node->position = node->parent->child[0]->position + sf::Vector2f((node->orderOfNode) * NODE_DISTANCE_HORIZONTAL, 0);
            }
        }
    }

    for (auto &node : nodeList) {
        if (node->isEndOfWord && node->child.empty()) {
            Node* cur = node;
            std::vector<Node*> path;
            while (cur != mRoot) {
                path.push_back(cur);
                cur = cur->parent;
            }
            std::reverse(path.begin(), path.end());
            std::vector<Node*> nodeNeedMoving;
            nodeNeedMoving.push_back(path[0]);
            for (int i = 1; i < path.size(); i++) {
                if (path[i - 1]->child.size() == 1) continue;
                if (isLeftMidRight(path[i]) == -1) {
                    for (int k = 0; k < nodeNeedMoving.size(); k++) {
                        if (isLeftMidRight(nodeNeedMoving[k]) == 1) {
                            for (int j = nodeNeedMoving[k]->orderOfNode; j < nodeNeedMoving[k]->parent->child.size(); j++) {
                                moveTree(nodeNeedMoving[k]->parent->child[j], false);
                            }
                        }
                        else {
                            for (int j = 0; j < nodeNeedMoving[k]->orderOfNode; j++) {
                                moveTree(nodeNeedMoving[k]->parent->child[j], true);
                            }
                        }
                    }
                }
                else if (isLeftMidRight(path[i]) == 1) { 
                    for (int k = 0; k < nodeNeedMoving.size(); k++) {
                        if (isLeftMidRight(nodeNeedMoving[k]) == -1) {
                            for (int j = 0; j <= nodeNeedMoving[k]->orderOfNode; j++) {
                                moveTree(nodeNeedMoving[k]->parent->child[j], true);
                            }
                        }
                        else {
                            for (int j = nodeNeedMoving[k]->orderOfNode + 1; j < nodeNeedMoving[k]->parent->child.size(); j++) {
                                moveTree(nodeNeedMoving[k]->parent->child[j], false);
                            }
                        }
                    }
                }
                else {
                    for (int k = 0; k < nodeNeedMoving.size(); k++) {
                        for (int j = 0; j < nodeNeedMoving[k]->orderOfNode; j++) {
                            moveTree(nodeNeedMoving[k]->parent->child[j], true);
                        }
                        for (int j = nodeNeedMoving[k]->orderOfNode + 1; j < nodeNeedMoving[k]->parent->child.size(); j++) {
                            moveTree(nodeNeedMoving[k]->parent->child[j], false);
                        }
                    }
                }
                nodeNeedMoving.push_back(path[i]);
            }
        }
    }

    for (auto &node : nodeList) {
        if (node == mRoot) continue;
        for (int k = 0; k < (int)node->child.size() / 2 - 1; k++) {
            for (int j = 0; j < node->orderOfNode; j++) {
                moveTree(node->parent->child[j], true);
            }
            for (int j = node->orderOfNode + 1; j < node->parent->child.size(); j++) {
                moveTree(node->parent->child[j], false);
            }
        }
    }
}

Trie::Node* Trie::copyNodeProperties(Node* root) {
    Node* newNode = new Node;
    newNode->val = root->val;
    newNode->depth = root->depth;
    newNode->orderOfNode = root->orderOfNode;
    newNode->position = root->position;
    newNode->isEndOfWord = root->isEndOfWord;
    newNode->nodeIndex = root->nodeIndex;
    newNode->isNodeHighlighted = root->isNodeHighlighted;
    for (int i = 0; i < root->edgeIndex.size(); i++) {
        newNode->edgeIndex.push_back(root->edgeIndex[i]);
    }
    newNode->parent = nullptr;
    newNode->child = {};
    return newNode;
}

Trie::Node* Trie::copyTrie(Node* root) {
    if (root == nullptr) return nullptr;
    Node* newNode = copyNodeProperties(root);
    for (int i = 0; i < root->child.size(); i++) {
        newNode->child.push_back(copyTrie(root->child[i]));
        newNode->child[i]->parent = newNode;
    }
    return newNode;
}

void Trie::getNodeList(Node* root, std::vector<Node*> &nodeList) {
    if (root == nullptr) return;
    std::queue<Node*> q;
    std::vector<Node*> nodeListTemp;
    q.push(root);
    while (!q.empty()) {
        Node* temp = q.front();
        q.pop();
        nodeListTemp.push_back(temp);
        for (auto &node : temp->child) {
            q.push(node);
        }
    }
    int currentIndex = 0;
    for (int i = 0; i < nodeListTemp.size(); i++) {
        for (auto &node : nodeListTemp) {
            if (node->nodeIndex == currentIndex) {
                nodeList.push_back(node);
                currentIndex++;
                break;
            }
        }
    }
}

bool Trie::isProcessingAnimation() {
    for (auto &child : mSceneLayers[Nodes]->getChildren()) 
        if (child->isProcessing()) return true;
    for (auto &child : mSceneLayers[Edges]->getChildren())
        if (child->isProcessing()) return true;
    return false;
}

Trie::TreeState* Trie::createTreeState(int animationIndex) {
    Node* newRoot = copyTrie(mRoot);
    std::vector<Node*> newNodeList = {};
    getNodeList(newRoot, newNodeList);
    Node* nodeForOperation = nullptr;
    if (mOperationNode) nodeForOperation = newNodeList[mOperationNode->nodeIndex];
    int operationIndex = mOperationIndex;
    std::vector<bool> isEdgeHighlighted;
    for (int i = 0; i < mIsEdgeHighlighted.size(); i++) {
        isEdgeHighlighted.push_back(mIsEdgeHighlighted[i]);
    }
    TreeState* newTreeState = new TreeState{newRoot, nodeForOperation, operationIndex, isEdgeHighlighted, newNodeList, animationIndex};
    return newTreeState;
}

void Trie::returnToPreviousStep() {
    resetNodeState();
    clear(mRoot);
    mNodeList.clear();
    mRoot = mTreeForBackward.top()->root;
    mOperationNode = mTreeForBackward.top()->nodeForOperation;
    mOperationIndex = mTreeForBackward.top()->operationIndex;
    mIsEdgeHighlighted = mTreeForBackward.top()->isEdgeHighlighted;
    mNodeList = mTreeForBackward.top()->nodeList;
    mAnimationStep = mTreeForBackward.top()->animationIndex;
    mTreeForBackward.pop();
    balanceTree();
    createTree();
}

void Trie::createBackupTree() {
    clear(mRootForBackup);
    mNodeListForBackup.clear();
    mRootForBackup = copyTrie(mRoot);
    getNodeList(mRootForBackup, mNodeListForBackup);
}

void Trie::restoreTree() {
    clear(mRoot);
    mNodeList.clear();
    mRoot = copyTrie(mRootForBackup);
    getNodeList(mRoot, mNodeList);
}

void Trie::setTreeScale(int treeSize) {
    if (treeSize < 16) {
        NODE_DISTANCE_HORIZONTAL = 120.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 100.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 40.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 40.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 40.f * Constant::SCALE_Y;
    }
    else if (treeSize < 31) {
        NODE_DISTANCE_HORIZONTAL = 70.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 100.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 25.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 25.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 25.f * Constant::SCALE_Y;
    }
    else {
        NODE_DISTANCE_HORIZONTAL = 60.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 100.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 20.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 20.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 20.f * Constant::SCALE_Y;
    }
}

void Trie::createRandomTree() {
    mInputData.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> wordLength(3, 8);
    std::uniform_int_distribution<> word(0, 25);

    for (int i = 0; i < mInputSize; i++) {
        std::string str = "";
        for (int j = 0; j < wordLength(gen); j++) {
            str += (char)(word(gen) + 'a');
        }
        mInputData.push_back(str);
    }

    clear(mRoot);   
    mNodeList.clear();
    mIsEdgeHighlighted.clear();

    for (int i = 0; i < mInputSize; i++) {
        insert(mRoot, mNodeList, mInputData[i]);
    }

    setTreeScale(mInputSize);
    balanceTree();
    createTree();
}