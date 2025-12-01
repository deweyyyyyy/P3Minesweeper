#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class WelcomeWindow{
    sf::RenderWindow window;
    sf::Font font;
    sf::Text title;
    sf::Text prompt;
    sf::Text nameText;
    std::string name;
    unsigned cols,rows;

    void layout();
    void updateNameText();
public:
    WelcomeWindow(unsigned c,unsigned r);
    bool run(std::string& outName);
};
