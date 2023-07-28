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
        NODE_DISTANCE_HORIZONTAL = 250.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 250.f * Constant::SCALE_Y;
    }
    else {
        Size::NODE_RADIUS = 35.f * Constant::SCALE_X;
        Size::NODE_RADIUS_X = 35.f * Constant::SCALE_X;
        Size::NODE_RADIUS_Y = 35.f * Constant::SCALE_Y;
        NODE_DISTANCE_HORIZONTAL = 220.f * Constant::SCALE_X;
        NODE_DISTANCE_VERTICAL = 220.f * Constant::SCALE_Y;
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
    graphState->queueForDjikstra = mQueueForDjikstra;
    graphState->djikstraStep = mDjikstraStep;
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
    mQueueForDjikstra = graphState->queueForDjikstra;
    mDjikstraStep = graphState->djikstraStep;
    for (int i = 0; i < mIsVisited.size(); i++) {
        mIsVisited[i] = graphState->isVisited[i];
    }
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