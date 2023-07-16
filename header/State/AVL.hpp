#ifndef AVL_HPP
#define AVL_HPP

#include <State/StateStack.hpp>
#include <SceneGraph/TreeNode.hpp>
#include <SceneGraph/Edge.hpp>
#include <SceneGraph/RectangleButton.hpp>
#include <SceneGraph/InputBox.hpp>
#include <SceneGraph/ImageButton.hpp>

class AVL : public State {
private:
    virtual void draw();
    virtual void update();
    virtual void handleEvent(sf::Event &event);
    virtual void buildScene();
public:
    struct Node {
        int val;
        int depth;
        int height;
        int balanceFactor;
        int nodeIndex;
        bool isLeft;
        sf::Vector2f position;
        Node* left;
        Node* right;
        Node* parent;
        int duplicate;
        bool isNodeHighlighted;
        bool isLeftEdgeHighlighted;
        bool isRightEdgeHighlighted;
        bool isInsertNode;
    };
public:
    explicit AVL(StateStack& stack, sf::RenderWindow &window);
    ~AVL();
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
private:
    float NODE_DISTANCE_HORIZONTAL = 80.f * Constant::SCALE_X;
    float NODE_DISTANCE_VERTICAL = 100.f * Constant::SCALE_Y;
private:
    std::vector<Node*> mNodeList = {};
    std::vector<Node*> mNodeListForBackup = {};
    struct TreeState {
        Node* root;
        Node* nodeForRotate;
        Node* nodeForOperation;
        Node* nodeForReplace;
        std::vector<Node*> nodeList;
        int animationIndex;
    };
    TreeState* createTreeState(int animationIndex);
private: //Algorithms
    void insert(Node* &root, std::vector<Node*> &nodeList, int data, bool isNeedRotating = true);
    Node* insertNonDuplicateNode(Node *&root, std::vector<Node*> &nodeList, Node* parent, int data, bool isNeedRotating);  
    Node* rotateLeft(Node *&root);
    Node* rotateRight(Node *&root);
    Node* getRotateNode();
    Node* copyAVL(Node* root);
    Node* findNode(Node* root, int data);
    void updateHeightAndBalanceFactor(Node *&root);
    int getHeight(Node* root);
    void find2NodesForDelete(int data);
    void moveTree(Node* root, bool isLeft);
    void createRandomTree();
    void getTravelPath(Node* root, int data);
    void getBalanceFactorPath(Node* start, Node* end);
    void deleteNode();
    void changeLink();
    void createBackupTree();
    void restoreTree();
    void balanceTree();
    void returnToPreviousStep();
private: //Visualization
    void createTree();

    void insertAnimation();
    void deleteAnimation();
    void searchAnimation();
    void traverseAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void checkBalanceFactorAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void moveTreeAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void nodeAppearAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void changeNodeAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
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
    std::vector<Node*> mTravelPath = {};
    Node* mOperationNode = nullptr;
    Node* mReplaceNode = nullptr;
    Node* mNodeForRotate = nullptr;
    Node* mNodeStartChecking = nullptr;
    int mOperationValue = 0;
    int mOperationIndex = -1;
    int mReplaceValue = 0;
    int mReplaceIndex = -1;
    bool mIsAnimation = false;
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