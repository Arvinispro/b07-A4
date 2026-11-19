#include "Board.h"

Board::Board(Subject *validate){
  //construct
  // subject->attach(this);
}
Board::~Board(){
  
  // subject->detach(this);

  //destruct
}
void Board::notify(){ //be called by validater to display
  // do things when notified by validater
} 
void Board::display(){}//utilize cout to display the board, u may design some cout operators
void Board::plus(Point piece, Point location){ //add piece on location, display the new board
  //add piece
  display();
}
void Board::minus(Point piece, Point location){ //remove piece on location, display the new board
  //remove
  display();
}
