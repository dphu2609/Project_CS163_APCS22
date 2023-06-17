#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <GlobalVar.hpp>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable {
private: 
    SceneNode* mParent;
    std::vector<std::unique_ptr<SceneNode>> mChildren;
public:
    typedef std::unique_ptr<SceneNode> Ptr;
    SceneNode();

    void attachChild(std::unique_ptr<SceneNode> child);
    std::vector<std::unique_ptr<SceneNode>>& getChildren();

    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void handleEvent(sf::Event &event);

private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {}
    virtual void updateCurrent() {}
    virtual void handleCurrentEvent(sf::Event &event) {}
};

#endif