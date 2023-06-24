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
        leftEdge->setPositionByPoints(sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS), (mNodeList[i]->left ? sf::Vector2f(mNodeList[i]->left->position.x + Size::NODE_RADIUS, mNodeList[i]->left->position.y + Size::NODE_RADIUS) : sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS)));
        mSceneLayers[LeftEdges]->attachChild(std::move(leftEdge));

        std::unique_ptr<Edge> rightEdge = std::make_unique<Edge>();
        rightEdge->setPositionByPoints(sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS), (mNodeList[i]->right ? sf::Vector2f(mNodeList[i]->right->position.x + Size::NODE_RADIUS, mNodeList[i]->right->position.y + Size::NODE_RADIUS) : sf::Vector2f(mNodeList[i]->position.x + Size::NODE_RADIUS, mNodeList[i]->position.y + Size::NODE_RADIUS)));
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
                        mAnimationStep++;
                    }
                }
            } 
            else {
                if (mIsLeftPath[mTravelIndex]) {
                    if (mSceneLayers[LeftEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->isChange1ColorFinished()) {
                        mSceneLayers[LeftEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->resetAnimationVar();
                        mTravelIndex++;
                        mTraverseControler = std::make_pair(false, false);
                    }
                }
                else {
                    if (mSceneLayers[RightEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->isChange1ColorFinished()) {
                        mSceneLayers[RightEdges]->getChildren()[mTravelPath[mTravelIndex]->nodeIndex]->resetAnimationVar();
                        mTravelIndex++;
                        mTraverseControler = std::make_pair(false, false);
                    }
                }
            }
            break;
        }

        case 4: {
            if (mInputQueue.size() > 1) mInputQueue.pop();
            mAnimationStep = 1;
            mTravelIndex = 0;
            mTravelPath.clear();
            mIsLeftPath.clear();
            mTraverseControler = std::make_pair(false, false);
        }
    };
}