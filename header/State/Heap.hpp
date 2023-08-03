#ifndef HEAP_HPP
#define HEAP_HPP

#include <State/StateStack.hpp>
#include <SceneGraph/TreeNode.hpp>
#include <SceneGraph/Edge.hpp>
#include <SceneGraph/RectangleButton.hpp>
#include <SceneGraph/InputBox.hpp>
#include <SceneGraph/ImageButton.hpp>
#include <SceneGraph/CodeBlock.hpp>
#include <tinyfiledialogs.h>

class Heap : public State {
private:
    virtual void draw();
    virtual void update();
    virtual void handleEvent(sf::Event &event);
    virtual void buildScene();
public:
    struct Node {
        int val;
        int depth;
        int nodeIndex;
        int order;
        sf::Vector2f position;
        Node* left;
        Node* right;
        Node* parent;
        bool isInsertNode;
        bool isNodeHighlighted;
    };
public:
    explicit Heap(StateStack& stack, sf::RenderWindow &window);
    ~Heap();
    void clear(Node *&root);
private:
    enum SceneLayer {
        LeftEdges,
        RightEdges,
        Nodes,
        Buttons,
        CreateOptions,
        InsertOptions,
        DeleteOptions,
        UpdateOptions,
        SearchOptions,
        ControlBox,
        CodeBox,
        ErrorContainer,
        ErrorConfirmButton,
        ReturnButton,
        LayerCount
    };
    enum ButtonTypes {
        ToggleMaxHeap,
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
        FromFileButton
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
    Node* mRoot = nullptr;
    Node* mRootForBackup = nullptr;
    bool mIsMaxHeap = true;
private:
    float NODE_DISTANCE_HORIZONTAL;
    float NODE_DISTANCE_VERTICAL;
private:
    std::vector<Node*> mNodeList = {};
    std::vector<Node*> mNodeListForBackup = {};
    struct TreeState {
        Node* root;
        Node* nodeForOperation;
        std::vector<Node*> nodeList;
        int animationIndex;
    };
    TreeState* createTreeState(int animationIndex);
private: //Algorithms
    void createBackupTree();
    void restoreTree();
    void insert(Node* &root, std::vector<Node*> &nodeList, int data, bool isNeedCorrectingHeap = true);
    Node* copyNodeProperties(Node* node);
    Node* copyHeap(Node* root);
    Node* findNode(Node* root, int index);
    void setTreeScale(int treeSize);
    void moveTree(Node* root, bool isLeft);
    void createRandomTree();
    std::vector<int> getTravelIndex(int index);
    void balanceTree();
    void returnToPreviousStep();
    void initFromFile() {}
    void annouceError(std::string error) {}
private: //Visualization
    void createTree();
    void insertAnimation();
    void deleteAnimation();
    void searchAnimation() {}
    void moveTreeAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void changeNodeAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void nodeAppearAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    bool isProcessingAnimation();
    void resetNodeState();
    void resetAnimation();
private:
    bool mInsertAnimation = false;
    bool mDeleteAnimation = false;
    bool mSearchAnimation = false;
    bool mUpdateAnimation = false;
private:
    std::vector<int> mInputData = {};
    int mInputSize = 10;
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
    bool mIsAnnouncingError = false;
    bool mIsInitFromFile = false;
};

#endif