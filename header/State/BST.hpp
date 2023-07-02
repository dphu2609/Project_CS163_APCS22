#ifndef BST_HPP
#define BST_HPP

#include <State/StateStack.hpp>
#include <SceneGraph/TreeNode.hpp>
#include <SceneGraph/Edge.hpp>
#include <SceneGraph/RectangleButton.hpp>
#include <SceneGraph/InputBox.hpp>
#include <SceneGraph/ImageButton.hpp>

class BST : public State {
private:
    virtual void draw();
    virtual void update();
    virtual void handleEvent(sf::Event &event);
    virtual void buildScene();
public:
    struct Node {
        int val;
        int height;
        int nodeIndex;
        bool isLeft;
        sf::Vector2f position;
        Node* left;
        Node* right;
        Node* parent;
        int duplicate;
    };
public:
    explicit BST(StateStack& stack, sf::RenderWindow &window);
    ~BST();
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
private: //Algorithms
    void insert(Node* &root, std::vector<Node*> &nodeList, int data);
    Node* insertNonDuplicateNode(Node *&root, std::vector<Node*> &nodeList, Node* parent, int data);  
    void find2NodesForDelete(int data);
    void moveTree(Node* root, bool isLeft);
    void createRandomTree();
    void getTravelPath(Node* root, int data);
    void swapNode(Node* &node1, Node* &node2);
    void deleteNode();
    void deleteNodeReversed();
    void changeLink();
    void changeLinkReversed();
    void reduceHeight(Node* root);
    void createBackupTree();
    void restoreTree();
    void balanceTree();
private: //Visualization
    void createTree();

    void insertAnimation();
    void insertAnimationReversed();
    void deleteAnimation();
    void deleteAnimationReversed();
    void traverseAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void moveTreeAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void nodeAppearAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void deleteNodeAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void deleteNodeAnimationReversed(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
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
    std::vector<Node*> mTravelPath = {};
    std::vector<bool> mIsLeftPath = {};
    Node* mOperationNode = nullptr;
    Node* mReplaceNode = nullptr;
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