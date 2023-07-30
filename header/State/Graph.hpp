#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <State/StateStack.hpp>
#include <SceneGraph/TreeNode.hpp>
#include <SceneGraph/Edge.hpp>
#include <SceneGraph/RectangleButton.hpp>
#include <SceneGraph/InputBox.hpp>
#include <SceneGraph/ImageButton.hpp>

class Graph : public State {
private:
    virtual void draw();
    virtual void update();
    virtual void handleEvent(sf::Event &event);
    virtual void buildScene();
public:
    struct Node {
        int val;
        int nodeIndex;
        sf::Vector2f position;
        std::vector<int> child;
        bool isNodeHighlighted;
        int distance = -1;
        int key = -1;
    };
public:
    explicit Graph(StateStack& stack, sf::RenderWindow &window);
    ~Graph();
    void clear();
private:
    enum SceneLayer {
        Edges,
        Nodes,
        Buttons,
        CreateOptions,
        InsertOptions,
        DeleteOptions,
        UpdateOptions,
        SearchOptions,
        ControlBox,
        MatrixColumnIndex,
        MatrixRowIndex,
        Matrix,
        MatrixOptions,
        LayerCount
    };
    enum ButtonTypes {
        Create,
        Insert,
        Delete,
        Update,
        Search
    };

    enum CreateOptions {
        SizeLabel,
        SizeInputBox,
        RamdomButton,
        FromFileButton,
        FromMatrixButton
    };

    enum InsertOptions {
        InsertInput,
        InsertStart
    };

    enum DeleteOptions {
        DeleteInput,
        DeleteStart
    };

    enum SearchOptions {
        SearchInput,
        SearchStart
    };

    enum ControlBox {
        Play,
        Pause,
        Next,
        Previous,
        Replay
    };

    enum MatrixOptions {
        ConfirmMatrix,
        ClearMatrix
    };
private:
    std::vector<std::vector<bool>> mConnections = {};
    std::vector<std::vector<int>> mDistance = {};
    std::vector<std::vector<std::pair<int, bool>>> mEdgeIndex = {};
private:
    float NODE_DISTANCE_HORIZONTAL = 250.f * Constant::SCALE_X;
    float NODE_DISTANCE_VERTICAL = 250.f * Constant::SCALE_Y;
private:
    std::vector<Node*> mNodeList = {};
    struct GraphState {
        std::vector<std::vector<bool>> connections;
        std::vector<std::vector<int>> distance;
        std::vector<std::vector<std::pair<int, bool>>> edgeIndex;
        std::priority_queue<std::pair<int, int>> priorityQueue;
        std::priority_queue<std::pair<int, std::pair<int, int>>> priorityQueueForPrim;
        int currentIndex;
        int checkIndex;
        std::vector<int> isVisited;
        std::vector<Node*> nodeList;
        int animationIndex;
    };
    GraphState* createGraphState(int animationIndex);
    void applyGraphState(GraphState* graphState);
private: //Algorithms
    void initGraph();
    Node* copyNodeProperties(Node* node) {return nullptr;}
    Node* copyGraph(Node* root) {return nullptr;}
    int getGraphSize(int nodeIndex);
    void setGraphScale(int GraphSize);
    void balanceGraph();
    void createRandomGraph();
    void returnToPreviousStep();
private: //Visualization
    void createGraph();
    void insertAnimation();
    void deleteAnimation();
    void searchAnimation() {}
    bool isProcessingAnimation() {return false;}
    void resetNodeState();
    void resetAnimation();
    void traverseAnimation(bool isAllowPause = true, float speed = 1.f, int animationStepAfterFinish = 0, int startNode = 0, int endNode = 0);
private:
    bool mInsertAnimation = false;
    bool mDeleteAnimation = false;
    bool mSearchAnimation = false;
    bool mUpdateAnimation = false;
private:
    std::vector<int> mInputData = {};
    int mInputSize = 5;
    int mPrevInputSize = 5;
    std::queue<int> mInputQueue = {};
    int mAnimationStep = 1;
private:
    GraphState* mBackupGraph = nullptr;
    std::stack<GraphState*> mGraphForBackward = {};
    std::priority_queue<std::pair<int, int>> mPriorityQueue = {};
    std::priority_queue<std::pair<int, std::pair<int, int>>> mPriorityQueueForPrim = {};
    std::vector<int> mIsVisited = {};
    int mSubGraphSize = 0;
    int mCurrentIndex = 0;
    int mCheckIndex = 0;
    std::pair<bool, bool> mTraverseControler = {false, false};
private: //Control box
    bool mIsAnimationPaused = false;
    bool mIsStepByStepMode = false;
    bool mIsReversed = false;
    bool mIsPendingReversed = false;
    bool mIsReplay = false;
};

#endif