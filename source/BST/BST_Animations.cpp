#include <State/BST.hpp>

void BST::createTree() {
    mSceneLayers[Nodes]->clearChildren();
    mSceneLayers[LeftEdges]->clearChildren();
    mSceneLayers[RightEdges]->clearChildren();
    for (int i = 0; i < mNodeList.size(); i++) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        node->set(std::to_string(mNodeList[i]->val) + (mNodeList[i]->duplicate > 1 ? ("-" + std::to_string(mNodeList[i]->duplicate)) : ""), mNodeList[i]->position);
        mSceneLayers[Nodes]->attachChild(std::move(node));

        std::unique_ptr<Edge> leftEdge = std::make_unique<Edge>();
        leftEdge->set(sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS), (mNodeList[i]->left ? sf::Vector2f(mNodeList[i]->left->position.x + Size::NODE_RADIUS, mNodeList[i]->left->position.y + Size::NODE_RADIUS) : sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS)));
        mSceneLayers[LeftEdges]->attachChild(std::move(leftEdge));

        std::unique_ptr<Edge> rightEdge = std::make_unique<Edge>();
        rightEdge->set(sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS), (mNodeList[i]->right ? sf::Vector2f(mNodeList[i]->right->position.x + Size::NODE_RADIUS, mNodeList[i]->right->position.y + Size::NODE_RADIUS) : sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS)));
        mSceneLayers[RightEdges]->attachChild(std::move(rightEdge));
    }
}

void BST::insertAnimation() {
    switch(mAnimationStep) {
        case 1: {
            createTree();
            mAnimationStep++;
            break;
        }
        case 2: {
            getTravelPath(mRoot, mInputQueue.front());
            mAnimationStep++;
            break;
        }
        case 3: {
            traverseAnimation(3, 4);
            break;
        }

        case 4: {
            insert(mInputQueue.front());
            if (mOperationNode->duplicate > 1) mAnimationStep = 6;
            else mAnimationStep = 5;
            break;
        }

        case 5: { //normal case
            nodeAppearAnimation(2, 7);
            moveTreeAnimation(2);
            break;
        }

        case 6: {
            changeNodeAnimation(2, 7);
            break;
        }

        case 7: {
            if (mInputQueue.size() > 1) mInputQueue.pop();
            else mInsertAnimation = false;
            resetAnimation();
            break;
        }
    };
}

void BST::deleteAnimation() {
    switch (mAnimationStep) {
        case 1: {
            createTree();
            mAnimationStep++;
            break;
        }

        case 2: {
            find2NodesForDelete(mInputQueue.front());
            getTravelPath(mRoot, mInputQueue.front());
            mAnimationStep++;
            break;
        }

        case 3: {
            traverseAnimation(2, 4);
            break;
        }

        case 4: {
            if (mReplaceNode) {
                getTravelPath(mOperationNode, mReplaceNode->val);
                mAnimationStep++;
            }
            else if (mOperationNode->duplicate > 1) {
                mOperationNode->duplicate--;
                mAnimationStep = 9;
            }
            else mAnimationStep = 6;
            break;
        }

        case 5: {
            traverseAnimation(2, 6);
            break;
        }

        case 6: {
            deleteNodeAnimation(1.5, 7);
            break;
        }

        case 7: {
            deleteNode();
            mAnimationStep++;
            break;
        }

        case 8: {
            moveTreeAnimation(1.5, 10);
            break;
        }

        case 9: {
            changeNodeAnimation(2, 10);
            break;
        }

        case 10: {
            if (mInputQueue.size() > 1) mInputQueue.pop();
            else mDeleteAnimation = false;
            resetAnimation();
            break;
        }

    }
}

void BST::traverseAnimation(float speed, int animationStepAfterFinish) {
    if (!mTraverseControler.first) 
        mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->change3Color(
            sf::Color(255, 171, 25), sf::Color::White,  sf::Color(255, 171, 25), 3
        );
    else if (!mTraverseControler.second) 
        mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->change3Color(
            sf::Color::White, sf::Color(255, 171, 25), sf::Color(255, 171, 25), 3
        );
    else {
        if (mIsLeftPath[mTravelIndex]) 
            mSceneLayers[LeftEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->change1Color(
                sf::Color(255, 171, 25), 3
            );
        else 
            mSceneLayers[RightEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->change1Color(
                sf::Color(255, 171, 25), 3
            );
    }  

    if (!mTraverseControler.first) {
        if (mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->isChange3ColorFinished()) {
            mSceneLayers[Nodes]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->resetAnimationVar();
            mTraverseControler.first = true;
        }
    }   
    else if (!mTraverseControler.second) {
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
                        mAnimationStep = animationStepAfterFinish;
                        break;
                    }
                }
            }
        }
    } 
    else {
        if (mIsLeftPath[mTravelIndex]) {
            if (mSceneLayers[LeftEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->isChange1ColorFinished()) {
                mSceneLayers[LeftEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->resetAnimationVar();
                mTravelIndex++;
                mTraverseControler = std::make_pair(false, false);
                if (mTravelIndex >= mTravelPath.size()) {
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
                    mAnimationStep = animationStepAfterFinish;
                }
            }
        }
    }
}

void BST::nodeAppearAnimation(float speed, int animationStepAfterFinish) {
    if (mSceneLayers[Nodes]->getChildren().size() < mNodeList.size()) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        node->set(
            std::to_string(mOperationNode->val), mOperationNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), 0,
            sf::Color(255, 171, 25), sf::Color::White,  sf::Color(255, 171, 25)
        );
        mSceneLayers[Nodes]->attachChild(std::move(node));
    }

    mSceneLayers[Nodes]->getChildren().back()->zoom(sf::Vector2f(Size::NODE_RADIUS, 0), speed);

    if (mSceneLayers[Nodes]->getChildren().back()->isZoomFinished()) {
        mAnimationStep = animationStepAfterFinish;
    }
}

void BST::moveTreeAnimation(float speed, int animationStepAfterFinish) {
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

    if (animationStepAfterFinish != 0) {
        bool isFinishedAll = true;
        for (auto &child : mSceneLayers[Nodes]->getChildren()) {
            if (!child->isMoveFinished()) {
                isFinishedAll = false;
                break;
            }
        }
        if (mDeleteAnimation && isFinishedAll) {
            mAnimationStep = animationStepAfterFinish;
        }
    }
}


void BST::changeNodeAnimation(float speed, int animationStepAfterFinish) {
    mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->change3Color(
        sf::Color(255, 171, 25), sf::Color::White,  sf::Color(255, 171, 25), 3
    );

    if (mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->isChange3ColorFinished()) {
        mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->resetAnimationVar();
        mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->setContent(std::to_string(mOperationNode->val) + (mOperationNode->duplicate > 1 ? "-" + std::to_string(mOperationNode->duplicate) : ""));
        mAnimationStep = animationStepAfterFinish;
    }
}

void BST::deleteNodeAnimation(float speed, int animationStepAfterFinish) {
    mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->zoom(sf::Vector2f(0, 0), speed);

    if (!mReplaceNode) {
        if (mOperationNode->isLeft) {
            mSceneLayers[LeftEdges]->getChildren()[mOperationNode->parent->nodeIndex]->moveBy2Points(
                mOperationNode->parent->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
                mOperationNode->parent->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), speed
            );
        }
        else {
            mSceneLayers[RightEdges]->getChildren()[mOperationNode->parent->nodeIndex]->moveBy2Points(
                mOperationNode->parent->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
                mOperationNode->parent->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), speed
            );
        }
    }
    else {
        if (mOperationNode->left) {
            mSceneLayers[LeftEdges]->getChildren()[mOperationNode->nodeIndex]->moveBy2Points(
                mReplaceNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
                mOperationNode->left->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), speed
            );

            if (mReplaceNode->left && mReplaceNode->parent->right) {
                mSceneLayers[RightEdges]->getChildren()[mReplaceNode->parent->nodeIndex]->moveBy2Points(
                    mReplaceNode->parent->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
                    mReplaceNode->left->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), speed
                );
            }
        }

        if (mOperationNode->right) {
            mSceneLayers[RightEdges]->getChildren()[mOperationNode->nodeIndex]->moveBy2Points( 
                mReplaceNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
                mOperationNode->right->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), speed
            );

            if (mReplaceNode->right && mReplaceNode->parent->left) {
                mSceneLayers[LeftEdges]->getChildren()[mReplaceNode->parent->nodeIndex]->moveBy2Points(
                    mReplaceNode->parent->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
                    mReplaceNode->right->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), speed
                );
            }
        }

        if (mOperationNode->parent) {
            if (mOperationNode->isLeft) {
                mSceneLayers[LeftEdges]->getChildren()[mOperationNode->parent->nodeIndex]->moveBy2Points(
                    mOperationNode->parent->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
                    mReplaceNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), speed
                );
            }
            else {
                mSceneLayers[RightEdges]->getChildren()[mOperationNode->parent->nodeIndex]->moveBy2Points(
                    mOperationNode->parent->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS),
                    mReplaceNode->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), speed
                );
            }
        }
    }

    if (mSceneLayers[Nodes]->getChildren()[mOperationNode->nodeIndex]->isZoomFinished()) {
        mAnimationStep = animationStepAfterFinish;
        for (auto &child : mSceneLayers[Nodes]->getChildren()) child->resetAnimationVar();
        for (auto &child : mSceneLayers[LeftEdges]->getChildren()) child->resetAnimationVar();
        for (auto &child : mSceneLayers[RightEdges]->getChildren()) child->resetAnimationVar();
    }
}

void BST::resetAnimation() {
    mAnimationStep = 1;
    mOperationNode = nullptr;
    mReplaceNode = nullptr;
    for (auto &child : mSceneLayers[Nodes]->getChildren()) child->resetAnimationVar();
    for (auto &child : mSceneLayers[LeftEdges]->getChildren()) child->resetAnimationVar();
    for (auto &child : mSceneLayers[RightEdges]->getChildren()) child->resetAnimationVar();
}