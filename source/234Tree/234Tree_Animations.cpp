#include <State/Tree234.hpp>

void Tree234::createTree() {
    mSceneLayers[Nodes]->clearChildren();
    mSceneLayers[Edges]->clearChildren();
    for (int i = 0; i < mNodeList.size(); i++) {
        if (mNodeList[i]->isInsertNode) {
            std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
            node->set(
                std::to_string(mNodeList[i]->val) + (mNodeList[i]->duplicate > 1 ? ("-" + std::to_string(mNodeList[i]->duplicate)) : ""), 
                mNodeList[i]->position, Size::NODE_RADIUS, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
            );
            mSceneLayers[Nodes]->attachChild(std::move(node));
        }
        else if (!mNodeList[i]->isNodeHighlighted) {
            std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
            node->set(std::to_string(mNodeList[i]->val) + (mNodeList[i]->duplicate > 1 ? ("-" + std::to_string(mNodeList[i]->duplicate)) : ""), mNodeList[i]->position);
            mSceneLayers[Nodes]->attachChild(std::move(node));
        }
        else {
            std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
            node->set(
                std::to_string(mNodeList[i]->val) + (mNodeList[i]->duplicate > 1 ? ("-" + std::to_string(mNodeList[i]->duplicate)) : ""), 
                mNodeList[i]->position, Size::NODE_RADIUS, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
            );
            mSceneLayers[Nodes]->attachChild(std::move(node));
        }
        for (int j = 0; j < 4; j++) {
            mNodeList[i]->edgeIndex[j] = mSceneLayers[Edges]->getChildren().size();
            sf::Vector2f startPos = sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS_X, mNodeList[i]->position.y + Size::NODE_RADIUS_Y);
            sf::Vector2f endPos = mNodeList[i]->child[j] ? sf::Vector2f(mNodeList[i]->child[j]->position.x + Size::NODE_RADIUS_X, mNodeList[i]->child[j]->position.y + Size::NODE_RADIUS_Y) : sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS_X, mNodeList[i]->position.y + Size::NODE_RADIUS_Y);
            if (mNodeList[i]->numKeys == 2) startPos += sf::Vector2f(- Size::NODE_RADIUS - Size::EDGE_THICKNESS, 0);
            if (mNodeList[i]->child[j] && mNodeList[i]->child[j]->numKeys == 2) endPos += sf::Vector2f(- Size::NODE_RADIUS - Size::EDGE_THICKNESS, 0);
            if (!mNodeList[i]->isEdgeHighlighted[j]) {
                std::unique_ptr<Edge> edge = std::make_unique<Edge>();
                edge->set(startPos, endPos);
                mSceneLayers[Edges]->attachChild(std::move(edge));
            }
            else {
                std::unique_ptr<Edge> edge = std::make_unique<Edge>();
                edge->set(startPos, endPos,Size::EDGE_THICKNESS, Color::NODE_HIGHLIGHT_COLOR);
                mSceneLayers[Edges]->attachChild(std::move(edge));
            }
        }
    }
}

void Tree234::insertAnimation() {
    switch (mAnimationStep) {
        case 1: {
            resetAnimation();
            createTree();
            getSplitCheckpoint(mRoot, mInputQueue.front());
            mAnimationStep = 2;
            break;
        }

        case 2: {
            if (mSplitCheckpoint.empty()) {
                mAnimationStep = 8;
                break;
            }
            if (mSplitCheckpointIndex == 0) getTravelPath(mRoot, mSplitCheckpoint[mSplitCheckpointIndex]->val);
            else {
                getTravelPath(mSplitCheckpoint[mSplitCheckpointIndex - 1], mSplitCheckpoint[mSplitCheckpointIndex]->val);
            }
            mAnimationStep = 3;
            break;
        }

        case 3: {
            traverseAnimation(true, 3, 4);
            break;
        }

        case 4: {
            mMovedValue = mSplitCheckpoint[mSplitCheckpointIndex]->val;
            splitNode(mRoot, mNodeList, mSplitCheckpoint[mSplitCheckpointIndex++]);
            mAnimationStep = 5;
            break;
        }

        case 5: {
            moveTreeAnimation(true, 1, 6);
            break;
        }
        
        case 6: {
            Node* splitedNode = mSplitCheckpoint[mSplitCheckpointIndex - 1];
            if (splitedNode->numKeys == 2) {
                if (mMovedValue == splitedNode->val) {
                    std::swap(mSceneLayers[Nodes]->getChildren()[splitedNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[splitedNode->tempLeft->nodeIndex]);
                }
            }
            else if (splitedNode->numKeys == 3) {
                if (mMovedValue == splitedNode->val) {
                    std::swap(mSceneLayers[Nodes]->getChildren()[splitedNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[splitedNode->tempRight->nodeIndex]);
                }
                else if (mMovedValue == splitedNode->tempLeft->val) {
                    std::swap(mSceneLayers[Nodes]->getChildren()[splitedNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[splitedNode->tempRight->nodeIndex]);
                    std::swap(mSceneLayers[Nodes]->getChildren()[splitedNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[splitedNode->tempLeft->nodeIndex]);
                }
            }
            balanceTree();
            mAnimationStep = 7;
            break;
        }

        case 7: {
            if (mSplitCheckpointIndex == mSplitCheckpoint.size()) moveTreeAnimation(true, 1, 8);
            else moveTreeAnimation(true, 1, 2);
            break;
        }

        case 8: {
            if (mSplitCheckpoint.empty()) getTravelPath(mRoot, mInputQueue.front());
            else getTravelPath(mSplitCheckpoint.back(), mInputQueue.front());
            mAnimationStep = 9;
            break;
        }

        case 9: {
            traverseAnimation(true, 3, 10);
            break;
        }

        case 10: {
            Node* insertNode = getInternalNode(mRoot, mInputQueue.front());
            insertInternalNode(insertNode, mNodeList, mInputQueue.front());
            balanceTree();
            mAnimationStep = 11;
            break;
        }
        
        case 11: {
            nodeAppearAnimation(true, 2);
            moveTreeAnimation(true, 2, 12);
            break;
        }

        case 12: {
            if (mInputQueue.size() > 1) {
                mInputQueue.pop();
                resetNodeState();
            }
            else mIsReplay = true;
            break;
        }
    }
}

void Tree234::deleteAnimation() {
    switch (mAnimationStep) {
        case 1: {
            resetAnimation();
            createTree();
            if (mRoot->numKeys == 1) {
                if (mRoot->child[0] && mRoot->child[0]->numKeys == 1 && mRoot->child[1] && mRoot->child[1]->numKeys == 1) {
                    getTravelPath(mRoot, mRoot->val);
                    mAnimationStep = 2;
                    break;
                }
            }
            getTravelPath(mRoot, mInputQueue.front());
            mAnimationStep = 6;
            break;
        }

        case 2: {
            traverseAnimation(true, 3, 3);
            break;
        }

        case 3: {
            mergeNode(mRoot);
            balanceTree();
            mAnimationStep = 4;
            break;
        }

        case 4: {
            moveTreeAnimation(true, 1, 5);
            break;
        }

        case 5: {
            getTravelPath(mRoot, mInputQueue.front());
            mAnimationStep = 6;
            break;
        }

        case 6: {
            traverseAnimation(true, 3, 7);
            break;
        }

        case 7: {
            mOperationNode = findNode(mRoot, mInputQueue.front());
            if ((mOperationNode->isAttached && mOperationNode->parent->isLeaf() && mOperationNode->parent->numKeys > 1) || (!mOperationNode->isAttached && mOperationNode->isLeaf() && mOperationNode->numKeys > 1)) {
                mAnimationStep = 11;
                break;
            }
            if (mOperationNode->isAttached || (!mOperationNode->isAttached && !mOperationNode->isLeaf())) {
                mReplaceNode = findReplaceNode(mOperationNode);
                if (mReplaceNode->numKeys == 1 && ((mOperationNode->isAttached && mReplaceNode->parent == mOperationNode->parent) || (!mOperationNode->isAttached && mReplaceNode->parent == mOperationNode))) {
                    if (mOperationNode->isAttached) {
                        if (mOperationNode->val < mOperationNode->parent->val) {
                            if (mOperationNode->parent->child[1]->numKeys == 1) {
                                mergeNode(mOperationNode);
                            }
                            else {
                                rotateLeft(mOperationNode->parent->child[0]);
                            }
                        }
                        else {
                            if (mOperationNode->parent->child[2]->numKeys == 1) {
                                mergeNode(mOperationNode);
                            }
                            else {
                                rotateRight(mOperationNode->parent->child[3]);
                            }
                        }
                    }
                    else {
                        if (mOperationNode->numKeys == 1) {
                            rotateLeft(mOperationNode->child[0]);
                        }
                        if (mOperationNode->numKeys == 2) {
                            if (mOperationNode->child[1]->numKeys == 1) {
                                mergeNode(mOperationNode);
                            }
                            else {
                                rotateRight(mOperationNode->child[2]);
                                mOperationNode = mOperationNode->child[2]->tempLeft;
                            }
                        }
                        else if (mOperationNode->numKeys == 3) {
                            if (mOperationNode->child[1]->numKeys == 1 && mOperationNode->child[2]->numKeys == 1) {
                                std::cout << "MERGE NODE" << std::endl;
                                mergeNode(mOperationNode);
                            }
                            else if (mOperationNode->child[1]->numKeys == 1) {
                                rotateLeft(mOperationNode->child[2]);
                                mOperationNode = mOperationNode->child[2];
                            }
                            else {
                                rotateRight(mOperationNode->child[2]);
                                mOperationNode = mOperationNode->child[2]->tempLeft;
                            }
                        }
                    }
                    balanceTree();
                    mAnimationStep = 10;
                }
                else {
                    getTravelPath(mOperationNode, mReplaceNode->val);
                    mAnimationStep = 8;
                }
            }
            else {
                if (mOperationNode->orderOfNode[0]) {
                    if (mOperationNode->parent->child[1]->numKeys == 1) {
                        mergeNode(mOperationNode->parent);
                    }
                    else {
                        rotateLeft(mOperationNode);
                    }
                }
                else if (mOperationNode->orderOfNode[1]) {
                    if (mOperationNode->parent->numKeys == 1) {
                        if (mOperationNode->parent->child[0]->numKeys == 1) {
                            mergeNode(mOperationNode->parent);
                        }
                        else {
                            rotateRight(mOperationNode);
                        }
                    }
                    else {
                        if (mOperationNode->parent->child[0]->numKeys == 1 && mOperationNode->parent->child[2]->numKeys == 1) {
                            mergeNode(mOperationNode->parent);
                        }
                        else if (mOperationNode->parent->child[0]->numKeys == 1) {
                            std::cout << "rotate left" << std::endl;
                            rotateLeft(mOperationNode);
                            std::cout << "rotate left done" << std::endl;
                        }
                        else {
                            std::cout << "rotate right" << std::endl;
                            rotateRight(mOperationNode);
                            std::cout << "rotate right done" << std::endl;
                        }
                    }
                }
                else if (mOperationNode->orderOfNode[2]) {
                    if (mOperationNode->parent->numKeys == 2) {
                        if (mOperationNode->parent->child[1]->numKeys == 1) {
                            mergeNode(mOperationNode->parent);
                        }
                        else {
                            std::cout << "rotate left" << std::endl;
                            rotateLeft(mOperationNode);
                            std::cout << "rotate left done" << std::endl;
                        }
                    }
                    else if (mOperationNode->parent->numKeys == 3) {
                        if (mOperationNode->parent->child[1]->numKeys == 1 && mOperationNode->parent->child[3]->numKeys == 1) {
                            mergeNode(mOperationNode->parent);
                        }
                        else if (mOperationNode->parent->child[1]->numKeys == 1) {
                            std::cout << "rotate left" << std::endl;
                            rotateLeft(mOperationNode);
                            std::cout << "rotate left done" << std::endl;
                        }
                        else {
                            std::cout << "rotate right" << std::endl;
                            rotateRight(mOperationNode);
                            std::cout << "rotate right done" << std::endl;
                        }
                    }
                }
                else if (mOperationNode->orderOfNode[3]) {
                    if (mOperationNode->parent->child[2]->numKeys == 1) {
                        mergeNode(mOperationNode->parent);
                    }
                    else {
                        std::cout << "rotate right" << std::endl;
                        rotateRight(mOperationNode);
                        std::cout << "rotate right done" << std::endl;
                    }
                }
                balanceTree();
                mAnimationStep = 10;
            }
            break;
        }

        case 8: {
            traverseAnimation(true, 3, 9);
            break;
        }

        case 9: {
            std::swap(mOperationNode->val, mReplaceNode->val);
            std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mReplaceNode->nodeIndex]);
            mOperationNode = mReplaceNode;
            mAnimationStep = 10;
            break;
        }

        case 10: {
            moveTreeAnimation(true, 1, 11);
            break;
        }

        case 11: {   
            deleteInternalNode(mOperationNode);
            balanceTree();
            mAnimationStep = 12;
            break;
        }

        case 12: {
            mSceneLayers[Nodes]->getChildren()[mOperationIndex]->zoom(sf::Vector2f(0, 0), 1);
            moveTreeAnimation(true, 1);
            if (mSceneLayers[Nodes]->getChildren()[mOperationIndex]->isZoomFinished()) {
                mSceneLayers[Nodes]->getChildren().erase(mSceneLayers[Nodes]->getChildren().begin() + mOperationIndex);
                for (int i = 0; i < 4; i++) {
                    mSceneLayers[Edges]->getChildren().erase(mSceneLayers[Edges]->getChildren().begin() + mOperationIndex * 4);
                }
                mAnimationStep = 13;
                for (int i = 0; i < mNodeList.size(); i++) {
                    mNodeList[i]->nodeIndex = i;
                }
            }
            break;
        }

        case 13: {
            if (mInputQueue.size() > 1) {
                mInputQueue.pop();
                resetNodeState();
            }
            else mIsReplay = true;
            break;
        }
    }
}

void Tree234::traverseAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    if (!mTraverseControler.first) {
        mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->change3Color(
            Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
        mTravelPath[mTravelIndex].first->isNodeHighlighted = true;
        if (mTravelPath[mTravelIndex].first->tempLeft) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->tempLeft->nodeIndex]->change3Color(
                Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
            );
            mTravelPath[mTravelIndex].first->tempLeft->isNodeHighlighted = true;
        }
        if (mTravelPath[mTravelIndex].first->tempRight) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->tempRight->nodeIndex]->change3Color(
                Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
            );
            mTravelPath[mTravelIndex].first->tempRight->isNodeHighlighted = true;
        }
    }
    else if (!mTraverseControler.second) {
        mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->change3Color(
            Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
        if (mTravelPath[mTravelIndex].first->tempLeft) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->tempLeft->nodeIndex]->change3Color(
                Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
            );
        }
        if (mTravelPath[mTravelIndex].first->tempRight) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->tempRight->nodeIndex]->change3Color(
                Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
            );
        }

    }
    else {
        mSceneLayers[Edges]->getChildren()[mTravelPath[mTravelIndex].first->edgeIndex[mTravelPath[mTravelIndex].second]]->change1Color(
            sf::Color(255, 171, 25), speed
        );
        mTravelPath[mTravelIndex].first->isEdgeHighlighted[mTravelPath[mTravelIndex].second] = true;
    }  

    if (!mTraverseControler.first) {
        if (mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->isChange3ColorFinished()) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->resetAnimationVar();
            if (mTravelPath[mTravelIndex].first->tempLeft) {
                mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->tempLeft->nodeIndex]->resetAnimationVar();
            }
            if (mTravelPath[mTravelIndex].first->tempRight) {
                mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->tempRight->nodeIndex]->resetAnimationVar();
            }
            mTraverseControler.first = true;
        }
    }   
    else if (!mTraverseControler.second && !mIsAnimationPaused) {
        if (mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->isChange3ColorFinished()) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->nodeIndex]->resetAnimationVar();
            if (mTravelPath[mTravelIndex].first->tempLeft) {
                mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->tempLeft->nodeIndex]->resetAnimationVar();
            }
            if (mTravelPath[mTravelIndex].first->tempRight) {
                mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex].first->tempRight->nodeIndex]->resetAnimationVar();
            }
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

void Tree234::moveTreeAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    for (auto &node : mNodeList) {
        mSceneLayers[Nodes]->getChildren()[node->nodeIndex]->move(node->position, speed);
    }
    for (auto &node : mNodeList) {
        for (int i = 0; i < 4; i++) {
            sf::Vector2f startPos = node->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS);
            if (node->numKeys == 2) startPos -= sf::Vector2f(Size::NODE_RADIUS + Size::EDGE_THICKNESS, 0);
            sf::Vector2f endPos = ((node->child[i]) ? node->child[i]->position : node->position) + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS);
            if (node->child[i] && node->child[i]->numKeys == 2) endPos -= sf::Vector2f(Size::NODE_RADIUS + Size::EDGE_THICKNESS, 0);
            mSceneLayers[Edges]->getChildren()[node->edgeIndex[i]]->moveBy2Points(
                startPos, endPos, speed
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

void Tree234::nodeAppearAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    if (mSceneLayers[Nodes]->getChildren().size() < mNodeList.size()) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        node->set(
            std::to_string(mOperationNode->val), mOperationNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), 0,
            Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
        );
        mSceneLayers[Nodes]->attachChild(std::move(node));

        for (int i = 0; i < 4; i++) {
            mNodeList.back()->edgeIndex[i] = mSceneLayers[Edges]->getChildren().size();
            std::unique_ptr<Edge> edge = std::make_unique<Edge>();
            edge->set(
                mOperationNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), 
                mOperationNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS)
            );
            mSceneLayers[Edges]->attachChild(std::move(edge));
        }

        if (mOperationNode->isAttached) {
            if (mOperationNode->parent->numKeys == 3) {
                if (mInputQueue.front() < mOperationNode->parent->val) {
                    std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->parent->nodeIndex], mSceneLayers[Nodes]->getChildren()[mOperationNode->parent->tempRight->nodeIndex]);
                    std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mOperationNode->parent->nodeIndex]);
                }
            }
        }
        else {
            if (mOperationNode->numKeys == 3) {
                std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mOperationNode->tempRight->nodeIndex]);
            }
            else if (mOperationNode->numKeys == 2) {
                std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mOperationNode->tempLeft->nodeIndex]);
            }
        }
    }
    
    mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->zoom(sf::Vector2f(Size::NODE_RADIUS, 0), speed);

    if (mSceneLayers[Nodes]->getChildren().back()->isZoomFinished() && !mIsAnimationPaused) {
        if (animationStepAfterFinish != 0) {
            mAnimationStep = animationStepAfterFinish;
            resetNodeState();
        }
    }
}

void Tree234::resetAnimation() {
    mAnimationStep = 1;
    mOperationNode = nullptr;
    mReplaceNode = nullptr;
    mOperationIndex = -1;
    mReplaceIndex = -1;
    for (auto &child : mSceneLayers[Nodes]->getChildren()) child->resetAnimationVar();
    for (auto &child : mSceneLayers[Edges]->getChildren()) child->resetAnimationVar();
    for (auto &child : mNodeList) {
        child->isNodeHighlighted = false;
        child->isInsertNode = false;
        for (int i = 0; i < 4; i++) child->isEdgeHighlighted[i] = false;
    }
}

bool Tree234::isProcessingAnimation() {
    for (auto &child : mSceneLayers[Nodes]->getChildren()) 
        if (child->isProcessing()) return true;
    for (auto &child : mSceneLayers[Edges]->getChildren())
        if (child->isProcessing()) return true;
    return false;
}

void Tree234::resetNodeState() {
    for (auto &child : mSceneLayers[Nodes]->getChildren()) {
        child->resetAnimationVar();
    }
    for (auto &child : mSceneLayers[Edges]->getChildren()) {
        child->resetAnimationVar();
    }
}