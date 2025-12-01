#include "GameWindow.h"
#include "LeaderboardWindow.h"
#include <cmath>

GameWindow::GameWindow(unsigned c,unsigned r,unsigned m,const std::string& name)
: cols(c),
  rows(r),
  mines(m),
  board(c,r,m),
  playerName(name),
  faceSprite(textures.get("face_happy.png")),
  debugSprite(textures.get("debug.png")),
  pauseSprite(textures.get("pause.png")),
  leaderboardSprite(textures.get("leaderboard.png"))
{
    window.create(sf::VideoMode({cols*32u,rows*32u+100u}),
                  "Minesweeper",
                  sf::Style::Close);
    window.setFramerateLimit(60);
    reset();
}

void GameWindow::reset(){
    board.reset(mines);
    debug=false;
    paused=false;
    gameOver=false;
    won=false;
    baseSeconds=0;
    timerRunning=true;
    timerClock.restart();

    faceSprite.setTexture(textures.get("face_happy.png"));
    debugSprite.setTexture(textures.get("debug.png"));
    pauseSprite.setTexture(textures.get("pause.png"));
    leaderboardSprite.setTexture(textures.get("leaderboard.png"));

    float y = 32.f*(rows+0.5f);

    faceSprite.setPosition(sf::Vector2f(cols*32.f/2.f-32.f, y));
    debugSprite.setPosition(sf::Vector2f(cols*32.f-304.f, y));
    pauseSprite.setPosition(sf::Vector2f(cols*32.f-240.f, y));
    leaderboardSprite.setPosition(sf::Vector2f(cols*32.f-176.f, y));
}

void GameWindow::setFaceTexture(){
    if(!gameOver)      faceSprite.setTexture(textures.get("face_happy.png"));
    else if(won)       faceSprite.setTexture(textures.get("face_win.png"));
    else               faceSprite.setTexture(textures.get("face_lose.png"));
}

int GameWindow::getElapsedSeconds() const{
    if(!timerRunning) return baseSeconds;
    return baseSeconds + (int)timerClock.getElapsedTime().asSeconds();
}

void GameWindow::pauseTimer(){
    if(!timerRunning) return;
    baseSeconds=getElapsedSeconds();
    timerRunning=false;
}

void GameWindow::resumeTimer(){
    if(timerRunning) return;
    timerClock.restart();
    timerRunning=true;
}

void GameWindow::handleTileClick(unsigned c,unsigned r,bool left,bool right){
    if(gameOver || paused) return;
    if(right){
        board.toggleFlag(c,r);
        return;
    }
    if(left){
        RevealResult res=board.reveal(c,r);
        if(res==RevealResult::HitMine){
            gameOver=true;
            won=false;
            pauseTimer();
            for(unsigned rr=0;rr<rows;rr++){
                for(unsigned cc=0;cc<cols;cc++){
                    Cell& cell=const_cast<Cell&>(board.at(cc,rr));
                    if(cell.mine) cell.revealed=true;
                }
            }
        }else if(board.allSafeRevealed()){
            gameOver=true;
            won=true;
            board.flagAllMines();
            pauseTimer();
        }
        setFaceTexture();
    }
}

void GameWindow::handleClick(int x,int y,bool left,bool right){
    if(left && right) return;

    if(y < (int)rows*32){
        unsigned c=(unsigned)(x/32);
        unsigned r=(unsigned)(y/32);
        if(c<cols && r<rows){
            handleTileClick(c,r,left,right);
        }
        return;
    }

    sf::Vector2f p((float)x,(float)y);
    auto hit=[&](sf::Sprite& s){
        sf::FloatRect b=s.getGlobalBounds();
        return b.contains(p);
    };

    if(left){
        if(hit(faceSprite)){
            reset();
            return;
        }
        if(hit(debugSprite)){
            if(!gameOver && !paused) debug=!debug;
            return;
        }
        if(hit(pauseSprite)){
            if(gameOver) return;
            if(!paused){
                paused=true;
                pauseSprite.setTexture(textures.get("play.png"));
                pauseTimer();
            }else{
                paused=false;
                pauseSprite.setTexture(textures.get("pause.png"));
                resumeTimer();
            }
            return;
        }
        if(hit(leaderboardSprite)){
            bool wasPaused=paused;
            pauseTimer();
            showLeaderboard(cols,rows,playerName,getElapsedSeconds(),
                            gameOver && won);
            if(wasPaused){
                paused=true;
            }else{
                paused=false;
                resumeTimer();
            }
            return;
        }
    }
}

void GameWindow::drawTiles(){
    sf::Sprite tileHidden(textures.get("tile_hidden.png"));
    sf::Sprite tileRevealed(textures.get("tile_revealed.png"));
    sf::Sprite mine(textures.get("mine.png"));
    sf::Sprite flag(textures.get("flag.png"));

    sf::Sprite numbers[8] = {
        sf::Sprite(textures.get("number_1.png")),
        sf::Sprite(textures.get("number_2.png")),
        sf::Sprite(textures.get("number_3.png")),
        sf::Sprite(textures.get("number_4.png")),
        sf::Sprite(textures.get("number_5.png")),
        sf::Sprite(textures.get("number_6.png")),
        sf::Sprite(textures.get("number_7.png")),
        sf::Sprite(textures.get("number_8.png"))
    };

    bool showAllRevealed = paused;

    for(unsigned r=0;r<rows;r++){
        for(unsigned c=0;c<cols;c++){
            const Cell& cell=board.at(c,r);
            float px=c*32.f;
            float py=r*32.f;

            if(showAllRevealed){
                tileRevealed.setPosition(sf::Vector2f(px,py));
                window.draw(tileRevealed);
                continue;
            }

            if(!cell.revealed){
                tileHidden.setPosition(sf::Vector2f(px,py));
                window.draw(tileHidden);
                if(cell.flagged){
                    flag.setPosition(sf::Vector2f(px,py));
                    window.draw(flag);
                }
                if(debug && cell.mine){
                    mine.setPosition(sf::Vector2f(px,py));
                    window.draw(mine);
                }
            }else{
                tileRevealed.setPosition(sf::Vector2f(px,py));
                window.draw(tileRevealed);
                if(cell.mine){
                    mine.setPosition(sf::Vector2f(px,py));
                    window.draw(mine);
                }else if(cell.adj>0){
                    sf::Sprite& num=numbers[cell.adj-1];
                    num.setPosition(sf::Vector2f(px,py));
                    window.draw(num);
                }
            }
        }
    }
}

static void drawNumber(sf::RenderWindow& window,
                       sf::Texture& tex,
                       int value,
                       int digits,
                       float x,float y){
    if(value<0) value=0;
    std::string s = std::to_string(value);
    if((int)s.size() > digits) s = s.substr(s.size()-digits);
    while((int)s.size() < digits) s = "0" + s;

    sf::Sprite spr(tex);
    for(int i=0;i<digits;i++){
        int d = s[i]-'0';
        sf::IntRect rect(
            sf::Vector2i(21*d,0),
            sf::Vector2i(21,32)
        );
        spr.setTextureRect(rect);
        spr.setPosition(sf::Vector2f(x + 21.f*i, y));
        window.draw(spr);
    }
}

void GameWindow::drawUI(){
    sf::Texture& digitsTex = textures.get("digits.png");

    int remaining = board.remainingMines();
    if(remaining < 0) remaining = 0;

    float baseY = 32.f*(rows+0.5f)+16.f;
    drawNumber(window,digitsTex,remaining,3,33.f,baseY);

    int t = getElapsedSeconds();
    int minutes = t/60;
    int seconds = t%60;
    if(minutes>99) minutes=99;

    float minX = cols*32.f - 97.f;
    float secX = cols*32.f - 54.f;
    drawNumber(window,digitsTex,minutes,2,minX,baseY);
    drawNumber(window,digitsTex,seconds,2,secX,baseY);

    window.draw(faceSprite);
    window.draw(debugSprite);
    window.draw(pauseSprite);
    window.draw(leaderboardSprite);
}

void GameWindow::run(){
    while(window.isOpen()){
        while(auto ev = window.pollEvent()){
            const sf::Event& e = *ev;
            if(e.is<sf::Event::Closed>()){
                window.close();
            }
            if(const auto* mb = e.getIf<sf::Event::MouseButtonPressed>()){
                bool left  = mb->button == sf::Mouse::Button::Left;
                bool right = mb->button == sf::Mouse::Button::Right;
                handleClick(mb->position.x, mb->position.y, left, right);
            }
        }

        if(!gameOver && board.allSafeRevealed()){
            gameOver=true;
            won=true;
            board.flagAllMines();
            pauseTimer();
            setFaceTexture();
        }

        window.clear(sf::Color::White);
        drawTiles();
        drawUI();
        window.display();
    }
}
