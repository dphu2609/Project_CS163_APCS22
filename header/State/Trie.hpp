#ifndef TRIE_HPP
#define TRIE_HPP

#include <State/StateStack.hpp>
#include <SceneGraph/TreeNode.hpp>
#include <SceneGraph/Edge.hpp>
#include <SceneGraph/RectangleButton.hpp>
#include <SceneGraph/InputBox.hpp>
#include <SceneGraph/ImageButton.hpp>

class Trie : public State {
private:
    virtual void draw();
    virtual void update();
    virtual void handleEvent(sf::Event &event);
    virtual void buildScene();
public:
    struct Node {
        char val;
        int depth;
        int nodeIndex;
        std::vector<int> edgeIndex;
        int orderOfNode;
        sf::Vector2f position;
        std::vector<Node*> child;
        Node* parent;
        bool isEndOfWord;
        bool isNodeHighlighted;
    };
public:
    explicit Trie(StateStack& stack, sf::RenderWindow &window);
    ~Trie();
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
    TreeState* createTreeState(int animationIndex) {return nullptr;}
private: //Algorithms
    void createBackupTree();
    void restoreTree();
    void clear(Node *&root);
    void insert(Node* &root, std::vector<Node*> &nodeList, std::string word);
    void add1Node(Node *&root, std::vector<Node*> &nodeList, std::string word, int &currentIndex);
    Node* copyNodeProperties(Node* node);
    Node* copyTrie(Node* root);
    // Node* findNode(Node* root, int index);
    void setTreeScale(int treeSize);
    void moveTree(Node* root, bool isLeft);
    void createRandomTree();
    void getNodeList(Node *root, std::vector<Node*> &nodeList);
    void getTravelPath(std::string word);
    int isLeftMidRight(Node* node);
    void balanceTree();
    void returnToPreviousStep() {}
private: //Visualization
    void createTree();
    void insertAnimation();
    void deleteAnimation() {}
    void searchAnimation() {}
    void traverseAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void moveTreeAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void changeNodeAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0) {}
    void nodeAppearAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0) {}
    bool isProcessingAnimation() {return false;}
    void resetNodeState();
    void resetAnimation();
private:
    bool mInsertAnimation = false;
    bool mDeleteAnimation = false;
    bool mSearchAnimation = false;
    bool mUpdateAnimation = false;
private:
    std::vector<std::string> mInputData = {};
    int mInputSize = 4;
    std::queue<std::string> mInputQueue = {};
    int mAnimationStep = 1;
private:
    std::stack<TreeState*> mTreeForBackward = {};
    std::vector<std::pair<Node*, int>> mTravelPath = {};
    std::vector<bool> mIsEdgeHighlighted = {};
    std::vector<Node*> mSplitCheckpoint = {};
    int mSplitCheckpointIndex = 0;
    Node* mOperationNode = nullptr;
    int mOperationIndex = 0;
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