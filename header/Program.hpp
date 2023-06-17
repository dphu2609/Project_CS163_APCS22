#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <SFML/Graphics.hpp>
#include <GlobalVar.hpp>
#include <State/BST.hpp>

class Program {
private:
    sf::RenderWindow mWindow;
    BST mBST;
public:
    Program();
    void run();
private:
    void processEvents();
};

#endif