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
    std::vector<int> sampleData = { 10, 5, 15, 3, 7, 13, 18, 2, 4, 6, 8, 12, 14, 17, 1, 9, 11, 16, 20, 19, 21, 23, 22, 25, 30, -2, 42};
    Node* mRoot = nullptr;
private:
    virtual void loadTextures();
    virtual void loadFonts();
    virtual void buildScene();
private:
    Node* insert(Node *&root, int height, bool isLeft, const sf::Vector2f &position, int data);
    void moveTree(Node* root, const sf::Vector2f &dis);
};

#endif