#ifndef BST_HPP
#define BST_HPP

#include <State/StateStack.hpp>
#include <SceneGraph/TreeNode.hpp>
#include <SceneGraph/Edge.hpp>
#include <SceneGraph/RectangleButton.hpp>

class BST : public State {
private:
    virtual void draw();
    virtual void update();
    virtual void handleEvent(sf::Event &event);
private:
    struct Node {
        int val;
        int height;
        int widthLeft;
        int widthRight;
        sf::Vector2f position;
        int nodeIndex;
        int edgeLeftIndex;
        int edgeRightIndex;
        bool isLeft;
        Node* left;
        Node* right;
    };
private:
    int mNodeCount = 0;
    int mEdgeLeftCount = 0;
    int mEdgeRightCount = 0;
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
        LayerCount
    };
private:
    //give data set for balanced binary search tree
    std::vector<int> sampleData = { 153, 73, 364, 271, 315, 292, 15, 27, 18, 20, 283, 549, 490, 64, 500 };
    Node* mRoot = nullptr;
private:
    virtual void buildScene();
private:
    Node* insert(Node *&root, int height, bool isLeft, const sf::Vector2f &position, int data);
    void moveTree(Node* root, const sf::Vector2f &dis);
private:
    const float NODE_DISTANCE_HORIZONTAL = 80.f * Constant::SCALE_X;
    const float NODE_DISTANCE_VERTICAL = 100.f * Constant::SCALE_Y;
    const float NODE_RADIUS = Size::NODE_RADIUS;
public:
    void testAnimation();
    std::vector<int> getTravelPath(int data);
    std::vector<std::pair<bool, int>> getEdgeTravelPath = {};
    std::vector<int> path = {};
    int indexTravel = 0;
    bool isNodeHighlight1 = false;
    bool isNodeHighlight2 = false;
    bool isEdgeHighlight = false;
    bool repeat = false;
    bool isReset = false;
};

#endif