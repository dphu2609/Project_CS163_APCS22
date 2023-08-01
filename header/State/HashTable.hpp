#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <State/StateStack.hpp>
#include <SceneGraph/TreeNode.hpp>
#include <SceneGraph/Edge.hpp>
#include <SceneGraph/RectangleButton.hpp>
#include <SceneGraph/InputBox.hpp>
#include <SceneGraph/ImageButton.hpp>
#include <SceneGraph/CodeBlock.hpp>

class HashTable : public State {
private:
    virtual void draw();
    virtual void update();
    virtual void handleEvent(sf::Event &event);
    virtual void buildScene();
public:
    struct Node {
        int val;
        int isNodeHighlighted;
        sf::Vector2f position;
    };
public:
    explicit HashTable(StateStack& stack, sf::RenderWindow &window);
    ~HashTable();
private:
    enum SceneLayer {
        Nodes,
        Buttons,
        CreateOptions,
        InsertOptions,
        DeleteOptions,
        UpdateOptions,
        SearchOptions,
        ControlBox,
        CodeBox,
        ReturnButton,
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
    std::vector<Node> mNodeList = {};
    std::vector<Node> mNodeListForBackup = {};
    struct TreeState {
        std::vector<Node> nodeList;
        int operationIndex;
        int animationIndex;
    };
    TreeState* createTreeState(int animationIndex);
    void applyTreeState(TreeState* treeState);
private: //Algorithms
    void createRandomTree();
    void setTreeScale(int treeSize);
    void balanceTree();
    void returnToPreviousStep();
private: //Visualization
    void createTree();

    void insertAnimation();
    void deleteAnimation();
    void searchAnimation();
    void changeNodeAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
    void traverseAnimation(bool isAllowPause, float speed = 1.f, int animationStepAfterFinish = 0);
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
    TreeState* mTreeStateForReplay = nullptr;
    std::stack<TreeState*> mTreeForBackward = {};
    std::pair<bool, bool> mTraverseControler = {false, false};
    int mOperationIndex = -1;
private: //Control box
    bool mIsAnimationPaused = false;
    bool mIsStepByStepMode = false;
    bool mIsReversed = false;
    bool mIsPendingReversed = false;
    bool mIsReplay = false;
};

#endif