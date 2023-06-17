#include <State/BST.hpp>

BST::BST(sf::RenderWindow &window) : State(window) {
    loadTextures();
    loadFonts();
    buildScene();
}

void BST::loadTextures() {}

void BST::loadFonts() {
    mFontsHolder.load(Fonts::FiraSansRegular, "assets/fonts/arial.ttf");
}

void BST::buildScene() {
    for (int i = 0; i < LayerCount; i++) {
        SceneNode::Ptr layer = std::make_unique<SceneNode>();
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
}