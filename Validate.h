#ifndef VALIDATE_H
#define VALIDATE_H

#include "Observer.h"
#include "Subject.h"
#include "Util.h"
#include <vector>
#include <memory>


class Board;

struct LegalMove {
  Point from;
  Point to;
  char promotion = 'z';
};

class Validate : public Subject, public Observer {
  Subject *p1 = nullptr;
  Subject *p2 = nullptr;
  Board *board = nullptr;
  moveNode state;


  bool whiteCastleKing = true, whiteCastleQueen = true;
  bool blackCastleKing = true, blackCastleQueen = true;
  Point enPassantTarget{0, 'a'};

  bool isLegalFor(Point from, Point to, bool white, char promotion) const;
  bool hasAnyLegalMove(bool white) const;

public:
  Validate();
  ~Validate() override;
  void notify() override;
  moveNode getState() override; //store two validated moves, one for board for display, 
            //one for system tracking game status (ie. did player win after this move?)
  void validate(); //validate the move recieved from player/bot by seeing if that is legal externally
                  //ie. is the king under check? is the destination occupied by other piece?
  void setPlayers(Subject *p1, Subject *p2); //set subjects of validater
  void setBoard(Board *b);
  Board &getBoard();

  bool tryMove(Point from, Point to, char promotion = 'z');
  bool isLegal(Point from, Point to, char promotion = 'z') const;
  bool inCheck(bool white) const;
  bool isCheckmate(bool white) const;
  bool isStalemate(bool white) const;
  bool validSetup() const;
  std::vector<Point> legalTargets(Point from) const;
  std::vector<LegalMove> legalMoves(bool white) const;
  void resetRuleState(bool standardGame = true);
};

#endif
