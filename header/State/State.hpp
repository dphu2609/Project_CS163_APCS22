#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Graphics.hpp>
#include <SceneGraph/SceneNode.hpp>
#include <ResourceHolder.hpp>

class State : private sf::NonCopyable {
public:
    explicit State(sf::RenderWindow &window);
    void update(sf::Time dt);
    void draw();
    void handleEvent(sf::Event &event);
public:
    sf::RenderWindow &mWindow;
    std::array<SceneNode*, 20> mSceneLayers;
    SceneNode mSceneGraph;
    ResourceHolder<sf::Texture, Textures::ID> mTexturesHolder;
    ResourceHolder<sf::Font, Fonts::ID> mFontsHolder;
private:
    virtual void loadTextures() = 0;
    virtual void loadFonts() = 0;
    virtual void buildScence() = 0;
};

#endif