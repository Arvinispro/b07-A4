#ifndef PLAYER_H
#define PLAYER_H
#include "Subject.h"
#include "Observer.h"
#include "Moves.h"
#include "Util.h"
#include "System.h"


class Player: public Subject, public Observer, public Moves{
  System *sys; 
  Point lastPiece;
  Point lastMove;
  public:
    Player();
    void notify() override;
    ~Player() override;
    Point getstate(); // query the move made by Player
};
#endif