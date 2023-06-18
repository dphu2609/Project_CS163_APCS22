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
    std::vector<int> sampleData = {44, 40, 76, 2, 75, 84, 19, 73, 9, 20, 53, 22, 68, 64, 69};
    Node* mRoot = nullptr;
private:
    virtual void loadTextures();
    virtual void loadFonts();
    virtual void buildScene();
private:
    Node* insert(Node *&root, int height, bool isLeft, const sf::Vector2f &position, int data);
    void moveTree(Node* root, const sf::Vector2f &dis);
private:
    const float NODE_DISTANCE = 100.f * Constant::scaleX;
    const float NODE_RADIUS = Size::NODE_RADIUS;
};

#endif