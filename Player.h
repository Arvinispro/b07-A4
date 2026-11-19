#ifndef PLAYER_H
#define PLAYER_H
#include "Subject.h"
#include "Observer.h"
#include "Moves.h"
#include "Util.h"
#include <vector>

class Player: public Subject, public Observer, public Moves{
  Subject *sys; //subject
  Point lastPiece;
  Point lastMove;//these are passed to validater
  public:
    Player(Subject *sys);
    ~Player() override;
    void notify() override; // be called by system to make move
    std::vector<Point> &getState() override; // query the move made by Player
};
#endif