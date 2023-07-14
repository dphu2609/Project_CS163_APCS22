#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <SFML/Graphics.hpp>
#include <GlobalVar.hpp>
#include <State/StateStack.hpp>
#include <State/AVL.hpp>
#include <State/Tree234.hpp>

class Program {
private:
    sf::RenderWindow mWindow;
private:
    StateStack mStateStack;
private:
    void registerStates();
public:
    Program();
    void run();
private:
    void processEvents();
private:
    void loadFonts();
    void loadTextures();
};

#endif