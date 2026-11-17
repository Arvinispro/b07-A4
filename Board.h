#ifndef BOARD_H
#define BOARD_H
export class Board: public Subject, public Observer{
  char **board;
  int whiteScore, blackScore;

  public:
  void display(); //utilize cout to display the board, u may design some cout operators
  char **plus(Point piece, Point location);//add piece on location, returns the new board
  char **minus(Point piece, Point location);//remove piece on location, returns the new board


}

#endif