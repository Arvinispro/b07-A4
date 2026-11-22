#include "Util.h"
#include <iostream>
#include <vector>
#include <stdexcept>

std::vector<std::vector<char>> game = {
/*8*/ {'r','n','b','q','k','b','n','r'},
/*7*/ {'p','p','p','p','p','p','p','p'},
/*6*/ {' ','_',' ','_',' ','_',' ','_'},
/*5*/ {'_',' ','_',' ','_',' ','_',' '},
/*4*/ {' ','_',' ','_',' ','_',' ','_'},
/*3*/ {'_',' ','_',' ','_',' ','_',' '},
/*2*/ {'P','P','P','P','P','P','P','P'},
/*1*/ {'R','N','B','Q','K','B','N','R'}
/*      a   b   c   d   e   f   g   h    */
};
bool gameReady = false, whiteTurn = true;

Point Point::operator+(const Point &y){
  return Point(row + y.row, col + y.col-'a');
}
Point &Point::operator+=(const Point &y){
  row += y.row;
  col += y.col - 'a';
  return *this;
}

std::istream &operator>>(std::istream &in, Point &move){
  in >> move.col >> move.row; //expect <char><int>, ie. "e3"
  if(in.fail()) throw std::invalid_argument{"Invalid point"};
  // input check
  if(move.col < 'a' || move.col > 'h') throw std::invalid_argument{"Invalid point"};
  if(move.row < 1 || move.row > 8) throw std::invalid_argument{"Invalid point"};

  return in;
}
