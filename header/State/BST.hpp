#ifndef BST_HPP
#define BST_HPP

#include <State/StateStack.hpp>
#include <SceneGraph/TreeNode.hpp>
#include <SceneGraph/Edge.hpp>
#include <SceneGraph/RectangleButton.hpp>
#include <SceneGraph/InputBox.hpp>

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
private:
    Node* mRoot = nullptr;
private:
    float NODE_DISTANCE_HORIZONTAL = 80.f * Constant::SCALE_X;
    float NODE_DISTANCE_VERTICAL = 100.f * Constant::SCALE_Y;
private:
    std::vector<Node*> mNodeList = {};
private: //Algorithms
    void insert(int data);
    Node* insertNonDuplicateNode(Node *&root, Node* parent, int data);  
    void find2NodesForDelete(int data);
    void moveTree(Node* root, bool isLeft);
    void createRandomTree();
    void getTravelPath(Node* root, int data);
    void swapNode(Node* &node1, Node* &node2);
private: //Visualization
    void createTree();

    void insertAnimation();
    void deleteAnimation();
    void traverseAnimation(int animationStepAfterFinish = 0);
    void moveTreeAnimation(int animationStepAfterFinish = 0);
    void nodeAppearAnimation(int animationStepAfterFinish = 0);
    void deleteNodeAnimation(int animationStepAfterFinish = 0);
    void changeNodeAnimation(int animationStepAfterFinish = 0);
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
    bool mIsAnimation = false;
    int mTravelIndex = 0;
    std::pair<bool, bool> mTraverseControler = { false, false };
};

#endif