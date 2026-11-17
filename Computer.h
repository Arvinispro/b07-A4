#ifndef COMPUTER_H
#define COMPUTER_H
#include "Subject.h"
#include "Observer.h"
#include "Moves.h"
#include "Util.h"
#include "System.h"


export class Computer: public Subject, public Observer, public Moves{
  System *sys;
  int level;
  Point lastPiece;
  Point lastMove;
  //there might be more functions for levels of computer
  public:
  Point getstate(); // query the move made by bot
  Computer();
  ~Computer() override;
}
#endif