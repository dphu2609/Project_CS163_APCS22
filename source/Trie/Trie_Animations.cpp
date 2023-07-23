#include <State/Trie.hpp>

void Trie::createTree() {
    mSceneLayers[Nodes]->getChildren().clear();
    mSceneLayers[Edges]->getChildren().clear();
    for (auto &node : mNodeList) {
        if (node->isEndOfWord) {
            std::unique_ptr<TreeNode> trieNode = std::make_unique<TreeNode>();
            std::string content = "a";
            content[0] = node->val;
            trieNode->set(
                content, node->position, Size::NODE_RADIUS, 
                Color::NODE_TEXT_COLOR, Color::NODE_COLOR, Color::NODE_OUTLINE_COLOR
            );
            mSceneLayers[Nodes]->attachChild(std::move(trieNode));
        }
        else {
            std::unique_ptr<TreeNode> trieNode = std::make_unique<TreeNode>();
            std::string content = "a";
            content[0] = node->val;
            trieNode->set(content, node->position);
            mSceneLayers[Nodes]->attachChild(std::move(trieNode));
        }
        for (int i = 0; i < node->child.size(); i++) {
            node->edgeIndex.push_back(mSceneLayers[Edges]->getChildren().size());
            std::unique_ptr<Edge> edge = std::make_unique<Edge>();
            edge->set(node->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS), node->child[i]->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS));
            mSceneLayers[Edges]->attachChild(std::move(edge));
        }
    }
}