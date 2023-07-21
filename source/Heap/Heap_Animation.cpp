#include <State/Heap.hpp>

void Heap::createTree() {
    mSceneLayers[Nodes]->clearChildren();
    mSceneLayers[LeftEdges]->clearChildren();
    mSceneLayers[RightEdges]->clearChildren();
    for (int i = 0; i < mNodeList.size(); i++) {
        if (mNodeList[i]->isInsertNode) {
            std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
            node->set(
                std::to_string(mNodeList[i]->val), 
                mNodeList[i]->position, Size::NODE_RADIUS, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, 
                Size::NODE_RADIUS / 8, std::to_string(mNodeList[i]->order)
            );
            mSceneLayers[Nodes]->attachChild(std::move(node));
        }
        else if (!mNodeList[i]->isNodeHighlighted) {
            std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
            node->set(
                std::to_string(mNodeList[i]->val),
                mNodeList[i]->position, Size::NODE_RADIUS, Color::NODE_COLOR, Color::NODE_TEXT_COLOR, Color::NODE_OUTLINE_COLOR,
                Size::NODE_RADIUS / 8, std::to_string(mNodeList[i]->order)
            );
            mSceneLayers[Nodes]->attachChild(std::move(node));
        }
        else {
            std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
            node->set(
                std::to_string(mNodeList[i]->val), 
                mNodeList[i]->position, Size::NODE_RADIUS, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR,
                Size::NODE_RADIUS / 8, std::to_string(mNodeList[i]->order)
            );
            mSceneLayers[Nodes]->attachChild(std::move(node));
        }

        std::unique_ptr<Edge> leftEdge = std::make_unique<Edge>();
        leftEdge->set(sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS_X, mNodeList[i]->position.y + Size::NODE_RADIUS_Y), (mNodeList[i]->left ? sf::Vector2f(mNodeList[i]->left->position.x + Size::NODE_RADIUS_X, mNodeList[i]->left->position.y + Size::NODE_RADIUS_Y) : sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS_X, mNodeList[i]->position.y + Size::NODE_RADIUS_Y)));
        mSceneLayers[LeftEdges]->attachChild(std::move(leftEdge));

        std::unique_ptr<Edge> rightEdge = std::make_unique<Edge>();
        rightEdge->set(sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS_X, mNodeList[i]->position.y + Size::NODE_RADIUS_Y), (mNodeList[i]->right ? sf::Vector2f(mNodeList[i]->right->position.x + Size::NODE_RADIUS_X, mNodeList[i]->right->position.y + Size::NODE_RADIUS_Y) : sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS_X, mNodeList[i]->position.y + Size::NODE_RADIUS_Y)));
        mSceneLayers[RightEdges]->attachChild(std::move(rightEdge));
    }
}

void Heap::insertAnimation() {
    switch (mAnimationStep) {
        case 1: {
            createBackupTree();
            createTree();
            mAnimationStep = 2;
            break;
        }

        case 2: {
            insert(mRoot, mNodeList, mInputQueue.front(), false);
            mOperationNode = mNodeList.back();
            mOperationNode->isInsertNode = true;
            balanceTree();
            mAnimationStep = 3;
            break;
        }

        case 3: {
            nodeAppearAnimation(true, 1.5);
            moveTreeAnimation(true, 1.5, 4);
            break;
        }

        case 4: {
            if (mOperationNode->parent && ((mIsMaxHeap && mOperationNode->val > mOperationNode->parent->val) || (!mIsMaxHeap && mOperationNode->val < mOperationNode->parent->val))) {
                std::swap(mOperationNode->val, mOperationNode->parent->val);
                std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mOperationNode->parent->nodeIndex]);
                mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->setLabel(std::to_string(mOperationNode->order));
                mSceneLayers[Nodes]->getChildren()[mOperationNode->parent->nodeIndex]->setLabel(std::to_string(mOperationNode->parent->order));
                mOperationNode = mOperationNode->parent;
                mAnimationStep = 5;
            }
            else mAnimationStep = 6;
            break;
        }

        case 5: {
            moveTreeAnimation(true, 1.5, 4);
            break;
        }

        case 6: {
            if (mInputQueue.size() > 1) {
                mInputQueue.pop();
            }
            else mIsReplay = true;
            break;
        }
    }
}

void Heap::deleteAnimation() {
    switch (mAnimationStep) {
        case 1: {
            createBackupTree();
            createTree();
            mAnimationStep = 2;
            break;
        }
        
        case 2: {
            std::swap(mNodeList[mInputQueue.front() - 1]->val, mNodeList.back()->val);
            std::swap(mSceneLayers[Nodes]->getChildren()[mInputQueue.front() - 1], mSceneLayers[Nodes]->getChildren().back());
            mSceneLayers[Nodes]->getChildren()[mInputQueue.front() - 1]->setLabel(std::to_string(mNodeList[mInputQueue.front() - 1]->order));
            mSceneLayers[Nodes]->getChildren().back()->setLabel(std::to_string(mNodeList.back()->order));
            if (mInputQueue.front() != mNodeList.size())
                mOperationNode = mNodeList[mInputQueue.front() - 1];
            else mOperationNode = nullptr;
            mAnimationStep = 3;
            break;
        }

        case 3: {
            moveTreeAnimation(true, 1.5, 4);
            break;
        }

        case 4: {
            if (mNodeList.back()->parent) {
                if (mNodeList.back()->order % 2 == 0) {
                    mNodeList.back()->parent->left = nullptr;
                }
                else {
                    mNodeList.back()->parent->right = nullptr;
                }
            }
            Node* temp = mNodeList.back();
            mNodeList.pop_back();
            delete temp;
            balanceTree();
            mAnimationStep = 5;
            break;
        }

        case 5: {
            mSceneLayers[Nodes]->getChildren().back()->zoom(sf::Vector2f(0, 0), 1.5);
            moveTreeAnimation(true, 1.5);
            if (mSceneLayers[Nodes]->getChildren().back()->isZoomFinished()) {
                mSceneLayers[Nodes]->getChildren().pop_back();
                mSceneLayers[LeftEdges]->getChildren().pop_back();
                mSceneLayers[RightEdges]->getChildren().pop_back();
                if (mOperationNode) mAnimationStep = 6;
                else mAnimationStep = 8;
                resetNodeState();
            }
            break;
        }

        case 6: {
            if (mIsMaxHeap) {
                if ((mOperationNode->left && mOperationNode->val < mOperationNode->left->val) || (mOperationNode->right && mOperationNode->val < mOperationNode->right->val)) {
                    if (mOperationNode->left && mOperationNode->right && mOperationNode->left->val < mOperationNode->right->val) {
                        std::swap(mOperationNode->val, mOperationNode->right->val);
                        std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mOperationNode->right->nodeIndex]);
                        mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->setLabel(std::to_string(mOperationNode->order));
                        mSceneLayers[Nodes]->getChildren()[mOperationNode->right->nodeIndex]->setLabel(std::to_string(mOperationNode->right->order));
                        mOperationNode = mOperationNode->right;
                    }
                    else if (mOperationNode->left) {
                        std::swap(mOperationNode->val, mOperationNode->left->val);
                        std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mOperationNode->left->nodeIndex]);
                        mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->setLabel(std::to_string(mOperationNode->order));
                        mSceneLayers[Nodes]->getChildren()[mOperationNode->left->nodeIndex]->setLabel(std::to_string(mOperationNode->left->order));
                        mOperationNode = mOperationNode->left;
                    }
                    mAnimationStep = 7;
                }
                else {
                    mAnimationStep = 8;
                }
            }
            else {
                if ((mOperationNode->left && mOperationNode->val > mOperationNode->left->val) || (mOperationNode->right && mOperationNode->val > mOperationNode->right->val)) {
                    if (mOperationNode->left && mOperationNode->right && mOperationNode->left->val > mOperationNode->right->val) {
                        std::swap(mOperationNode->val, mOperationNode->right->val);
                        std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mOperationNode->right->nodeIndex]);
                        mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->setLabel(std::to_string(mOperationNode->order));
                        mSceneLayers[Nodes]->getChildren()[mOperationNode->right->nodeIndex]->setLabel(std::to_string(mOperationNode->right->order));
                        mOperationNode = mOperationNode->right;
                    }
                    else if (mOperationNode->left) {
                        std::swap(mOperationNode->val, mOperationNode->left->val);
                        std::swap(mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex], mSceneLayers[Nodes]->getChildren()[mOperationNode->left->nodeIndex]);
                        mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->setLabel(std::to_string(mOperationNode->order));
                        mSceneLayers[Nodes]->getChildren()[mOperationNode->left->nodeIndex]->setLabel(std::to_string(mOperationNode->left->order));
                        mOperationNode = mOperationNode->left;
                    }
                    mAnimationStep = 7;
                }
                else {
                    mAnimationStep = 8;
                }
            }
            break;
        }

        case 7: {
            moveTreeAnimation(true, 1.5, 6);
            break;
        }

        case 8: {
            if (mInputQueue.size() > 1) {
                mInputQueue.pop();
            }
            else mIsReplay = true;
            break;
        }
    }
}

void Heap::nodeAppearAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    if (mSceneLayers[Nodes]->getChildren().size() < mNodeList.size() && !mIsReversed) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        node->set(
            std::to_string(mOperationNode->val), mOperationNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), 0,
            Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR,
            0, std::to_string(mNodeList.back()->order)
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
    
    mSceneLayers[Nodes]->getChildren().back()->zoom(sf::Vector2f(Size::NODE_RADIUS, 0), speed);

    if (mSceneLayers[Nodes]->getChildren().back()->isZoomFinished() && !mIsAnimationPaused) {
        if (animationStepAfterFinish != 0) {
            mAnimationStep = animationStepAfterFinish;
            resetNodeState();
        }
    }
}

void Heap::moveTreeAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;

    for (auto &node : mNodeList) {
        mSceneLayers[Nodes]->getChildren()[node->nodeIndex]->move(node->position, speed);
        mSceneLayers[LeftEdges]->getChildren()[node->nodeIndex]->moveBy2Points(
            node->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
            (node->left ? node->left->position : node->position) + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
            speed
        );
        mSceneLayers[RightEdges]->getChildren()[node->nodeIndex]->moveBy2Points(
            node->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
            (node->right ? node->right->position : node->position) + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
            speed
        );
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


void Heap::changeNodeAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->change3Color(
        Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, 3
    );

    if (mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->isChange3ColorFinished() && !mIsAnimationPaused) {
        mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->resetAnimationVar();
        mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->setContent(std::to_string(mOperationNode->val));
        mAnimationStep = animationStepAfterFinish;
    }
}

void Heap::resetAnimation() {
    mAnimationStep = 1;
    mOperationNode = nullptr;
    for (auto &child : mSceneLayers[Nodes]->getChildren()) child->resetAnimationVar();
    for (auto &child : mSceneLayers[LeftEdges]->getChildren()) child->resetAnimationVar();
    for (auto &child : mSceneLayers[RightEdges]->getChildren()) child->resetAnimationVar();
    for (auto &child : mNodeList) {
        child->isNodeHighlighted = false;
        child->isInsertNode = false;
    }
    setTreeScale(mNodeList.size());
}

bool Heap::isProcessingAnimation() {
    for (auto &child : mSceneLayers[Nodes]->getChildren()) 
        if (child->isProcessing()) return true;
    for (auto &child : mSceneLayers[LeftEdges]->getChildren())
        if (child->isProcessing()) return true;
    for (auto &child : mSceneLayers[RightEdges]->getChildren())
        if (child->isProcessing()) return true;
    return false;
}

void Heap::resetNodeState() {
    for (auto &child : mSceneLayers[Nodes]->getChildren()) child->resetAnimationVar();
    for (auto &child : mSceneLayers[LeftEdges]->getChildren()) child->resetAnimationVar();
    for (auto &child : mSceneLayers[RightEdges]->getChildren()) child->resetAnimationVar();
}