#include <State/Graph.hpp>

void Graph::createTree() {
    mSceneLayers[Nodes]->getChildren().clear();
    mSceneLayers[Edges]->getChildren().clear();
    for (auto &node : mNodeList) {
        if (node->isNodeHighlighted) {
            std::unique_ptr<TreeNode> graphNode = std::make_unique<TreeNode>();
            graphNode->set(
                std::to_string(node->val), 
                node->position, Size::NODE_RADIUS, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR
            );
            mSceneLayers[Nodes]->attachChild(std::move(graphNode));
        }
        else {
            std::unique_ptr<TreeNode> graphNode = std::make_unique<TreeNode>();
            graphNode->set(std::to_string(node->val), node->position);
            mSceneLayers[Nodes]->attachChild(std::move(graphNode));
        }
    }
    for (int i = 0; i < mConnections.size(); i++) {
        for (int j = i; j < mConnections[0].size(); j++) {
            if (mConnections[i][j]) {
                std::unique_ptr<Edge> edge = std::make_unique<Edge>();
                sf::Vector2f startPos = mNodeList[i]->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS);
                sf::Vector2f endPos = mNodeList[j]->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS);
                if (mEdgeIndex[i][j].second) {
                    edge->set(startPos, endPos, Size::EDGE_THICKNESS, Color::NODE_HIGHLIGHT_COLOR, std::to_string(mDistance[i][j]));
                }
                else {
                    edge->set(startPos, endPos, Size::EDGE_THICKNESS, Color::NODE_EDGE_COLOR, std::to_string(mDistance[i][j]));
                }
                mEdgeIndex[i][j].first = mSceneLayers[Edges]->getChildren().size();
                mSceneLayers[Edges]->attachChild(std::move(edge));
            }
        }
    }
}