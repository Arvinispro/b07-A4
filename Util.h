#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <vector>
extern std::vector<std::vector<char>> game; //an 8*8 chess board, called game b/c of overlapping name with 'board'
extern bool gameReady, whiteTurn;
class Point{
  int row;
  char col;
  Point &operator+(const Point &y); //adding two points
  public:
    int &getRow();
    char &getCol();
  
};
std::istream &operator>>(std::istream &in, Point &move);
#endif