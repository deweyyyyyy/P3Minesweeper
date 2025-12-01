#include "WelcomeWindow.h"
#include <cctype>

static void centerText(sf::Text& text,float x,float y){
    sf::FloatRect rect=text.getLocalBounds();
    sf::Vector2f origin = rect.position + rect.size/2.f;
    text.setOrigin(origin);
    text.setPosition(sf::Vector2f(x,y));
}

WelcomeWindow::WelcomeWindow(unsigned c,unsigned r)
: cols(c),rows(r),
  title(font,"WELCOME TO MINESWEEPER!",24),
  prompt(font,"Enter your name:",20),
  nameText(font,"",18)
{
    window.create(sf::VideoMode({cols*32u,rows*32u+100u}),
                  "Welcome",
                  sf::Style::Close);

    font.openFromFile("files/font.ttf");

    title.setFont(font);
    prompt.setFont(font);
    nameText.setFont(font);

    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);

    prompt.setFillColor(sf::Color::White);
    prompt.setStyle(sf::Text::Bold);

    nameText.setFillColor(sf::Color::Yellow);
    nameText.setStyle(sf::Text::Bold);

    layout();
    updateNameText();
}

void WelcomeWindow::layout(){
    float w=cols*32.f;
    float h=rows*32.f+100.f;
    centerText(title,    w/2.f,h/2.f-150.f);
    centerText(prompt,   w/2.f,h/2.f- 75.f);
    centerText(nameText, w/2.f,h/2.f- 45.f);
}

void WelcomeWindow::updateNameText(){
    nameText.setString(name + "|");
    layout();
}

bool WelcomeWindow::run(std::string& outName){
    while(window.isOpen()){
        while(auto ev = window.pollEvent()){
            const sf::Event& e = *ev;

            if(e.is<sf::Event::Closed>()){
                window.close();
                return false;
            }
            if(const auto* te = e.getIf<sf::Event::TextEntered>()){
                char32_t u = te->unicode;
                if(u==8){
                    if(!name.empty()) name.pop_back();
                }else if(u==U'\r' || u==U'\n'){
                    if(!name.empty()){
                        outName=name;
                        window.close();
                        return true;
                    }
                }else if(name.size()<10 && std::isalpha((unsigned char)u)){
                    char c=(char)u;
                    if(name.empty()) c=(char)std::toupper((unsigned char)c);
                    else c=(char)std::tolower((unsigned char)c);
                    name.push_back(c);
                }
                updateNameText();
            }
        }

        window.clear(sf::Color(0,0,200));
        window.draw(title);
        window.draw(prompt);
        window.draw(nameText);
        window.display();
    }
    return false;
}
