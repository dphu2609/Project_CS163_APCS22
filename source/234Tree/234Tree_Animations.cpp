#include <State/Tree234.hpp>

void Tree234::createTree() {
    mSceneLayers[Nodes]->clearChildren();
    mSceneLayers[Edges]->clearChildren();
    for (int i = 0; i < mNodeList.size(); i++) {
        if (mNodeList[i]->isInsertNode) {
            std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
            node->set(false,
                std::to_string(mNodeList[i]->val) + (mNodeList[i]->duplicate > 1 ? ("-" + std::to_string(mNodeList[i]->duplicate)) : ""), 
                mNodeList[i]->position, Size::NODE_RADIUS, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
            );
            mSceneLayers[Nodes]->attachChild(std::move(node));
        }
        else if (!mNodeList[i]->isNodeHighlighted) {
            std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
            node->set(false, std::to_string(mNodeList[i]->val) + (mNodeList[i]->duplicate > 1 ? ("-" + std::to_string(mNodeList[i]->duplicate)) : ""), mNodeList[i]->position);
            mSceneLayers[Nodes]->attachChild(std::move(node));
        }
        else {
            std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
            node->set(false,
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
            createBackupTree();
            createTree();
            mSceneLayers[CodeBox]->getChildren()[0]->setMultipleContent(CodeContainer::codeHolder[Code::Insert234]);
            getSplitCheckpoint(mRoot, mInputQueue.front());
            mAnimationStep = 2;
            break;
        }

        case 2: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(1));
            mSceneLayers[CodeBox]->getChildren()[0]->activateLine({0});
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
            if (!mIsReversed) mTreeForBackward.push(createTreeState(4));
            mMovedValue = mSplitCheckpoint[mSplitCheckpointIndex]->val;
            splitNode(mRoot, mNodeList, mSplitCheckpoint[mSplitCheckpointIndex++]);
            mSceneLayers[CodeBox]->getChildren()[0]->activateLine({1, 2});
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
            if (!mIsReversed) mTreeForBackward.push(createTreeState(8));
            if (mRoot == nullptr) {
                mAnimationStep = 10;
                break;
            }
            if (mSplitCheckpoint.empty()) getTravelPath(mRoot, mInputQueue.front());
            else getTravelPath(mSplitCheckpoint.back(), mInputQueue.front());
            mSceneLayers[CodeBox]->getChildren()[0]->activateLine({0});
            mAnimationStep = 9;
            break;
        }

        case 9: {
            traverseAnimation(true, 3, 10);
            break;
        }

        case 10: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(10));
            Node* insertNode = getInternalNode(mRoot, mInputQueue.front());
            if (mRoot == nullptr) mRoot = insertInternalNode(mRoot, mNodeList, mInputQueue.front());
            else insertInternalNode(insertNode, mNodeList, mInputQueue.front());
            balanceTree();
            mSceneLayers[CodeBox]->getChildren()[0]->activateLine({3, 4});
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
                mAnimationStep = 1;
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
            createBackupTree();
            mSceneLayers[CodeBox]->getChildren()[0]->setMultipleContent(CodeContainer::codeHolder[Code::Delete234]);
            mAnimationStep++;
            break;
        }

        case 2: {
            mSceneLayers[CodeBox]->getChildren()[0]->activateLine({0});
            getTravelPath(mRoot, mInputQueue.front());
            mOperationNode = findNode(mRoot, mInputQueue.front());
            if (!mIsReversed) mTreeForBackward.push(createTreeState(1));
            mAnimationStep++;
            break;
        }

        case 3: {
            traverseAnimation(true, 3, 4);
            break;
        }

        case 4: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(4));
            if (mOperationNode) mAnimationStep = 5;
            else mAnimationStep = 20;
            break;
        }

        case 5: {
            mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->change3Color(
                Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, 2
            );
            if (mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->isChange3ColorFinished()) {
                mAnimationStep = 6;
            }
            break;
        }

        case 6: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(6));
            resetAnimation();
            createTree();
            if (mRoot->numKeys == 1) {
                if (mRoot->child[0] && mRoot->child[0]->numKeys == 1 && mRoot->child[1] && mRoot->child[1]->numKeys == 1) {
                    getTravelPath(mRoot, mRoot->val);
                    mAnimationStep = 7;
                    break;
                }
            }
            getTravelPath(mRoot, mInputQueue.front());
            mAnimationStep = 11;
            break;
        }

        case 7: {
            traverseAnimation(true, 3, 8);
            break;
        }

        case 8: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(8));
            mergeNode(mRoot);
            balanceTree();
            mSceneLayers[CodeBox]->getChildren()[0]->activateLine({1});
            mAnimationStep = 9;
            break;
        }

        case 9: {
            moveTreeAnimation(true, 1, 10);
            break;
        }

        case 10: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(10));
            getTravelPath(mRoot, mInputQueue.front());
            mAnimationStep = 11;
            break;
        }

        case 11: {
            traverseAnimation(true, 3, 12);
            break;
        }

        case 12: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(12));
            mOperationNode = findNode(mRoot, mInputQueue.front());  
            if (mOperationNode == nullptr) {
                mAnimationStep = 20;
                break;
            }
            if ((mOperationNode->isAttached && mOperationNode->parent->isLeaf() && mOperationNode->parent->numKeys > 1) || (!mOperationNode->isAttached && mOperationNode->isLeaf() && mOperationNode->numKeys > 1)) {
                mAnimationStep = 18;
                break;
            }
            if ((mOperationNode->isAttached && !mOperationNode->parent->isLeaf()) || (!mOperationNode->isAttached && !mOperationNode->isLeaf())) {
                mReplaceNode = findReplaceNode(mOperationNode);
                if (!mOperationNode->isAttached) getTravelPath(mOperationNode, mReplaceNode->val);
                else getTravelPath(mOperationNode->parent, mReplaceNode->val);
                mSceneLayers[CodeBox]->getChildren()[0]->activateLine({8, 9, 10});
                mAnimationStep = 13;
            }
            else {
                handleLeafNodeWith1NumKeys(mOperationNode);
                balanceTree();
                mSceneLayers[CodeBox]->getChildren()[0]->activateLine({3, 4, 5, 6, 7});
                mAnimationStep = 17;
            }
            break;
        }

        case 13: {
            traverseAnimation(true, 3, 14);
            break;
        }

        case 14: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(14));
            if (mReplaceNode->depth > mOperationNode->depth + 1 || (mReplaceNode->isAttached) || (!mReplaceNode->isAttached && mReplaceNode->numKeys != 1)) {
                std::swap(mOperationNode->val, mReplaceNode->val);
                std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mReplaceNode->nodeIndex]);
                mOperationNode = mReplaceNode;
            }
            if ((!mReplaceNode->isAttached && mReplaceNode->numKeys == 1)) mAnimationStep = 15;
            else {
                mOperationNode = mReplaceNode;
                mAnimationStep = 17;
            }
            break;
        }

        case 15: {
            moveTreeAnimation(true, 1, 16);
            break;
        }

        case 16: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(16));
            handleLeafNodeWith1NumKeys(mReplaceNode);
            balanceTree();
            mAnimationStep = 17;
        }

        case 17: {
            moveTreeAnimation(true, 1, 18);
            break;
        }

        case 18: {   
            if (!mIsReversed) mTreeForBackward.push(createTreeState(18));
            deleteInternalNode(mOperationNode);
            balanceTree();
            mAnimationStep = 19;
            break;
        }

        case 19: {
            mSceneLayers[Nodes]->getChildren()[mOperationIndex]->zoom(sf::Vector2f(0, 0), 1.5);
            moveTreeAnimation(true, 1.5);
            if (mSceneLayers[Nodes]->getChildren()[mOperationIndex]->isZoomFinished()) {
                mSceneLayers[Nodes]->getChildren().erase(mSceneLayers[Nodes]->getChildren().begin() + mOperationIndex);
                for (int i = 0; i < 4; i++) {
                    mSceneLayers[Edges]->getChildren().erase(mSceneLayers[Edges]->getChildren().begin() + mOperationIndex * 4);
                }
                mAnimationStep = 20;
                for (int i = 0; i < mNodeList.size(); i++) {
                    mNodeList[i]->nodeIndex = i;
                }
            }
            break;
        }

        case 20: {
            if (mInputQueue.size() > 1) {
                mInputQueue.pop();
                mAnimationStep = 1;
            }
            else mIsReplay = true;
            break;
        }
    }
}

void Tree234::searchAnimation() {
    switch(mAnimationStep) {
        case 1: {
            resetAnimation();
            createTree();
            createBackupTree();
            mSceneLayers[CodeBox]->getChildren()[0]->setMultipleContent(CodeContainer::codeHolder[Code::Search234]);
            mAnimationStep++;
            break;
        }

        case 2: {
            getTravelPath(mRoot, mInputQueue.front());
            mOperationNode = findNode(mRoot, mInputQueue.front());
            if (!mIsReversed) mTreeForBackward.push(createTreeState(1));
            mSceneLayers[CodeBox]->getChildren()[0]->activateLine({0, 1, 4});
            mAnimationStep++;
            break;
        }

        case 3: {
            traverseAnimation(true, 3, 4);
            break;
        }

        case 4: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(4));
            if (mOperationNode) {
                mSceneLayers[CodeBox]->getChildren()[0]->activateLine({2, 3});
                mAnimationStep = 5;
            }
            else { 
                mSceneLayers[CodeBox]->getChildren()[0]->activateLine({5});
                mAnimationStep = 6;
            }
            break;
        }

        case 5: {
            mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->change3Color(
                Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, 2
            );
            if (mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->isChange3ColorFinished()) {
                mAnimationStep = 6;
            }
            break;
        }

        case 6: {
            if (mInputQueue.size() > 1) {
                mInputQueue.pop();
                mAnimationStep = 1;
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
        node->set(false, 
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

void Tree234::setTreeScale(int treeSize) {
    if (treeSize < 15) {
        NODE_DISTANCE_HORIZONTAL = 320.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 200.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 40.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 40.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 40.f * Constant::SCALE_Y;
    }
    else if (treeSize < 20) {
        NODE_DISTANCE_HORIZONTAL = 260.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 180.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 30.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 30.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 30.f * Constant::SCALE_Y;
    }
    else if (treeSize < 25) {
        NODE_DISTANCE_HORIZONTAL = 180.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 150.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 23.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 23.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 23.f * Constant::SCALE_Y;
    }
    else {
        NODE_DISTANCE_HORIZONTAL = 150.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 150.f * Constant::SCALE_Y;
        Size::NODE_RADIUS = 22.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 22.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 22.f * Constant::SCALE_Y;
    }
}

void Tree234::resetAnimation() {
    mAnimationStep = 1;
    mOperationNode = nullptr;
    mReplaceNode = nullptr;
    mOperationIndex = -1;
    mReplaceIndex = -1;
    resetNodeState();
    for (auto &child : mNodeList) {
        child->isNodeHighlighted = false;
        child->isInsertNode = false;
        for (int i = 0; i < 4; i++) child->isEdgeHighlighted[i] = false;
    }
    while (!mTreeForBackward.empty()) {
        delete mTreeForBackward.top();
        mTreeForBackward.pop();
    }
    setTreeScale(mNodeList.size());
    balanceTree();
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