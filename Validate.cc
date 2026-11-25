#include "Validate.h"
#include "Board.h"
#include "Moves.h"

#include <cctype>
#include <cstdlib>

using namespace std;

namespace {
using Grid = vector<vector<char>>;

char at(const Grid &board, Point p) {
  if (!Moves::onBoard(p)) return '\0';
  return board[8 - p.row][p.col - 'a'];
}

void put(Grid &board, Point p, char c) {
  board[8 - p.row][p.col - 'a'] = c;
}

char emptyAt(Point p) {
  return ((8 - p.row) + (p.col - 'a')) % 2 == 0 ? ' ' : '_';
}

bool same(Point a, Point b) { return a.row == b.row && a.col == b.col; }

bool opponent(char a, char b) {
  return Moves::isPiece(a) && Moves::isPiece(b) &&
         Moves::isWhite(a) != Moves::isWhite(b);
}

bool clearPath(const Grid &board, Point from, Point to) {
  int dx = (to.col > from.col) - (to.col < from.col);
  int dy = (to.row > from.row) - (to.row < from.row);
  Point current{from.row + dy, static_cast<char>(from.col + dx)};
  while (!same(current, to)) {
    if (!Moves::onBoard(current) || Moves::isPiece(at(board, current))) return false;
    current.row += dy;
    current.col = static_cast<char>(current.col + dx);
  }
  return true;
}

bool needsClearPath(char piece) {
  char type = static_cast<char>(tolower(piece));
  return type == 'b' || type == 'r' || type == 'q';
}

bool attacked(const Grid &board, Point target, bool byWhite) {
  for (int row = 1; row <= 8; ++row) {
    for (char col = 'a'; col <= 'h'; ++col) {
      Point from{row, col};
      char piece = at(board, from);
      if (!Moves::isPiece(piece) || Moves::isWhite(piece) != byWhite) continue;
      if (!Moves::attacks(piece, from, target)) continue;
      if (!needsClearPath(piece) || clearPath(board, from, target)) return true;
    }
  }
  return false;
}

bool locateKing(const Grid &board, bool white, Point &location) {
  char king = white ? 'K' : 'k';
  for (int row = 1; row <= 8; ++row) {
    for (char col = 'a'; col <= 'h'; ++col) {
      if (at(board, {row, col}) == king) {
        location = {row, col};
        return true;
      }
    }
  }
  return false;
}

bool kingInCheck(const Grid &board, bool white) {
  Point king{};
  return locateKing(board, white, king) && attacked(board, king, !white);
}

bool validPromotion(char promotion) {
  char p = static_cast<char>(tolower(promotion));
  return p == 'q' || p == 'r' || p == 'b' || p == 'n';
}

// Check whether the king would be in check after the move.
void simulate(Grid &board, Point from, Point to, char promotion) {
  char piece = at(board, from);
  char dest = at(board, to);
  char type = static_cast<char>(tolower(piece));
  if (type == 'p' && from.col != to.col && !Moves::isPiece(dest)) {
    Point victim{from.row, to.col};
    put(board, victim, emptyAt(victim));
  }
  if (type == 'k' && abs(to.col - from.col) == 2) {
    Point rookFrom{from.row, to.col > from.col ? 'h' : 'a'};
    Point rookTo{from.row, to.col > from.col ? 'f' : 'd'};
    char rook = at(board, rookFrom);
    put(board, rookFrom, emptyAt(rookFrom));
    put(board, rookTo, rook);
  }
  put(board, from, emptyAt(from));
  if (promotion != 'z') {
    piece = Moves::isWhite(piece) ? toupper(promotion) : tolower(promotion);
  }
  put(board, to, piece);
}
}

Validate::Validate() : Subject({}) {}

Validate::~Validate() {
  if (p1) p1->detach(this);
  if (p2 && p2 != p1) p2->detach(this);
}

void Validate::setBoard(Board *b) { board = b; }

void Validate::setPlayers(Subject *white, Subject *black) {
  if (p1) p1->detach(this);
  if (p2 && p2 != p1) p2->detach(this);
  p1 = white;
  p2 = black;
  if (p1) p1->attach(this);
  if (p2 && p2 != p1) p2->attach(this);
}

moveNode Validate::getState() { return state; } //debug

void Validate::notify() {
  validate();
}

void Validate::validate() {
  Subject *player = whiteTurn ? p1 : p2;
  if (!player) return;
  vector<Point> &move = player->getState();
  if (move.size() < 2) return;
  tryMove(move[0], move[1]);
}

void Validate::resetRuleState(bool standardGame) {
  if (standardGame) {
    whiteCastleKing = whiteCastleQueen = true;
    blackCastleKing = blackCastleQueen = true;
  } else if (board) {
    whiteCastleKing = board->at({1, 'e'}) == 'K' && board->at({1, 'h'}) == 'R';
    whiteCastleQueen = board->at({1, 'e'}) == 'K' && board->at({1, 'a'}) == 'R';
    blackCastleKing = board->at({8, 'e'}) == 'k' && board->at({8, 'h'}) == 'r';
    blackCastleQueen = board->at({8, 'e'}) == 'k' && board->at({8, 'a'}) == 'r';
  } else {
    whiteCastleKing = whiteCastleQueen = false;
    blackCastleKing = blackCastleQueen = false;
  }
  enPassantTarget = Point{0, 'a'};
  state.clear();
}

bool Validate::isLegalFor(Point from, Point to, bool white, char promote) const {
  if (!board || !Moves::onBoard(from) || !Moves::onBoard(to)) return false;
  char pieceType = board->at(from);
  char destination = board->at(to);
  if (!Moves::isPiece(pieceType) || Moves::isWhite(pieceType) != white) return false;
  if (Moves::isPiece(destination) && !opponent(pieceType, destination)) return false;
  if (destination == 'K' || destination == 'k') return false;

  char type = tolower(pieceType);
  bool enPassant = false;
  if (type == 'p' && from.col != to.col && !Moves::isPiece(destination) &&
      enPassantTarget.row != 0 && same(to, enPassantTarget)) {
    char victim = board->at({from.row, to.col});
    enPassant = victim == (white ? 'p' : 'P');
  }
  bool capture = Moves::isPiece(destination) || enPassant;
  if (!Moves::isPatternLegal(pieceType, from, to, capture)) return false;

  if (type == 'p') {
    if (from.col != to.col && !capture) return false;
    if (from.col == to.col) {
      if (Moves::isPiece(destination)) return false;
      if (abs(to.row - from.row) == 2) {
        Point between{(from.row + to.row) / 2, from.col};
        if (Moves::isPiece(board->at(between))) return false;
      }
    }
    bool finalRank = to.row == (white ? 8 : 1);
    if (finalRank && !validPromotion(promote)) return false;
    if (!finalRank && promote != 'z') return false;
  } else if (promote != 'z') {
    return false;
  }

  bool castling = type == 'k' && abs(to.col - from.col) == 2;
  if (castling) {
    bool kingSide = to.col == 'g';
    bool queenSide = to.col == 'c';
    if (!kingSide && !queenSide) return false;
    bool right = white ? (kingSide ? whiteCastleKing : whiteCastleQueen)
                       : (kingSide ? blackCastleKing : blackCastleQueen);
    if (!right || kingInCheck(board->position(), white)) return false;
    char rook = white ? 'R' : 'r';
    char rookFile = kingSide ? 'h' : 'a';
    if (board->at({from.row, rookFile}) != rook) return false;
    int step = kingSide ? 1 : -1;
    for (char f = from.col + step; f != rookFile; f += step) {
      if (Moves::isPiece(board->at({from.row, f}))) return false;
    }
    Point between{from.row, static_cast<char>(from.col + step)};
    if (attacked(board->position(), between, !white)) return false;
    if (attacked(board->position(), to, !white)) return false;
  }

  if (needsClearPath(pieceType) && !clearPath(board->position(), from, to)) return false;

  Point king{};
  if (!locateKing(board->position(), white, king)) return false;
  Grid after = board->position();
  simulate(after, from, to, promote);
  return !kingInCheck(after, white);
}

bool Validate::isLegal(Point from, Point to, char promotion) const {
  return isLegalFor(from, to, whiteTurn, promotion);
}

bool Validate::tryMove(Point from, Point to, char promotion) {
  state.clear();
  if (!isLegal(from, to, promotion)) {
    notifyObservers();
    return false;
  }

  char moving = board->at(from);
  char captured = board->at(to);
  bool white = Moves::isWhite(moving);
  char type = tolower(moving);

  if (type == 'k') {
    if (white) whiteCastleKing = whiteCastleQueen = false;
    else blackCastleKing = blackCastleQueen = false;
  }
  if (type == 'r') {
    if (from.row == 1 && from.col == 'a') whiteCastleQueen = false;
    if (from.row == 1 && from.col == 'h') whiteCastleKing = false;
    if (from.row == 8 && from.col == 'a') blackCastleQueen = false;
    if (from.row == 8 && from.col == 'h') blackCastleKing = false;
  }
  if (captured == 'R') {
    if (to.row == 1 && to.col == 'a') whiteCastleQueen = false;
    if (to.row == 1 && to.col == 'h') whiteCastleKing = false;
  }
  if (captured == 'r') {
    if (to.row == 8 && to.col == 'a') blackCastleQueen = false;
    if (to.row == 8 && to.col == 'h') blackCastleKing = false;
  }

  enPassantTarget = Point{0, 'a'};
  if (type == 'p' && abs(to.row - from.row) == 2) {
    enPassantTarget = Point{(from.row + to.row) / 2, from.col};
  }

  board->applyUnchecked(from, to, promotion);
  state = {from, to};
  whiteTurn = !whiteTurn;
  notifyObservers();
  return true;
}

bool Validate::inCheck(bool white) const {
  return board != nullptr && kingInCheck(board->position(), white);
}

bool Validate::hasAnyLegalMove(bool white) const {
  if (!board) return false;
  for (int row = 1; row <= 8; ++row) {
    for (char col = 'a'; col <= 'h'; ++col) {
      Point from{row, col};
      char pieceType = board->at(from);
      if (!Moves::isPiece(pieceType) || Moves::isWhite(pieceType) != white) continue;
      for (int r = 1; r <= 8; ++r) {
        for (char c = 'a'; c <= 'h'; ++c) {
          char promote = (tolower(pieceType) == 'p' &&
                          r == (white ? 8 : 1)) ? 'Q' : 'z';
          if (isLegalFor(from, {r, c}, white, promote)) return true;
        }
      }
    }
  }
  return false;
}

bool Validate::isCheckmate(bool white) const {
  if (!board) return false;
  Point king{};
  if (!locateKing(board->position(), white, king)) return false;
  return inCheck(white) && !hasAnyLegalMove(white);
}

bool Validate::isStalemate(bool white) const {
  if (!board) return false;
  Point king{};
  if (!locateKing(board->position(), white, king)) return false;
  return !inCheck(white) && !hasAnyLegalMove(white);
}

bool Validate::validSetup() const {
  if (!board) return false;
  int whiteKings = 0, blackKings = 0;
  for (int row = 1; row <= 8; ++row) {
    for (char col = 'a'; col <= 'h'; ++col) {
      char pieceType = board->at({row, col});
      if (pieceType == 'K') ++whiteKings;
      if (pieceType == 'k') ++blackKings;
      if ((row == 1 || row == 8) && (pieceType == 'P' || pieceType == 'p'))
        return false;
    }
  }
  return whiteKings == 1 && blackKings == 1 &&
         !inCheck(true) && !inCheck(false);
}

vector<Point> Validate::legalTargets(Point from) const {
  vector<Point> targets;
  if (!board || !Moves::onBoard(from)) return targets;
  char pieceType = board->at(from);
  if (!Moves::isPiece(pieceType)) return targets;
  bool white = Moves::isWhite(pieceType);
  for (int row = 1; row <= 8; ++row) {
    for (char col = 'a'; col <= 'h'; ++col) {
      char promote = (tolower(pieceType) == 'p' &&
                      row == (white ? 8 : 1)) ? 'Q' : 'z';
      if (isLegalFor(from, {row, col}, white, promote)) targets.push_back({row, col});
    }
  }
  return targets;
}

vector<LegalMove> Validate::legalMoves(bool white) const {
  vector<LegalMove> moves;
  if (!board) return moves;

  for (int row = 1; row <= 8; ++row) {
    for (char col = 'a'; col <= 'h'; ++col) {
      Point from{row, col};
      char piece = board->at(from);
      if (!Moves::isPiece(piece) || Moves::isWhite(piece) != white) continue;

      for (int r = 1; r <= 8; ++r) {
        for (char c = 'a'; c <= 'h'; ++c) {
          Point to{r, c};
          if (tolower(piece) == 'p' && r == (white ? 8 : 1)) {
            for (char promotion : {'Q', 'R', 'B', 'N'}) {
              if (isLegalFor(from, to, white, promotion))
                moves.push_back({from, to, promotion});
            }
          } else if (isLegalFor(from, to, white, 'z')) {
            moves.push_back({from, to, 'z'});
          }
        }
      }
    }
  }
  return moves;
}
Board &Validate::getBoard(){
  return *board;
}
