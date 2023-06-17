#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <SFML/Graphics.hpp>
#include <GlobalVar.hpp>

class Program {
private:
    sf::RenderWindow mWindow;
public:
    Program();
    void run();
private:
    void processEvents();
};

#endif