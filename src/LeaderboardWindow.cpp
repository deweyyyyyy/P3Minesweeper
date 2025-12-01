#include "LeaderboardWindow.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>

struct Entry{
    int seconds=0;
    std::string timeStr;
    std::string name;
    bool isNew=false;
};

static int parseTime(const std::string& s){
    int m=0,s2=0;
    char c;
    std::stringstream ss(s);
    ss>>m>>c>>s2;
    return m*60+s2;
}

static std::string formatTime(int secs){
    if(secs<0) secs=0;
    int m=secs/60;
    int s=secs%60;
    std::ostringstream out;
    out<<std::setw(2)<<std::setfill('0')<<m<<":"<<std::setw(2)<<std::setfill('0')<<s;
    return out.str();
}

static void loadEntries(std::vector<Entry>& v){
    std::ifstream in("files/leaderboard.txt");
    std::string line;
    while(std::getline(in,line)){
        if(line.empty()) continue;
        std::stringstream ss(line);
        std::string time,name;
        if(std::getline(ss,time,',') && std::getline(ss,name)){
            Entry e;
            e.timeStr=time;
            e.seconds=parseTime(time);
            e.name=name;
            v.push_back(e);
        }
    }
}

static void saveEntries(const std::vector<Entry>& v){
    std::ofstream out("files/leaderboard.txt",std::ios::trunc);
    int count=0;
    for(const auto& e:v){
        if(count==5) break;
        out<<e.timeStr<<","<<e.name<<"\n";
        count++;
    }
}

static void center(sf::Text& t,float x,float y){
    sf::FloatRect rect=t.getLocalBounds();
    sf::Vector2f origin=rect.position + rect.size/2.f;
    t.setOrigin(origin);
    t.setPosition(sf::Vector2f(x,y));
}

void showLeaderboard(unsigned cols,unsigned rows,
                     const std::string& playerName,
                     int elapsedSeconds,
                     bool fromVictory){
    std::vector<Entry> entries;
    loadEntries(entries);

    if(fromVictory){
        Entry me;
        me.seconds=elapsedSeconds;
        me.timeStr=formatTime(elapsedSeconds);
        me.name=playerName;
        entries.push_back(me);
        std::sort(entries.begin(),entries.end(),
                  [](const Entry& a,const Entry& b){return a.seconds<b.seconds;});
        if(entries.size()>5) entries.resize(5);
        for(auto& e:entries){
            if(e.name==playerName && e.seconds==elapsedSeconds){
                e.isNew=true;
                break;
            }
        }
        saveEntries(entries);
    }

    sf::RenderWindow window(sf::VideoMode({cols*16u,rows*16u+50u}),
                            "Leaderboard",
                            sf::Style::Close);
    sf::Font font;
    font.openFromFile("files/font.ttf");

    sf::Text title(font,"LEADERBOARD",20);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Text body(font,"",18);
    body.setFillColor(sf::Color::White);
    body.setStyle(sf::Text::Bold);

    std::ostringstream lines;
    int rank=1;
    for(const auto& e:entries){
        lines<<rank<<".  "<<e.timeStr<<"  "<<e.name;
        if(e.isNew) lines<<" *";
        if(rank<(int)entries.size()) lines<<"\n\n";
        rank++;
    }
    body.setString(lines.str());

    sf::RectangleShape bg(sf::Vector2f(cols*16.f,rows*16.f+50.f));
    bg.setFillColor(sf::Color(0,0,200));

    center(title,cols*16.f/2.f, rows*16.f/2.f - 80.f);
    center(body, cols*16.f/2.f, rows*16.f/2.f + 10.f);

    while(window.isOpen()){
        while(auto ev = window.pollEvent()){
            const sf::Event& e = *ev;
            if(e.is<sf::Event::Closed>()) window.close();
        }
        window.clear();
        window.draw(bg);
        window.draw(title);
        window.draw(body);
        window.display();
    }
}
