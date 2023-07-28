#include <State/Graph.hpp>

void Graph::createGraph() {
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
        if (node->distance == INT_MAX) {
            mSceneLayers[Nodes]->getChildren()[node->nodeIndex]->setLabel("INF");
        }
        else if (node->distance != -1) {
            mSceneLayers[Nodes]->getChildren()[node->nodeIndex]->setLabel(std::to_string(node->distance));
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
                mEdgeIndex[j][i].first = mSceneLayers[Edges]->getChildren().size();
                mSceneLayers[Edges]->attachChild(std::move(edge));
            }
        }
    }
}

void Graph::insertAnimation() {
    switch(mAnimationStep) {
        case 1: {
            resetAnimation();
            createGraph();
            mBackupGraph = createGraphState(1);
            mAnimationStep = 2;
            break;
        }

        case 2: {
            if (!mIsReversed) mGraphForBackward.push(createGraphState(2));
            for (auto &node : mNodeList) node->distance = INT_MAX;
            mNodeList[mInputQueue.front()]->distance = 0;
            for (auto &node : mNodeList) {
                if (node->distance == 0) {
                    mSceneLayers[Nodes]->getChildren()[node->nodeIndex]->setLabel("0");
                }
                else {
                    mSceneLayers[Nodes]->getChildren()[node->nodeIndex]->setLabel("INF");
                }
            }
            mAnimationStep = 3;
            break;
        }

        case 3: {
            if (!mIsReversed) mGraphForBackward.push(createGraphState(3));
            mQueueForDjikstra.push({0, mInputQueue.front()});
            mDjikstraStep.push({0, std::make_pair(mInputQueue.front(), mInputQueue.front())});
            mAnimationStep = 4;
            break;
        }

        case 4: {
            if (!mIsReversed) mGraphForBackward.push(createGraphState(4));
            if (mQueueForDjikstra.empty()) {
                mAnimationStep = 6;
                break;
            }
            int temp = mQueueForDjikstra.top().second;
            mDjikstraStepPair = mDjikstraStep.top().second;
            mQueueForDjikstra.pop();
            mDjikstraStep.pop();
            if (mIsVisited[temp]) {
                break;
            }
            mIsVisited[temp] = true;
            mSceneLayers[Nodes]->getChildren()[temp]->setLabel(std::to_string(mNodeList[temp]->distance));
            for (auto &node : mNodeList[temp]->child) {
                if (mNodeList[temp]->distance + mDistance[temp][node] < mNodeList[node]->distance) {
                    mNodeList[node]->distance = mNodeList[temp]->distance + mDistance[temp][node];
                    mQueueForDjikstra.push({-mNodeList[node]->distance, node});
                    mDjikstraStep.push({-mNodeList[node]->distance, std::make_pair(temp, node)});
                }
            }
            mAnimationStep = 5;
            break;
        }

        case 5: {
            traverseAnimation(true, 2, 4, mDjikstraStepPair.first, mDjikstraStepPair.second);
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

void Graph::traverseAnimation(bool isAllowPause, float speed, int animationStepAfterFinish, int startNode, int endNode) {
    if (!isAllowPause) mIsAnimationPaused = false;
    if (!mTraverseControler.first) {
        if (startNode == endNode) mTraverseControler.first = true;
        else {
            mSceneLayers[Edges]->getChildren()[mEdgeIndex[startNode][endNode].first]->change1Color(
                Color::NODE_HIGHLIGHT_COLOR, speed
            );
        }
    }
    else if (!mTraverseControler.second) {
        mSceneLayers[Nodes]->getChildren()[endNode]->change3Color(
            Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
    }
    else  {
        mSceneLayers[Nodes]->getChildren()[endNode]->change3Color(
            Color::NODE_HIGHLIGHT_TEXT_COLOR, Color::NODE_HIGHLIGHT_COLOR, Color::NODE_HIGHLIGHT_OUTLINE_COLOR, speed
        );
    }

    if (!mTraverseControler.first) {
        if (mSceneLayers[Edges]->getChildren()[mEdgeIndex[startNode][endNode].first]->isChange1ColorFinished()) {
            mTraverseControler.first = true;
            mEdgeIndex[startNode][endNode].second = true;
            mSceneLayers[Edges]->getChildren()[mEdgeIndex[startNode][endNode].first]->resetAnimationVar();
        }
    }
    else if (!mTraverseControler.second && !mIsAnimationPaused) {
        if (mSceneLayers[Nodes]->getChildren()[endNode]->isChange3ColorFinished()) {
            mTraverseControler.second = true;
            mNodeList[endNode]->isNodeHighlighted = true;
            mSceneLayers[Nodes]->getChildren()[endNode]->resetAnimationVar();
        }
    }
    else if (!mIsAnimationPaused) {
        if (mSceneLayers[Nodes]->getChildren()[endNode]->isChange3ColorFinished()) {
            mTraverseControler.first = false;
            mTraverseControler.second = false;
            resetNodeState();
            mAnimationStep = animationStepAfterFinish;
        }
    }
}

void Graph::resetNodeState() {
    for (auto &child : mSceneLayers[Nodes]->getChildren()) child->resetAnimationVar();
    for (auto &child : mSceneLayers[Edges]->getChildren()) child->resetAnimationVar();
}

void Graph::resetAnimation() {
    for (auto &node : mNodeList) {
        node->isNodeHighlighted = false;
    }
    for (auto &row : mEdgeIndex) {
        for (auto &col : row) {
            col.second = false;
        }
    }
    mIsVisited.clear();
    mIsVisited.resize(11, false);
    if (mBackupGraph) {
        delete mBackupGraph;
        mBackupGraph = nullptr;
    }
    resetNodeState();
}