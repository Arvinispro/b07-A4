#ifndef MOVES_H
#define MOVES_H
#include "Util.h"
class Moves{
  //each function would validate intrinsically to see if it is a legal move under that piece.
  //ie. Pawn should capture diagnoallly, or rook should only go straight lines
  void pawn(Point &cur, const Point &move);
  void knight(Point &cur, const Point &move);
  void bishop(Point &cur, const Point &move);
  void rook(Point &cur, const Point &move);
  void queen(Point &cur, const Point &move);
  void king(Point &cur, const Point &move);

  public:
    void chooseMove(Point piece, Point move); //allow player and bot to make move
                                             
};
#endif