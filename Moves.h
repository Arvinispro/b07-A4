#ifndef MOVES_H
#define MOVES_H

#include "Util.h"

class Moves {
public:
  static bool onBoard(Point p);
  static bool isPiece(char piece);
  static bool isWhite(char piece);
  static bool isPatternLegal(char piece, Point from, Point to, bool capture);
  static bool attacks(char piece, Point from, Point to);
};

#endif
