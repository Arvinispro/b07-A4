#ifndef BOARD_H
#define BOARD_H
#include "Subject.h"
#include "Observer.h"
#include "Validate.h"
class Board: public Observer{
  Subject *validate; //Subject, to getState()
  int whiteScore, blackScore;
  public:
    Board(Subject *validate);
    ~Board() override;
    void notify() override; //be called by validater to display
    void display(); //utilize cout to display the board, u may design some cout operators
    void plus(Point piece, Point location);//add piece on location, display the new board
    void minus(Point piece, Point location);//remove piece on location, display the new board


};

#endif