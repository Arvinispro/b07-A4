#include "Board.h"
#include "Moves.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

namespace {
char emptySquare(Point p) {
  return ((8 - p.row) + (p.col - 'a')) % 2 == 0 ? ' ' : '_';
}
}

Board::Board(Subject *v) : validate{v} {
  if (validate) validate->attach(this);
}

Board::~Board() {
  if (validate) validate->detach(this);
}

void Board::notify() {
  display();
}

void Board::display() {
  for (int row = 8; row >= 1; row--) {
    cout << row << ' ';
    for (char col = 'a'; col <= 'h'; col++) {
      cout << at(Point{row, col});
    }
    cout << '\n';
  }
  cout << "\n  abcdefgh\n";
}

const vector<vector<char>> &Board::position() const {
  return game;
}

char Board::at(Point p) const {
  if (!Moves::onBoard(p)) return '\0';
  return game[8 - p.row][p.col - 'a'];
}

void Board::put(Point p, char piece) {
  if (!Moves::onBoard(p)) return;
  if (piece == ' ' || piece == '_') piece = emptySquare(p);
  game[8 - p.row][p.col - 'a'] = piece;
}

void Board::plus(char piece, Point p) {
  put(p, piece);
  display();
}

void Board::minus(Point p) {
  put(p, emptySquare(p));
  display();
}

void Board::clear() {
  for (int row = 1; row <= 8; row++) {
    for (char col = 'a'; col <= 'h'; col++) {
      Point p{row, col};
      put(p, emptySquare(p));
    }
  }
}

void Board::standardSetup() {
  clear();
  string white = "RNBQKBNR";
  string black = "rnbqkbnr";
  for (int i = 0; i < 8; i++) {
    char col = 'a' + i;
    put({1, col}, white[i]);
    put({2, col}, 'P');
    put({7, col}, 'p');
    put({8, col}, black[i]);
  }
}

void Board::applyUnchecked(Point from, Point to, char promotion) {
  char piece = at(from);
  char target = at(to);
  if (!Moves::isPiece(piece)) return;

  // En passant takes the pawn on the adjacent square.
  if (tolower(piece) == 'p' && from.col != to.col && !Moves::isPiece(target)) {
    Point captured{from.row, to.col};
    put(captured, emptySquare(captured));
  }

  // Castling also moves the rook.
  if (tolower(piece) == 'k' && abs(to.col - from.col) == 2) {
    Point rookFrom{from.row, to.col > from.col ? 'h' : 'a'};
    Point rookTo{from.row, to.col > from.col ? 'f' : 'd'};
    char rook = at(rookFrom);
    put(rookFrom, emptySquare(rookFrom));
    put(rookTo, rook);
  }

  put(from, emptySquare(from));
  if (promotion != 'z') {
    piece = Moves::isWhite(piece) ? toupper(promotion) : tolower(promotion);
  }
  put(to, piece);
}
