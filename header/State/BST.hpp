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
private:
    Node* mRoot = nullptr;
//Visulization
private:
    float NODE_DISTANCE_HORIZONTAL = 80.f * Constant::SCALE_X;
    float NODE_DISTANCE_VERTICAL = 100.f * Constant::SCALE_Y;
    float NODE_RADIUS = Size::NODE_RADIUS;
private:
    std::vector<Node*> mNodeList = {};
    std::vector<Node*> mTravelPath = {};
    std::vector<int> mNodeIndexList = {};
    std::vector<std::pair<bool, int>> mEdgeIndexList = {};
private:
    Node* insert(Node *&root, Node* parent, int data);  
    void moveTree(Node* root, bool isLeft);
public:
    void createTree();
private:
    std::vector<int> sampleData = { 50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85, 5, 15, 23, 33, 43 };
//settings
private:  // create section
};

#endif