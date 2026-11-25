#include "Moves.h"
#include <cctype>
#include <cstdlib>

bool Moves::onBoard(Point p) {
  return p.row >= 1 && p.row <= 8 && p.col >= 'a' && p.col <= 'h';
}

bool Moves::isPiece(char piece) {
  switch (piece) {
    case 'K': case 'Q': case 'R': case 'B': case 'N': case 'P':
    case 'k': case 'q': case 'r': case 'b': case 'n': case 'p':
      return true;
    default:
      return false;
  }
}

bool Moves::isWhite(char piece) {
  return piece == 'K' || piece == 'Q' || piece == 'R' ||
         piece == 'B' || piece == 'N' || piece == 'P';
}

bool Moves::isPatternLegal(char piece, Point from, Point to, bool capture) {
  if (!isPiece(piece) || !onBoard(from) || !onBoard(to)) return false;

  int dx = to.col - from.col;
  int dy = to.row - from.row;
  int x = std::abs(dx);
  int y = std::abs(dy);
  if (x == 0 && y == 0) return false;

  switch (std::tolower(piece)) {
    case 'p': {
      int forward = isWhite(piece) ? 1 : -1;
      int startRow = isWhite(piece) ? 2 : 7;
      if (capture) return x == 1 && dy == forward;
      return dx == 0 && (dy == forward ||
             (from.row == startRow && dy == 2 * forward));
    }
    case 'n':
      return (x == 1 && y == 2) || (x == 2 && y == 1);
    case 'b':
      return x == y;
    case 'r':
      return dx == 0 || dy == 0;
    case 'q':
      return x == y || dx == 0 || dy == 0;
    case 'k':
      if (x <= 1 && y <= 1) return true;
      return dy == 0 && x == 2 && from.col == 'e' &&
             from.row == (isWhite(piece) ? 1 : 8);
  }
  return false;
}

bool Moves::attacks(char piece, Point from, Point to) {
  if (!isPiece(piece) || !onBoard(from) || !onBoard(to)) return false;

  int dx = to.col - from.col;
  int dy = to.row - from.row;
  if (std::tolower(piece) == 'p') {
    return std::abs(dx) == 1 && dy == (isWhite(piece) ? 1 : -1);
  }
  if (std::tolower(piece) == 'k') {
    return std::abs(dx) <= 1 && std::abs(dy) <= 1 && (dx != 0 || dy != 0);
  }
  return isPatternLegal(piece, from, to, true);
}
