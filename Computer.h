#ifndef COMPUTER_H
#define COMPUTER_H
#include "Subject.h"
#include "Observer.h"
#include "Moves.h"
#include "Util.h"
#include <vector>

class Computer: public Subject, public Observer, public Moves{
  Subject *sys; //subject
  int level;
  Point lastPiece;
  Point lastMove; //these are passed to validater

  //there might be more functions for levels of computer
  public:
    Computer(Subject *sys, int level); // set validate in the obse
    ~Computer() override;
    void notify() override; // be called by system to make move
    std::vector<Point> &getState(); // query the move made by bot
    
    
    
};
#endif