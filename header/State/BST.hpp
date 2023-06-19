#ifndef BST_HPP
#define BST_HPP

#include <State/State.hpp>
#include <SceneGraph/TreeNode.hpp>
#include <SceneGraph/Edge.hpp>

class BST : public State {
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
    explicit BST(sf::RenderWindow &window);
    ~BST();
    void clear(Node *&root);
private:
    enum Layer {
        LeftEdges,
        RightEdges,
        Nodes,
        LayerCount
    };
private:

    //give data set for balanced binary search tree
    std::vector<int> sampleData = { 50, 30, 20, 40, 70, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85, 5, 15, 23, 28, 33 };
    Node* mRoot = nullptr;
private:
    virtual void loadTextures();
    virtual void loadFonts();
    virtual void buildScene();
private:
    Node* insert(Node *&root, int height, bool isLeft, const sf::Vector2f &position, int data);
    void moveTree(Node* root, const sf::Vector2f &dis);
private:
    const float NODE_DISTANCE_HORIZONTAL = 60.f * Constant::SCALE_X;
    const float NODE_DISTANCE_VERTICAL = 80.f * Constant::SCALE_Y;
    const float NODE_RADIUS = Size::NODE_RADIUS;
public:
    void testAnimation();
    bool repeat = false;
};

#endif