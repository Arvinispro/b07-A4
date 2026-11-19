#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <vector>
extern std::vector<std::vector<char>> game; //an 8*8 chess board, called game b/c of overlapping name with 'board'
extern bool gameReady, whiteTurn;
struct Point{
  int row;
  char col;
  Point operator+(const Point &y); //adding two points
  Point &operator+=(const Point &y); //adding two points
  
};
std::istream &operator>>(std::istream &in, Point &move);
#endif