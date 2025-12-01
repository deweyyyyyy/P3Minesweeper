#pragma once
#include <vector>

struct Cell{
    bool mine=false;
    bool revealed=false;
    bool flagged=false;
    int adj=0;
};

enum class RevealResult{None,Revealed,HitMine};

class Board{
    unsigned cols,rows;
    unsigned mineCount;
    unsigned flagCount=0;
    std::vector<Cell> cells;

    bool inBounds(int c,int r) const;
    Cell& atRef(unsigned c,unsigned r);
    void computeAdjacency();
    void floodReveal(unsigned c,unsigned r);
public:
    Board(unsigned c,unsigned r,unsigned m);
    void reset(unsigned m);

    RevealResult reveal(unsigned c,unsigned r);
    void toggleFlag(unsigned c,unsigned r);

    const Cell& at(unsigned c,unsigned r) const;
    unsigned getCols() const;
    unsigned getRows() const;
    unsigned getMineCount() const;
    unsigned getFlagCount() const;
    int remainingMines() const;
    bool allSafeRevealed() const;
    void flagAllMines();
};
