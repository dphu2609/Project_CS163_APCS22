#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <State/StateStack.hpp>
#include <SceneGraph/RectangleButton.hpp>
#include <SceneGraph/InputBox.hpp>
#include <SceneGraph/ImageButton.hpp>
#include <SceneGraph/TreeNode.hpp>
#include <SceneGraph/Edge.hpp>
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
    float NODE_DISTANCE_HORIZONTAL = 80.f * Constant::SCALE_X;
    float NODE_DISTANCE_VERTICAL = 80.f * Constant::SCALE_Y;

    std::vector<bool> mIsHovered = {};
    void handleHoverOfDataStructureButtons(sf::Event &event, int indexOfDSLayer, int indexOfNodeLayer, int indexOfEdgeLayer = -1);
    bool mFirstInit = false;
    void changeTheme();
private: 
    enum SceneLayers {
        Background,
        AppName,
        DataStructureButtons,
        AVLEdges,
        AVLNodes,
        GraphEdges,
        GraphNodes,
        HashTableNodes,
        HeapEdges,
        HeapNodes,
        Tree234Edges,   
        Tree234Nodes,
        TrieEdges,
        TrieNodes,
        ThemeButton,
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