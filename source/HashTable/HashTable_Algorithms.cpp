#include <State/HashTable.hpp>

void HashTable::createRandomTree() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(7, 21);
    mInputSize = dis(gen);
    mNodeList.clear();
    mNodeList.resize(mInputSize);
    for (auto &node : mNodeList) {
        node.val = -1;
        node.isNodeHighlighted = false;
        node.position = sf::Vector2f(0, 0);
    }
    mSceneLayers[CreateOptions]->getChildren()[SizeInputBox]->setContent(std::to_string(mInputSize));
    setTreeScale(mInputSize);
    balanceTree();
    createTree();
}

void HashTable::insert(int val) {
    int index = val % mInputSize;
    while (mNodeList[index].val != -1) {
        index = (index + 1) % mInputSize;
    }
    mNodeList[index].val = val;
}

void HashTable::setTreeScale(int size) {
    Size::NODE_RADIUS = 40.f * Constant::SCALE_X;
    Size::NODE_RADIUS_X = 40.f * Constant::SCALE_X;
    Size::NODE_RADIUS_Y = 40.f * Constant::SCALE_Y;
}

void HashTable::balanceTree() {
    sf::Vector2f startPos = sf::Vector2f(Constant::WINDOW_WIDTH/2 - (mInputSize * (Size::NODE_RADIUS * 17 / 16)), 250 * Constant::SCALE_Y);
    for (int i = 0; i < mInputSize; i++) {
        mNodeList[i].position = startPos + sf::Vector2f(i * (Size::NODE_RADIUS * 2 + Size::NODE_RADIUS / 4), 0);
    }
}

HashTable::TreeState* HashTable::createTreeState(int animationIndex) {
    TreeState* treeState = new TreeState();
    treeState->nodeList = mNodeList;
    treeState->operationIndex = mOperationIndex;
    treeState->animationIndex = animationIndex;
    return treeState;
}

void HashTable::applyTreeState(TreeState* treeState) {
    mNodeList = treeState->nodeList;
    mOperationIndex = treeState->operationIndex;
    mAnimationStep = treeState->animationIndex;
}

void HashTable::returnToPreviousStep() {
    applyTreeState(mTreeForBackward.top());
    mTreeForBackward.pop();
    createTree();
}