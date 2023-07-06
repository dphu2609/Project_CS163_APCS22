#include <State/AVL.hpp>

void AVL::createTree() {
    mSceneLayers[Nodes]->clearChildren();
    mSceneLayers[LeftEdges]->clearChildren();
    mSceneLayers[RightEdges]->clearChildren();
    int index = 0;
    while (index < mNodeList.size()) {
        for (int i = 0; i < mNodeList.size(); i++) {
            if (mNodeList[i]->nodeIndex == index) {
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

                if (!mNodeList[i]->isLeftEdgeHighlighted) {
                    std::unique_ptr<Edge> leftEdge = std::make_unique<Edge>();
                    leftEdge->set(sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS), (mNodeList[i]->left ? sf::Vector2f(mNodeList[i]->left->position.x + Size::NODE_RADIUS, mNodeList[i]->left->position.y + Size::NODE_RADIUS) : sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS)));
                    mSceneLayers[LeftEdges]->attachChild(std::move(leftEdge));
                }
                else {
                    std::unique_ptr<Edge> leftEdge = std::make_unique<Edge>();
                    leftEdge->set(
                        sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS), 
                        (mNodeList[i]->left ? sf::Vector2f(mNodeList[i]->left->position.x + Size::NODE_RADIUS, mNodeList[i]->left->position.y + Size::NODE_RADIUS) : sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS)),
                        Size::EDGE_THICKNESS, Color::NODE_HIGHLIGHT_COLOR
                    );
                    mSceneLayers[LeftEdges]->attachChild(std::move(leftEdge));
                }

                if (!mNodeList[i]->isRightEdgeHighlighted) {
                    std::unique_ptr<Edge> rightEdge = std::make_unique<Edge>();
                    rightEdge->set(sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS), (mNodeList[i]->right ? sf::Vector2f(mNodeList[i]->right->position.x + Size::NODE_RADIUS, mNodeList[i]->right->position.y + Size::NODE_RADIUS) : sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS)));
                    mSceneLayers[RightEdges]->attachChild(std::move(rightEdge));
                }
                else {
                    std::unique_ptr<Edge> rightEdge = std::make_unique<Edge>();
                    rightEdge->set(
                        sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS), 
                        (mNodeList[i]->right ? sf::Vector2f(mNodeList[i]->right->position.x + Size::NODE_RADIUS, mNodeList[i]->right->position.y + Size::NODE_RADIUS) : sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS)),
                        Size::EDGE_THICKNESS, Color::NODE_HIGHLIGHT_COLOR
                    );
                    mSceneLayers[RightEdges]->attachChild(std::move(rightEdge));
                }
                index++;
            }
        }
    }
}

void AVL::insertAnimation() {
    switch(mAnimationStep) {
        case 1: {
            createBackupTree();
            createTree();
            mAnimationStep++;
            break;
        }
        case 2: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(2));
            getTravelPath(mRoot, mInputQueue.front());
            mAnimationStep++;
            break;
        }
        case 3: {
            traverseAnimation(true, 3, 4);
            break;
        }

        case 4: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(4));
            insert(mRoot, mNodeList, mInputQueue.front(), false);
            mOperationNode->isInsertNode = true;
            mNodeForRotate = getRotateNode();
            balanceTree();
            if (mOperationNode->duplicate > 1) { 
                mAnimationStep = 6;
                if (!mIsReversed) mTreeForBackward.push(createTreeState(6));
            }
            else {
                mAnimationStep = 5;
                if (!mIsReversed) mTreeForBackward.push(createTreeState(5));
            }
            break;
        }

        case 5: { //normal case
            nodeAppearAnimation(true, 2);
            moveTreeAnimation(true, 2, 7);
            break;
        }

        case 6: {
            changeNodeAnimation(true, 2, 17);
            break;
        }

        case 7: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(7));
            if (!mNodeForRotate) {
                mAnimationStep = 17;
                break;
            }
            getBalanceFactorPath(mOperationNode->parent, mNodeForRotate);
            mAnimationStep = 8;
            break;
        }

        case 8: {
            checkBalanceFactorAnimation(true, 2, 9);
            break;
        }

        case 9: {
            if (mNodeForRotate->balanceFactor > 1) {
                if (mOperationNode->val > mNodeForRotate->right->val) {
                    if (mNodeForRotate->parent) {
                        if (mNodeForRotate->isLeft) mNodeForRotate->parent->left = rotateLeft(mNodeForRotate);
                        else mNodeForRotate->parent->right = rotateLeft(mNodeForRotate);
                    }
                    else mRoot = rotateLeft(mNodeForRotate);
                    std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    mAnimationStep = 14;
                } else {
                    mNodeForRotate->right = rotateRight(mNodeForRotate->right);
                    std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->right->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->right->right->nodeIndex]);
                    std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->right->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->right->right->nodeIndex]);
                    std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->right->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->right->right->nodeIndex]);
                    mAnimationStep = 10;
                }
            }
            else if (mNodeForRotate->balanceFactor < -1) {
                if (mOperationNode->val < mNodeForRotate->left->val) {
                    if (mNodeForRotate->parent) {
                        if (mNodeForRotate->isLeft) mNodeForRotate->parent->left = rotateRight(mNodeForRotate);
                        else mNodeForRotate->parent->right = rotateRight(mNodeForRotate);
                    }
                    else mRoot = rotateRight(mNodeForRotate);
                    std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    mAnimationStep = 14;
                } else {
                    mNodeForRotate->left = rotateLeft(mNodeForRotate->left);
                    std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->left->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->left->left->nodeIndex]);
                    std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->left->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->left->left->nodeIndex]);
                    std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->left->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->left->left->nodeIndex]);
                    mAnimationStep = 10;
                }
            }
            if (mAnimationStep == 14) {
                if (!mIsReversed) mTreeForBackward.push(createTreeState(14));
                updateHeightAndBalanceFactor(mRoot);
            }
            else if (!mIsReversed) mTreeForBackward.push(createTreeState(10));
            resetNodeState();
            break;
        }

        case 10: {
            moveTreeAnimation(true, 1.5, 11);
            break;
        }

        case 11: {
            balanceTree();
            resetNodeState();
            if (!mIsReversed) mTreeForBackward.push(createTreeState(12));
            mAnimationStep = 12;
            break;
        }

        case 12: {
            moveTreeAnimation(true, 1.5, 13);
            break;
        }

        case 13: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(13));
            if (mNodeForRotate->balanceFactor > 1) {
                if (mNodeForRotate->parent) {
                    if (mNodeForRotate->isLeft) mNodeForRotate->parent->left = rotateLeft(mNodeForRotate);
                    else mNodeForRotate->parent->right = rotateLeft(mNodeForRotate);
                }
                else mRoot = rotateLeft(mNodeForRotate);
                std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
            }
            else if (mNodeForRotate->balanceFactor < -1) {
                if (mNodeForRotate->parent) {
                    if (mNodeForRotate->isLeft) mNodeForRotate->parent->left = rotateRight(mNodeForRotate);
                    else mNodeForRotate->parent->right = rotateRight(mNodeForRotate);
                }
                else mRoot = rotateRight(mNodeForRotate);
                std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
            }
            resetNodeState();
            updateHeightAndBalanceFactor(mRoot);
            if (!mIsReversed) mTreeForBackward.push(createTreeState(14));
            mAnimationStep = 14;
            break;
        }

        case 14: {
            moveTreeAnimation(true, 1.5, 15);
            break;
        }

        case 15: {
            balanceTree();
            resetNodeState();
            if (!mIsReversed) mTreeForBackward.push(createTreeState(16));
            mAnimationStep = 16;
            break;
        }

        case 16: {
            moveTreeAnimation(true, 1.5, 17);
            break;
        }


        case 17: {
            if (mNodeForRotate) getBalanceFactorPath(mNodeForRotate, mRoot);
            else getBalanceFactorPath(mOperationNode->parent, mRoot);
            resetNodeState();
            mAnimationStep = 18;
            break;
        }

        case 18: {
            checkBalanceFactorAnimation(true, 2, 19);
            break;
        }

        case 19: {
            if (mInputQueue.size() > 1) {
                mInputQueue.pop();
                resetAnimation();
            }
            else mIsReplay = true;
            break;
        }
    };
    mIsAnimationPaused = mIsStepByStepMode;
}

void AVL::deleteAnimation() {
    switch (mAnimationStep) {
        case 1: {
            createTree();
            createBackupTree();
            mAnimationStep++;
            break;
        }

        case 2: {
            getTravelPath(mRoot, mInputQueue.front());
            mAnimationStep++;
            break;
        }

        case 3: {
            traverseAnimation(true, 3, 4);
            break;
        }

        case 4: {
            find2NodesForDelete(mInputQueue.front());
            if (mReplaceNode) {
                getTravelPath(mOperationNode, mReplaceNode->val);
                mAnimationStep++;
            }
            else if (mOperationNode->duplicate > 1) {
                mOperationNode->duplicate--;
                mAnimationStep = 22;
            }
            else mAnimationStep = 6;
            break;
        }

        case 5: {
            traverseAnimation(true, 3, 6);
            break;
        }

        case 6: {
            changeLink();
            mAnimationStep++;
            break;
        }

        case 7: {
            mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->move(mNodeListForBackup[mOperationNode->nodeIndex]->position, 1.5);            mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->zoom(sf::Vector2f(0, 0), 1.5);
            moveTreeAnimation(true, 1.5, 8);
            break;
        }

        case 8: {
            deleteNode();
            mNodeForRotate = getRotateNode();
            mAnimationStep++;
            break;
        }

        case 9: {
            moveTreeAnimation(true, 1.5, 10);
            break;
        }

        case 10: {
            if (!mNodeForRotate) {
                mAnimationStep = 23; ///////
                break;
            }
            getBalanceFactorPath(mNodeStartChecking, mNodeForRotate);
            mAnimationStep = 11;
            break;
        }

        case 11: {
            checkBalanceFactorAnimation(true, 2, 12);
            break;
        }

        case 12: {
            if (mNodeForRotate->balanceFactor > 1) {
                if (mNodeForRotate->right->balanceFactor == 1) {
                    if (mNodeForRotate->parent) {
                        if (mNodeForRotate->isLeft) mNodeForRotate->parent->left = rotateLeft(mNodeForRotate);
                        else mNodeForRotate->parent->right = rotateLeft(mNodeForRotate);
                    }
                    else mRoot = rotateLeft(mNodeForRotate);
                    std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    mAnimationStep = 17;
                } else if (mNodeForRotate->right->balanceFactor == -1) {
                    mNodeForRotate->right = rotateRight(mNodeForRotate->right);
                    std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->right->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->right->right->nodeIndex]);
                    std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->right->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->right->right->nodeIndex]);
                    std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->right->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->right->right->nodeIndex]);
                    mAnimationStep = 13;
                }
            }
            else if (mNodeForRotate->balanceFactor < -1) {
                if (mNodeForRotate->left->balanceFactor == -1) {
                    if (mNodeForRotate->parent) {
                        if (mNodeForRotate->isLeft) mNodeForRotate->parent->left = rotateRight(mNodeForRotate);
                        else mNodeForRotate->parent->right = rotateRight(mNodeForRotate);
                    }
                    else mRoot = rotateRight(mNodeForRotate);
                    std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                    mAnimationStep = 17;
                } else if (mNodeForRotate->left->balanceFactor == 1) {
                    mNodeForRotate->left = rotateLeft(mNodeForRotate->left);
                    std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->left->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->left->left->nodeIndex]);
                    std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->left->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->left->left->nodeIndex]);
                    std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->left->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->left->left->nodeIndex]);
                    mAnimationStep = 13;
                }
            }
            if (mAnimationStep == 17) {
                updateHeightAndBalanceFactor(mRoot);
            }
            resetNodeState();
            break;
        }

        case 13: {
            moveTreeAnimation(true, 1.5, 14);
            break;
        }

        case 14: {
            balanceTree();
            resetNodeState();
            mAnimationStep = 15;
            break;
        }

        case 15: {
            moveTreeAnimation(true, 1.5, 16);
            break;
        }

        case 16: {
            if (mNodeForRotate->balanceFactor > 1) {
                if (mNodeForRotate->parent) {
                    if (mNodeForRotate->isLeft) mNodeForRotate->parent->left = rotateLeft(mNodeForRotate);
                    else mNodeForRotate->parent->right = rotateLeft(mNodeForRotate);
                }
                else mRoot = rotateLeft(mNodeForRotate);
                std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
            }
            else if (mNodeForRotate->balanceFactor < -1) {
                if (mNodeForRotate->parent) {
                    if (mNodeForRotate->isLeft) mNodeForRotate->parent->left = rotateRight(mNodeForRotate);
                    else mNodeForRotate->parent->right = rotateRight(mNodeForRotate);
                }
                else mRoot = rotateRight(mNodeForRotate);
                std::swap(mSceneLayers[Nodes]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[Nodes]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                std::swap(mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[LeftEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
                std::swap(mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->nodeIndex], mSceneLayers[RightEdges]->getChildren()[mNodeForRotate->parent->nodeIndex]);
            }
            resetNodeState();
            updateHeightAndBalanceFactor(mRoot);
            mAnimationStep = 17;
            break;
        }

        case 17: {
            moveTreeAnimation(true, 1.5, 18);
            break;
        }

        case 18: {
            balanceTree();
            resetNodeState();
            mAnimationStep = 19;
            break;
        }

        case 19: {
            moveTreeAnimation(true, 1.5, 20);
            break;
        }


        case 20: {
            getBalanceFactorPath(mNodeForRotate, mRoot);
            resetNodeState();
            mAnimationStep = 21;
            break;
        }

        case 21: {
            checkBalanceFactorAnimation(true, 2, 23);
            break;
        }

        case 22: {
            changeNodeAnimation(true, 2, 23);
            break;
        }

        case 23: {
            if (mInputQueue.size() > 1) {
                mInputQueue.pop();
                resetAnimation();
            }
            else mIsReplay = true;
            break;
        }
    };
    mIsAnimationPaused = mIsStepByStepMode;
}


void AVL::traverseAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    if (!mTraverseControler.first) {
        mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->change3Color(
            Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
        mTravelPath[mTravelIndex]->isNodeHighlighted = true;
    }
    else if (!mTraverseControler.second) {
        mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->change3Color(
            Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
    }
    else {
        if (mIsLeftPath[mTravelIndex]) {
            mSceneLayers[LeftEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->change1Color(
                sf::Color(255, 171, 25), speed
            );
            mTravelPath[mTravelIndex]->isLeftEdgeHighlighted = true;
        }
        else {
            mSceneLayers[RightEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->change1Color(
                sf::Color(255, 171, 25), speed
            );
            mTravelPath[mTravelIndex]->isRightEdgeHighlighted = true;
        }
    }  

    if (!mTraverseControler.first) {
        if (mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->isChange3ColorFinished()) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->resetAnimationVar();
            mTraverseControler.first = true;
        }
    }   
    else if (!mTraverseControler.second && !mIsAnimationPaused) {
        if (mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->isChange3ColorFinished()) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->resetAnimationVar();
            mTraverseControler.second = true;
            if (mTravelIndex >= mTravelPath.size() - 1) {
                for (auto &node : mNodeList) {
                    if (node->val == mInputQueue.front()) {
                        mTraverseControler = std::make_pair(false, false);
                        mTravelIndex = 0;
                        mTravelPath.clear();
                        mIsLeftPath.clear();
                        resetNodeState();
                        mAnimationStep = animationStepAfterFinish;
                        break;
                    }
                }
            }
        }
    } 
    else if (!mIsAnimationPaused) {
        if (mIsLeftPath[mTravelIndex]) {
            if (mSceneLayers[LeftEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->isChange1ColorFinished()) {
                mSceneLayers[LeftEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->resetAnimationVar();
                mTravelIndex++;
                mTraverseControler = std::make_pair(false, false);
                if (mTravelIndex >= mTravelPath.size()) {
                    mTravelIndex = 0;
                    mTravelPath.clear();
                    mIsLeftPath.clear();
                    mAnimationStep = animationStepAfterFinish;
                }
            }
        }
        else {
            if (mSceneLayers[RightEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->isChange1ColorFinished()) {
                mSceneLayers[RightEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->resetAnimationVar();
                mTravelIndex++;
                mTraverseControler = std::make_pair(false, false);
                if (mTravelIndex >= mTravelPath.size()) {
                    mTravelIndex = 0;
                    mTravelPath.clear();
                    mIsLeftPath.clear();
                    resetNodeState();
                    mAnimationStep = animationStepAfterFinish;
                }
            }
        }
    }
}

void AVL::checkBalanceFactorAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    if (!mTraverseControler.first) { 
        mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->change3Color(
            Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
        mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->setLabel(std::to_string(-mTravelPath[mTravelIndex]->balanceFactor));
        mTravelPath[mTravelIndex]->isNodeHighlighted = true;
    }
    else { 
        mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->change3Color(
            Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
    }

    if (!mTraverseControler.first) {
        if (mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->isChange3ColorFinished()) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->resetAnimationVar();
            mTraverseControler.first = true;
        }
    }   
    else if (!mIsAnimationPaused) {
        if (mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->isChange3ColorFinished()) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->resetAnimationVar();
            mTraverseControler.first = false;
            if (!mIsReversed && mTravelIndex >= mTravelPath.size() - 1) {
                mTraverseControler = std::make_pair(false, false);
                mTravelPath.clear();
                mIsLeftPath.clear();
                resetNodeState();
                mAnimationStep = animationStepAfterFinish;
            }
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->setLabel("");
            mTravelIndex++;
        }
    } 
}

void AVL::nodeAppearAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    if (mSceneLayers[Nodes]->getChildren().size() < mNodeList.size() && !mIsReversed) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        node->set(
            std::to_string(mOperationNode->val), mOperationNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), 0,
            Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
        );
        mSceneLayers[Nodes]->attachChild(std::move(node));

        std::unique_ptr<Edge> leftEdge = std::make_unique<Edge>();
        leftEdge->set(
            mOperationNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), 
            mOperationNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS)
        );
        mSceneLayers[LeftEdges]->attachChild(std::move(leftEdge));

        std::unique_ptr<Edge> rightEdge = std::make_unique<Edge>();
        rightEdge->set(
            mOperationNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), 
            mOperationNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS) 
        );
        mSceneLayers[RightEdges]->attachChild(std::move(rightEdge));
    }
    
    if (!mIsReversed) mSceneLayers[Nodes]->getChildren().back()->zoom(sf::Vector2f(Size::NODE_RADIUS, 0), speed);

    if (mSceneLayers[Nodes]->getChildren().back()->isZoomFinished() && !mIsAnimationPaused) {
        if (animationStepAfterFinish != 0) {
            mAnimationStep = animationStepAfterFinish;
            resetNodeState();
        }
    }
}

void AVL::moveTreeAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    int index = 0;
    for (auto &child : mSceneLayers[Nodes]->getChildren()) {
        child->move(mNodeList[index]->position, speed);
        index++;
    }
    index = 0;
    for (auto &child : mSceneLayers[LeftEdges]->getChildren()) {
        child->moveBy2Points(
            mNodeList[index]->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
            (mNodeList[index]->left ? mNodeList[index]->left->position : mNodeList[index]->position) + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
            speed
        );
        index++;
    }

    index = 0;
    for (auto &child : mSceneLayers[RightEdges]->getChildren()) {
        child->moveBy2Points(
            mNodeList[index]->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
            (mNodeList[index]->right ? mNodeList[index]->right->position : mNodeList[index]->position) + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
            speed
        );
        index++;
    }

    if (animationStepAfterFinish != 0 && !mIsAnimationPaused) {
        bool isFinishedAll = true;
        for (auto &child : mSceneLayers[Nodes]->getChildren()) {
            if (!child->isMoveFinished()) {
                isFinishedAll = false;
                break;
            }
        }
        for (auto &child : mSceneLayers[LeftEdges]->getChildren()) {
            if (!child->isMoveBy2PointsFinished()) {
                isFinishedAll = false;
                break;
            }
        }
        for (auto &child : mSceneLayers[RightEdges]->getChildren()) {
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


void AVL::changeNodeAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->change3Color(
        Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, 3
    );

    if (mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->isChange3ColorFinished() && !mIsAnimationPaused) {
        mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->resetAnimationVar();
        mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->setContent(std::to_string(mOperationNode->val) + (mOperationNode->duplicate > 1 ? "-" + std::to_string(mOperationNode->duplicate) : ""));
        mAnimationStep = animationStepAfterFinish;
    }
}

void AVL::resetAnimation() {
    mAnimationStep = 1;
    mOperationNode = nullptr;
    mReplaceNode = nullptr;
    mOperationIndex = -1;
    mReplaceIndex = -1;
    for (auto &child : mSceneLayers[Nodes]->getChildren()) child->resetAnimationVar();
    for (auto &child : mSceneLayers[LeftEdges]->getChildren()) child->resetAnimationVar();
    for (auto &child : mSceneLayers[RightEdges]->getChildren()) child->resetAnimationVar();
}

bool AVL::isProcessingAnimation() {
    if (!mIsReversed && (mAnimationStep == 2 || mAnimationStep == 4)) return false;
    if (mIsReversed && (mAnimationStep == 6 || mAnimationStep == 3)) return false;
    for (auto &child : mSceneLayers[Nodes]->getChildren()) 
        if (child->isProcessing()) return true;
    for (auto &child : mSceneLayers[LeftEdges]->getChildren())
        if (child->isProcessing()) return true;
    for (auto &child : mSceneLayers[RightEdges]->getChildren())
        if (child->isProcessing()) return true;
    return false;
}

void AVL::resetNodeState() {
    for (auto &child : mSceneLayers[Nodes]->getChildren()) {
        child->resetAnimationVar();
    }
    for (auto &child : mSceneLayers[LeftEdges]->getChildren()) {
        child->resetAnimationVar();
    }
    for (auto &child : mSceneLayers[RightEdges]->getChildren()) {
        child->resetAnimationVar();
    }
}