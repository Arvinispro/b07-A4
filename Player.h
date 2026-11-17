#ifndef PLAYER_H
#define PLAYER_H
#include "Subject.h"
#include "Observer.h"
#include "Moves.h"
#include "Util.h"
#include "System.h"


export class Player: public Subject, public Observer, public Moves{
  System *sys; 
  Point lastPiece;
  Point lastMove;
  public:
  Point getstate(); // query the move made by Player
}
#endif