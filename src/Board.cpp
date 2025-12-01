#include "Board.h"
#include <random>

Board::Board(unsigned c,unsigned r,unsigned m):cols(c),rows(r),mineCount(m){
    reset(mineCount);
}

bool Board::inBounds(int c,int r) const{
    return c>=0 && r>=0 && c<(int)cols && r<(int)rows;
}

Cell& Board::atRef(unsigned c,unsigned r){
    return cells[r*cols + c];
}

const Cell& Board::at(unsigned c,unsigned r) const{
    return cells[r*cols + c];
}

unsigned Board::getCols() const{return cols;}
unsigned Board::getRows() const{return rows;}
unsigned Board::getMineCount() const{return mineCount;}
unsigned Board::getFlagCount() const{return flagCount;}
int Board::remainingMines() const{return (int)mineCount - (int)flagCount;}

void Board::reset(unsigned m){
    mineCount=m;
    flagCount=0;
    cells.assign(cols*rows,Cell{});
    std::mt19937 rng((unsigned)std::random_device{}());
    std::uniform_int_distribution<int> dist(0,(int)cells.size()-1);
    unsigned placed=0;
    while(placed<mineCount){
        int idx=dist(rng);
        if(!cells[idx].mine){
            cells[idx].mine=true;
            placed++;
        }
    }
    computeAdjacency();
}

void Board::computeAdjacency(){
    for(auto& c:cells) c.adj=0;
    for(unsigned r=0;r<rows;r++){
        for(unsigned c=0;c<cols;c++){
            const Cell& cell=at(c,r);
            if(!cell.mine) continue;
            for(int dr=-1;dr<=1;dr++){
                for(int dc=-1;dc<=1;dc++){
                    if(dr==0 && dc==0) continue;
                    int nc=(int)c+dc,nr=(int)r+dr;
                    if(inBounds(nc,nr)){
                        atRef((unsigned)nc,(unsigned)nr).adj++;
                    }
                }
            }
        }
    }
}

RevealResult Board::reveal(unsigned c,unsigned r){
    Cell& cell=atRef(c,r);
    if(cell.flagged || cell.revealed) return RevealResult::None;
    if(cell.mine){
        cell.revealed=true;
        return RevealResult::HitMine;
    }
    floodReveal(c,r);
    return RevealResult::Revealed;
}

void Board::floodReveal(unsigned c,unsigned r){
    if(!inBounds((int)c,(int)r)) return;
    Cell& cell=atRef(c,r);
    if(cell.revealed || cell.flagged) return;
    cell.revealed=true;
    if(cell.adj!=0) return;
    for(int dr=-1;dr<=1;dr++){
        for(int dc=-1;dc<=1;dc++){
            if(dr==0 && dc==0) continue;
            int nc=(int)c+dc,nr=(int)r+dr;
            if(inBounds(nc,nr)){
                floodReveal((unsigned)nc,(unsigned)nr);
            }
        }
    }
}

void Board::toggleFlag(unsigned c,unsigned r){
    Cell& cell=atRef(c,r);
    if(cell.revealed) return;
    if(cell.flagged){
        cell.flagged=false;
        if(flagCount>0) flagCount--;
    }else{
        cell.flagged=true;
        flagCount++;
    }
}

bool Board::allSafeRevealed() const{
    for(const auto& cell:cells){
        if(!cell.mine && !cell.revealed) return false;
    }
    return true;
}

void Board::flagAllMines(){
    for(auto& cell:cells){
        if(cell.mine && !cell.flagged){
            cell.flagged=true;
            flagCount++;
        }
    }
}
