#include <State/HashTable.hpp>

void HashTable::createTree() {
    mSceneLayers[Nodes]->getChildren().clear();
    for (int i = 0; i < mNodeList.size(); i++) {
        std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>();
        if (!mNodeList[i].isNodeHighlighted) {
            node->set(false, std::to_string(mNodeList[i].val), mNodeList[i].position);
        }
        else {
            node->set(
                false, std::to_string(mNodeList[i].val), mNodeList[i].position, Size::NODE_RADIUS,
                Color::NODE_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
            );
        }
        node->setLabel(std::to_string(i));
        mSceneLayers[Nodes]->attachChild(std::move(node));
    }
}

void HashTable::insertAnimation() {
    switch (mAnimationStep) {
        case 1: {
            resetAnimation();
            createTree();
            mTreeStateForReplay = createTreeState(1);
            mOperationIndex = mInputQueue.front() % mInputSize;
            mSceneLayers[CodeBox]->getChildren()[0]->setMultipleContent(CodeContainer::codeHolder[Code::InsertHashTable]);
            mSceneLayers[CodeBox]->getChildren()[0]->activateLine({0});
            mAnimationStep = 2;
            break;
        }

        case 2: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(2));
            if (mNodeList[mOperationIndex].val == -1) {
                mNodeList[mOperationIndex].val = mInputQueue.front();
                mSceneLayers[CodeBox]->getChildren()[0]->activateLine({1, 2});
                mAnimationStep = 3;
            }
            else {
                mSceneLayers[CodeBox]->getChildren()[0]->activateLine({3});
                mAnimationStep = 4;
            }
            break;
        }

        case 3: {
            changeNodeAnimation(true, 2, 6);
            break;
        }

        case 4: {
            traverseAnimation(true, 2, 5);
            break;
        }

        case 5: {
            mOperationIndex++;
            mOperationIndex %= mInputSize;
            bool isLoop = true;
            for (int i = 0; i < mNodeList.size(); i++) {
                if (!mNodeList[i].isNodeHighlighted) {
                    isLoop = false;
                    break;
                }
            }
            if (isLoop) {
                mAnimationStep = 6;
            }
            else {
                mAnimationStep = 2;
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

void HashTable::deleteAnimation() {
    switch (mAnimationStep) {
        case 1: {
            resetAnimation();
            createTree();
            mTreeStateForReplay = createTreeState(1);
            mOperationIndex = mInputQueue.front() % mInputSize;
            mSceneLayers[CodeBox]->getChildren()[0]->setMultipleContent(CodeContainer::codeHolder[Code::DeleteHashTable]);
            mSceneLayers[CodeBox]->getChildren()[0]->activateLine({0});
            mAnimationStep = 2;
            break;
        }

        case 2: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(2));
            if (mNodeList[mOperationIndex].val == mInputQueue.front()) {
                mNodeList[mOperationIndex].val = -1;
                mSceneLayers[CodeBox]->getChildren()[0]->activateLine({5});
                mAnimationStep = 3;
            }
            else {
                mSceneLayers[CodeBox]->getChildren()[0]->activateLine({2, 3, 4});
                mAnimationStep = 4;
            }
            break;
        }

        case 3: {
            changeNodeAnimation(true, 2, 6);
            break;
        }

        case 4: {
            traverseAnimation(true, 2, 5);
            break;
        }

        case 5: {
            mOperationIndex++;
            mOperationIndex %= mInputSize;
            bool isLoop = true;
            for (int i = 0; i < mNodeList.size(); i++) {
                if (!mNodeList[i].isNodeHighlighted) {
                    isLoop = false;
                    break;
                }
            }
            if (isLoop) {
                mAnimationStep = 6;
            }
            else {
                mAnimationStep = 2;
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

void HashTable::searchAnimation() {
    switch (mAnimationStep) {
        case 1: {
            resetAnimation();
            createTree();
            mTreeStateForReplay = createTreeState(1);
            mOperationIndex = mInputQueue.front() % mInputSize;
            mSceneLayers[CodeBox]->getChildren()[0]->setMultipleContent(CodeContainer::codeHolder[Code::SearchHashTable]);
            mSceneLayers[CodeBox]->getChildren()[0]->activateLine({0});
            mAnimationStep = 2;
            break;
        }

        case 2: {
            if (!mIsReversed) mTreeForBackward.push(createTreeState(2));
            if (mNodeList[mOperationIndex].val == mInputQueue.front()) {
                mNodeList[mOperationIndex].val = mInputQueue.front();
                mSceneLayers[CodeBox]->getChildren()[0]->activateLine({5});
                mAnimationStep = 3;
            }
            else {
                mSceneLayers[CodeBox]->getChildren()[0]->activateLine({2, 3, 4});
                mAnimationStep = 4;
            }
            break;
        }

        case 3: {
            changeNodeAnimation(true, 2, 6);
            break;
        }

        case 4: {
            traverseAnimation(true, 2, 5);
            break;
        }

        case 5: {
            mOperationIndex++;
            mOperationIndex %= mInputSize;
            bool isLoop = true;
            for (int i = 0; i < mNodeList.size(); i++) {
                if (!mNodeList[i].isNodeHighlighted) {
                    isLoop = false;
                    break;
                }
            }
            if (isLoop) {
                mAnimationStep = 6;
            }
            else {
                mAnimationStep = 2;
            }
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

void HashTable::traverseAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    if (!mTraverseControler.first) {
        mSceneLayers[Nodes]->getChildren()[mOperationIndex]->change3Color(
            Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
    }
    else {
        mSceneLayers[Nodes]->getChildren()[mOperationIndex]->change3Color(
            Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
    }

    if (!mTraverseControler.first) {
        if (mSceneLayers[Nodes]->getChildren()[mOperationIndex]->isChange3ColorFinished()) {
            mSceneLayers[Nodes]->getChildren()[mOperationIndex]->resetAnimationVar();
            mNodeList[mOperationIndex].isNodeHighlighted = true;
            mTraverseControler.first = true;
        }
    }
    else if (!mIsAnimationPaused) {
        if (mSceneLayers[Nodes]->getChildren()[mOperationIndex]->isChange3ColorFinished()) {
            mSceneLayers[Nodes]->getChildren()[mOperationIndex]->resetAnimationVar();
            mTraverseControler.first = false;
            mAnimationStep = animationStepAfterFinish;
        }
    }
}

void HashTable::changeNodeAnimation(bool isAllowPause, float speed, int animationStepAfterFinish) {
    if (!isAllowPause) mIsAnimationPaused = false;
    mSceneLayers[Nodes]->getChildren()[mOperationIndex]->change3Color(
        Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
    );

    if (mSceneLayers[Nodes]->getChildren()[mOperationIndex]->isChange3ColorFinished() && !mIsAnimationPaused) {
        mSceneLayers[Nodes]->getChildren()[mOperationIndex]->resetAnimationVar();
        mSceneLayers[Nodes]->getChildren()[mOperationIndex]->setContent(std::to_string(mNodeList[mOperationIndex].val));
        mNodeList[mOperationIndex].isNodeHighlighted = true;
        mAnimationStep = animationStepAfterFinish;
    }
}

bool HashTable::isProcessingAnimation() {
    for (auto &child : mSceneLayers[Nodes]->getChildren()) {
        if (child->isProcessing()) return true;
    }
    return false;
}

void HashTable::resetNodeState() {
    for (auto &child : mSceneLayers[Nodes]->getChildren()) {
        child->resetAnimationVar();
    }
}

void HashTable::resetAnimation() {
    mOperationIndex = -1;
    for (auto &node : mNodeList) {
        node.isNodeHighlighted = false;
    }
    while (!mTreeForBackward.empty()) {
        delete mTreeForBackward.top();
        mTreeForBackward.pop();
    }
    mAnimationStep = 1;
}