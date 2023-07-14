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