#ifndef BST_HPP
#define BST_HPP

#include <State/State.hpp>

class BST : public State {
public:
    explicit BST(sf::RenderWindow &window);
private:
    enum Layer {
        Nodes,
        Edges,
        LayerCount
    };
private:
    virtual void loadTextures();
    virtual void loadFonts();
    virtual void buildScene();
}

#endif