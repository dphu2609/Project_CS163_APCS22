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
private:
    std::vector<std::vector<bool>> mConnections = {};
    std::vector<std::vector<int>> mDistance = {};
    std::vector<std::vector<std::pair<int, bool>>> mEdgeIndex = {};
private:
    float NODE_DISTANCE_HORIZONTAL = 250.f * Constant::SCALE_X;
    float NODE_DISTANCE_VERTICAL = 250.f * Constant::SCALE_Y;
private:
    std::vector<Node*> mNodeList = {};
    struct TreeState {
        Node* root;
        Node* nodeForOperation;
        std::vector<Node*> nodeList;
        int animationIndex;
    };
    TreeState* createTreeState(int animationIndex) {return nullptr;}
private: //Algorithms
    void restoreTree() {}
    void initGraph();
    Node* copyNodeProperties(Node* node) {return nullptr;}
    Node* copyGraph(Node* root) {return nullptr;}
    void setTreeScale(int treeSize);
    void balanceTree();
    void createRandomTree();
    void returnToPreviousStep() {}
private: //Visualization
    void createTree();
    void insertAnimation() {}
    void deleteAnimation() {}
    void searchAnimation() {}
    bool isProcessingAnimation() {return false;}
    void resetNodeState() {}
    void resetAnimation() {}
private:
    bool mInsertAnimation = false;
    bool mDeleteAnimation = false;
    bool mSearchAnimation = false;
    bool mUpdateAnimation = false;
private:
    std::vector<int> mInputData = {};
    int mInputSize = 6;
    std::queue<int> mInputQueue = {};
    int mAnimationStep = 1;
private:
    std::stack<TreeState*> mTreeForBackward = {};
    std::vector<std::pair<Node*, int>> mTravelPath = {};
    std::vector<Node*> mSplitCheckpoint = {};
    int mSplitCheckpointIndex = 0;
    Node* mOperationNode = nullptr;
    int mTravelIndex = 0;
    std::pair<bool, bool> mTraverseControler = {false, false};
private: //Control box
    bool mIsAnimationPaused = false;
    bool mIsStepByStepMode = false;
    bool mIsReversed = false;
    bool mIsPendingReversed = false;
    bool mIsReplay = false;
};

#endif