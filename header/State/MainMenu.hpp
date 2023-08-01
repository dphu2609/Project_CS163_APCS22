#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <State/StateStack.hpp>
#include <SceneGraph/RectangleButton.hpp>
#include <SceneGraph/InputBox.hpp>
#include <SceneGraph/ImageButton.hpp>
#include <GlobalVar.hpp>

class MainMenu : public State {
private:
    virtual void draw();
    virtual void update();
    virtual void handleEvent(sf::Event &event);
    virtual void buildScene();
public:
    explicit MainMenu(StateStack& stack, sf::RenderWindow &window);
private:
    float NODE_RADIUS = 20.f * Constant::SCALE_X;
    float NODE_DISTANCE_HORIZONTAL = 40.f * Constant::SCALE_X;
    float NODE_DISTANCE_VERTICAL = 40.f * Constant::SCALE_Y;
private: 
    enum SceneLayers {
        Background,
        DataStructureButtons,
        AVLNodes,
        AVLEdges,
        GraphNodes,
        GraphEdges,
        HashTableNodes,
        HeapNodes,
        HeadEdges,
        Tree234Nodes,
        Tree234Edges,
        TrieNodes,
        TrieEdges,
        ExitButton,
        LayerCount
    };

    enum DataStructure {
        AVL,
        Tree234,
        Trie,
        HashTable,
        Graph,
        Heap
    };
};

#endif