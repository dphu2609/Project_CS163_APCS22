#include <State/BST.hpp>

void BST::createTree() {
    clear(mRoot);
    mNodeList.clear();
    mSceneLayers[Nodes]->clearChildren();
    mSceneLayers[LeftEdges]->clearChildren();
    mSceneLayers[RightEdges]->clearChildren();
    for (int i = 0; i < mInputData.size(); i++) {
        insert(mRoot, nullptr, mInputData[i]);
    }
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