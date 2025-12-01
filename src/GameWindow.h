#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"
#include "TextureManager.h"

class GameWindow{
    sf::RenderWindow window;
    unsigned cols,rows;
    unsigned mines;

    Board board;
    TextureManager textures;

    bool debug=false;
    bool paused=false;
    bool gameOver=false;
    bool won=false;

    bool timerRunning=false;
    sf::Clock timerClock;
    int baseSeconds=0;

    std::string playerName;

    sf::Sprite faceSprite;
    sf::Sprite debugSprite;
    sf::Sprite pauseSprite;
    sf::Sprite leaderboardSprite;

    void reset();
    void setFaceTexture();
    void handleClick(int x,int y,bool left,bool right);
    void handleTileClick(unsigned c,unsigned r,bool left,bool right);
    int getElapsedSeconds() const;
    void pauseTimer();
    void resumeTimer();

    void drawTiles();
    void drawUI();
public:
    GameWindow(unsigned c,unsigned r,unsigned m,const std::string& name);
    void run();
};
