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
            createTree();
            getSplitCheckpoint(mRoot, mInputQueue.front());
            mAnimationStep = 2;
            break;
        }

        case 2: {
            if (mSplitCheckpoint.empty()) {
                mAnimationStep = 6;
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
            splitNode(mRoot, mNodeList, mSplitCheckpoint[mSplitCheckpointIndex++]);
            mAnimationStep = 5;
            break;
        }

        case 5: {
            moveTreeAnimation(true, 1, 6);
            break;
        }
        
        case 6: {
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
                resetAnimation();
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
    }
    
    mSceneLayers[Nodes]->getChildren().back()->zoom(sf::Vector2f(Size::NODE_RADIUS, 0), speed);

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