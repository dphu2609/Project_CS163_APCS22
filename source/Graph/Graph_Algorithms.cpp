#include <State/Graph.hpp>

void Graph::clear() {
    for (int i = 0; i < mNodeList.size(); ++i) {
        delete mNodeList[i];
    }
    mNodeList.clear();
}

void Graph::initGraph() {
    clear();
    mNodeList.resize(mConnections.size(), nullptr);
    int index = 0;
    for (int i = 0; i < mConnections.size(); i++) {
        Node* node = new Node{i, index++, sf::Vector2f(0, 0), {}, false};
        mNodeList[i] = node;
        for (int j = 0; j < mConnections.size(); j++) {
            if (mConnections[i][j] && i != j) {
                node->child.push_back(j);
            }
        }
    }
}

void Graph::createRandomGraph() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 9);
    std::uniform_int_distribution<> childNum(0, 2);
    std::uniform_int_distribution<> childIndex(0, mInputSize - 1);
    mConnections.clear();
    mDistance.clear();
    mEdgeIndex.clear();
    mConnections.resize(mInputSize);
    mDistance.resize(mInputSize);
    mEdgeIndex.resize(mInputSize);
    for (auto &child : mSceneLayers[Matrix]->getChildren()) child->setContent("");
    for (int i = 0; i < mInputSize; i++) {
        mConnections[i].resize(mInputSize, false);
        mDistance[i].resize(mInputSize, -1);
        mEdgeIndex[i].resize(mInputSize, {-1, false});
    } 
    for (int i = 0; i < mInputSize; i++) {
        int childNumber = childNum(gen);
        for (int j = 0; j < childNumber; j++) {
            int child = childIndex(gen);
            if (child == i || mConnections[i][child]) {
                continue;
            }
            mConnections[i][child] = true;
            mConnections[child][i] = true;
            int distance = dis(gen);
            mDistance[i][child] = distance;
            mDistance[child][i] = distance;
            mSceneLayers[Matrix]->getChildren()[i * 10 + child]->setContent(std::to_string(distance));
            mSceneLayers[Matrix]->getChildren()[child * 10 + i]->setContent(std::to_string(distance));
        }
    }
    initGraph();
    setGraphScale(mInputSize);
    balanceGraph();
    createGraph();
}

void Graph::setGraphScale(int GraphSize) {
    if (GraphSize < 7) {
        Size::NODE_RADIUS = 40.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 40.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 40.f * Constant::SCALE_Y;
        NODE_DISTANCE_HORIZONTAL = 350.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 300.f * Constant::SCALE_Y;
    }
    else {
        Size::NODE_RADIUS = 35.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 35.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 35.f * Constant::SCALE_Y;
        NODE_DISTANCE_HORIZONTAL = 320.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 250.f * Constant::SCALE_Y;
    }
}

void Graph::balanceGraph() {
    std::vector<bool> visited(mInputSize, false);
    std::vector<int> depth(mInputSize, 1);
    int count = 0;
    int maxDepth = 0;
    Node* startNode = mNodeList[0];
    while (count < mInputSize) {
        count = 0;
        std::queue<Node*> q;
        q.push(startNode);
        visited[startNode->val] = true;
        while (!q.empty()) {
            Node* temp = q.front();
            q.pop();
            maxDepth = std::max(maxDepth, depth[temp->val]);
            for (auto &node : temp->child) {
                if (!visited[node]) {
                    depth[node] = depth[temp->val] + 1;
                    visited[node] = true;
                    q.push(mNodeList[node]);
                }
            }
        }
        for (int i = 0; i < visited.size(); i++) {
            if (visited[i]) count++;
        }
        for (int i = 0; i < visited.size(); i++) {
            if (!visited[i]) {
                startNode = mNodeList[i];
                break;
            }
        }
    }

    for (auto &node : mNodeList) {
        int numOfDuplicateDepth = 0;
        for (auto childNode : node->child) {
            if (depth[node->val] == depth[childNode] && depth[node->val] != 1) {
                numOfDuplicateDepth++;
                if (numOfDuplicateDepth > 1) {
                    depth[childNode] = depth[node->val] + 1;
                    maxDepth = std::max(maxDepth, depth[childNode]);
                }
            }
        }
    }

    std::vector<std::vector<Node*>> nodeLayer;
    for (int i = 1; i <= maxDepth; i++) {
        std::vector<Node*> layer;
        for (int j = 0; j < depth.size(); j++) {
            if (depth[j] == i) 
                layer.push_back(mNodeList[j]);
        }
        nodeLayer.push_back(layer);
    }
    sf::Vector2f startPos = sf::Vector2f(Constant::WINDOW_WIDTH/2 - Size::NODE_RADIUS, 150 * Constant::SCALE_Y);
    for (int i = 0; i < nodeLayer.size(); i++) {
        nodeLayer[i][0]->position = startPos + sf::Vector2f(- ((int)nodeLayer[i].size() - 1) * NODE_DISTANCE_HORIZONTAL / 2, i * NODE_DISTANCE_VERTICAL);
        for (int j = 1; j < nodeLayer[i].size(); j++) {
            nodeLayer[i][j]->position = nodeLayer[i][0]->position + sf::Vector2f(j * NODE_DISTANCE_HORIZONTAL, 0);
        }
    }
}

Graph::GraphState* Graph::createGraphState(int animationIndex) {
    GraphState* graphState = new GraphState;
    graphState->nodeList.resize(mNodeList.size());
    graphState->connections.resize(mConnections.size());
    graphState->distance.resize(mDistance.size());
    graphState->edgeIndex.resize(mEdgeIndex.size());
    graphState->isVisited.resize(mIsVisited.size());
    for (int i = 0; i < mConnections.size(); i++) {
        graphState->connections[i].resize(mConnections[i].size());
        graphState->distance[i].resize(mDistance[i].size());
        graphState->edgeIndex[i].resize(mEdgeIndex[i].size());
        for (int j = 0; j < mConnections[i].size(); j++) {
            graphState->connections[i][j] = mConnections[i][j];
            graphState->distance[i][j] = mDistance[i][j];
            graphState->edgeIndex[i][j] = mEdgeIndex[i][j];
        }
    }
    graphState->priorityQueue = mPriorityQueue;
    graphState->priorityQueueForPrim = mPriorityQueueForPrim;
    graphState->currentIndex = mCurrentIndex;
    graphState->checkIndex = mCheckIndex;
    for (int i = 0; i < mIsVisited.size(); i++) {
        graphState->isVisited[i] = mIsVisited[i];
    }
    for (int i = 0; i < mNodeList.size(); i++) {
        graphState->nodeList[i] = new Node;
        graphState->nodeList[i]->val = mNodeList[i]->val;
        graphState->nodeList[i]->nodeIndex = mNodeList[i]->nodeIndex;
        graphState->nodeList[i]->position = mNodeList[i]->position;
        graphState->nodeList[i]->child = mNodeList[i]->child;
        graphState->nodeList[i]->isNodeHighlighted = mNodeList[i]->isNodeHighlighted;
        graphState->nodeList[i]->distance = mNodeList[i]->distance;
        graphState->nodeList[i]->key = mNodeList[i]->key;
    }
    graphState->animationIndex = animationIndex;
    return graphState;
}

void Graph::applyGraphState(GraphState * graphState) {
    for (int i = 0; i < mConnections.size(); i++) {
        for (int j = 0; j < mConnections[i].size(); j++) {
            mConnections[i][j] = graphState->connections[i][j];
            mDistance[i][j] = graphState->distance[i][j];
            mEdgeIndex[i][j] = graphState->edgeIndex[i][j];
        }
    }
    mPriorityQueue = graphState->priorityQueue;
    mPriorityQueueForPrim = graphState->priorityQueueForPrim;
    for (int i = 0; i < mIsVisited.size(); i++) {
        mIsVisited[i] = graphState->isVisited[i];
    }
    mCurrentIndex = graphState->currentIndex;
    mCheckIndex = graphState->checkIndex;
    clear();
    for (int i = 0; i < graphState->nodeList.size(); i++) {
        mNodeList.push_back(graphState->nodeList[i]);
    }
    mAnimationStep = graphState->animationIndex;
}

void Graph::returnToPreviousStep() {
    if (mGraphForBackward.empty()) return;
    applyGraphState(mGraphForBackward.top());
    mGraphForBackward.pop();
    createGraph();
}

int Graph::getGraphSize(int nodeIndex) {
    int count = 0;
    std::vector<bool> visited(mInputSize, false);
    std::queue<int> q;
    q.push(nodeIndex);
    visited[nodeIndex] = true;
    while (!q.empty()) {
        int temp = q.front();
        q.pop();
        count++;
        for (auto &node : mNodeList[temp]->child) {
            if (!visited[node]) {
                visited[node] = true;
                q.push(node);
            }
        }
    }
    return count;
}

void Graph::updateGraphByMouse(sf::Event &event) {
    int index = 0;
    for (auto &child : mSceneLayers[Nodes]->getChildren()) {
        if (child->isLeftMouseHeld(mWindow, event)) {
            mNodeList[index]->position = sf::Vector2f(sf::Mouse::getPosition(mWindow)) - sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS);
            break;
        }
        index++;
    }
    for (int i = 0; i < mNodeList.size(); i++) {
        mSceneLayers[Nodes]->getChildren()[i]->setPosition(mNodeList[i]->position);
    }

    for (int i = 0; i < mEdgeIndex.size(); i++) {
        for (int j = i; j < mEdgeIndex[i].size(); j++) {
            if (mConnections[i][j]) {
                sf::Vector2f pos1 = mNodeList[i]->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS);
                sf::Vector2f pos2 = mNodeList[j]->position + sf::Vector2f(Size::NODE_RADIUS, Size::NODE_RADIUS);
                mSceneLayers[Edges]->getChildren()[mEdgeIndex[i][j].first]->setPositionByPoints(pos1, pos2);
            }
        }
    }
}