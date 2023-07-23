#include <State/Trie.hpp>

void Trie::createTree() {
    mSceneLayers[Nodes]->getChildren().clear();
    mSceneLayers[Edges]->getChildren().clear();
    for (auto &node : mNodeList) {
        if (node->isEndOfWord) {
            std::unique_ptr<TreeNode> trieNode = std::make_unique<TreeNode>();
            std::string content = "a";
            content[0] = node->val;
            if (node->isNodeHighlighted) {
                trieNode->set(
                    content, node->position, Size::NODE_RADIUS, 
                    Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
                );
            }
            else {
                trieNode->set(
                    content, node->position, Size::NODE_RADIUS, 
                    Color::NODE_TEXT_COLOR, Color::NODE_COLOR, Color::NODE_OUTLINE_COLOR
                );
            }
            mSceneLayers[Nodes]->attachChild(std::move(trieNode));
        }
        else {
            std::unique_ptr<TreeNode> trieNode = std::make_unique<TreeNode>();
            std::string content = "a";
            content[0] = node->val;
            if (node->isNodeHighlighted) {
                trieNode->set(
                    content, node->position, Size::NODE_RADIUS, 
                    Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
                );
            }
            else trieNode->set(content, node->position);
            mSceneLayers[Nodes]->attachChild(std::move(trieNode));
        }
    }
    if (mIsEdgeHighlighted.empty()) {
        for (auto &node : mNodeList) {
            node->edgeIndex.clear();
            for (int i = 0; i < node->child.size(); i++) {
                node->edgeIndex.push_back(mSceneLayers[Edges]->getChildren().size());
                std::unique_ptr<Edge> edge = std::make_unique<Edge>();
                edge->set(node->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), node->child[i]->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS));
                mSceneLayers[Edges]->attachChild(std::move(edge));
                mIsEdgeHighlighted.push_back(false);
            }
        }
    }
    else {
        int index = 0;
        for (auto &node : mNodeList) {
            node->edgeIndex.clear();
            for (int i = 0; i < node->child.size(); i++) {
                node->edgeIndex.push_back(index++);
                std::unique_ptr<Edge> edge = std::make_unique<Edge>();
                if (mIsEdgeHighlighted[index - 1]) {
                    edge->set(
                        node->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), node->child[i]->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
                        Size::EDGE_THICKNESS, Color::NODE_HIGHLIGHT_COLOR
                    );
                }
                else {
                    edge->set(node->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), node->child[i]->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS));
                }
                mSceneLayers[Edges]->attachChild(std::move(edge));
            }
        }
    }
}

void Trie::insertAnimation() {
    switch (mAnimationStep) {
        case 1: {
            resetAnimation();
            createBackupTree();
            createTree();
            mAnimationStep = 2;
            break;
        }

        case 2: {
            if (mRoot == nullptr) {
                Node* newNode = new Node{' ', 0, 0, {}, -1, sf::Vector2f(Constant::WINDOW_WIDTH/2 - Size::NODE_RADIUS, 150 * Constant::SCALE_Y) + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), {}, nullptr, false, false};
                mRoot = newNode;
                mNodeList.push_back(newNode);
                std::unique_ptr<TreeNode> trieNode = std::make_unique<TreeNode>();
                std::string content = "a";
                content[0] = mRoot->val;
                trieNode->set(content, mRoot->position, 0);
                mSceneLayers[Nodes]->attachChild(std::move(trieNode));
                mAnimationStep = 6;
            }
            else mAnimationStep = 3;
            break;
        }

        case 3: {
            getTravelPath(mInputQueue.front());
            mOperationNode = mTravelPath.back().first;
            mOperationIndex = mTravelPath.size() - 1;
            mAnimationStep = 4;
            break;
        }

        case 4: {
            traverseAnimation(true, 3, 5);
            break;
        }

        case 5: {
            if (mOperationIndex < mInputQueue.front().size()) {
                add1Node(mOperationNode, mNodeList, mInputQueue.front(), mOperationIndex);
                balanceTree();
                mOperationNode->position += sf::Vector2f(0, Size::NODE_RADIUS);
                mAnimationStep = 6;
            }
            else {
                mAnimationStep = 7;
            }
            break;
        }

        case 6: {
            mSceneLayers[Nodes]->getChildren().back()->zoom(sf::Vector2f(Size::NODE_RADIUS, 0), 1.5);
            if (mRoot->child.size() != 0) moveTreeAnimation(true, 1.5);
            if (mSceneLayers[Nodes]->getChildren().back()->isZoomFinished()) {
                if (mRoot->child.size() == 0) mAnimationStep = 3;
                else mAnimationStep = 5;
                resetNodeState();
            }
            break;
        }

        case 7: {
            if (mInputQueue.size() > 1) {
                mInputQueue.pop();
            }
            else mIsReplay = true;
            break;
        }
    }
}

void Trie::traverseAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    if (!mTraverseControler.first) {
        mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->change3Color(
            Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
        mTravelPath[mTravelIndex].first->isNodeHighlighted = true;
    }
    else if (!mTraverseControler.second) {
        mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->change3Color(
            Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
    }
    else {
        mSceneLayers[Edges]->getChildren()[mTravelPath[mTravelIndex].first->edgeIndex[mTravelPath[mTravelIndex].second]]->change1Color(
            sf::Color(255, 171, 25), speed
        );
        mIsEdgeHighlighted[mTravelPath[mTravelIndex].second] = true;
    }
    if (!mTraverseControler.first) {
        if (mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->isChange3ColorFinished()) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->resetAnimationVar();
            mTraverseControler.first = true;
        }
    } 
    else if (!mTraverseControler.second && !mIsAnimationPaused) {
        if (mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->isChange3ColorFinished()) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->resetAnimationVar();
            mTraverseControler.second = true;
            if (mTravelPath[mTravelIndex].second == -1) {
                mTraverseControler = std::make_pair(false, false);
                resetNodeState();
                mAnimationStep = animationStepAfterFinish;
            }
        }
    } 
    else if (!mIsAnimationPaused) {
        if (mSceneLayers[Edges]->getChildren()[mTravelPath[mTravelIndex].first->edgeIndex[mTravelPath[mTravelIndex].second]]->isChange1ColorFinished()) {
            mSceneLayers[Edges]->getChildren()[mTravelPath[mTravelIndex].first->edgeIndex[mTravelPath[mTravelIndex].second]]->resetAnimationVar();
            mTravelIndex++;
            mTraverseControler = std::make_pair(false, false);
            if (mTravelIndex >= mTravelPath.size()) {
                resetNodeState();
                mAnimationStep = animationStepAfterFinish;
            }
        }
    }
}

void Trie:: moveTreeAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    for (auto &node : mNodeList) {
        mSceneLayers[Nodes]->getChildren()[node->nodeIndex]->move(node->position, speed);
    }
    for (auto &node : mNodeList) {
        for (int i = 0; i < node->child.size(); i++) {
            mSceneLayers[Edges]->getChildren()[node->edgeIndex[i]]->moveBy2Points(
                node->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), 
                node->child[i]->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), speed
            );
        }
    }
    if (animationStepAfterFinish != 0 && !mIsAnimationPaused) {
        bool isFinishedAll = true;
        for (auto &child : mSceneLayers[Nodes]->getChildren()) {
            if (!child->isMoveFinished()) {
                isFinishedAll = false;
                break;
            }
        }
        for (auto &child : mSceneLayers[Edges]->getChildren()) {
            if (!child->isMoveBy2PointsFinished()) {
                isFinishedAll = false;
                break;
            }
        }
        if (isFinishedAll) {
            mAnimationStep = animationStepAfterFinish;
            resetNodeState();
        }
    }
}

void Trie::resetAnimation() {
    mAnimationStep = 1;
    mOperationNode = nullptr;
    mOperationIndex = 0;
    resetNodeState();
    int wordNum = 0;
    for (auto &child : mNodeList) {
        child->isNodeHighlighted = false;
        if (child->isEndOfWord) wordNum++;
    }
    mIsEdgeHighlighted.clear();
    setTreeScale(wordNum);
    balanceTree();
}

void Trie::resetNodeState() {
    for (auto &child : mSceneLayers[Nodes]->getChildren()) child->resetAnimationVar();
    for (auto &child : mSceneLayers[Edges]->getChildren()) child->resetAnimationVar();
}