#ifndef MOVES_H
#define MOVES_H

#include <array>
#include <optional>
#include <string>
#include <vector>
#include "Util.h"


namespace chess {

enum class Color { White, Black };
enum class PieceType { None, King, Queen, Rook, Bishop, Knight, Pawn };

struct Piece {
    PieceType type = PieceType::None;
    Color color = Color::White;
    bool empty() const { return type == PieceType::None; }
};

struct Move {
    Point from;
    Point to;
    PieceType promotion = PieceType::None; // Q, R, B or N for promotion
};

bool operator==(const Move &a, const Move &b);

class Board {
public:
    Board();
    void clear();
    void standardSetup();
    void put(Point where, Piece p);
    Piece at(Point where) const;
    Color sideToMove() const { return turn_; }
    void setSideToMove(Color c) { turn_ = c; }
    void setCastlingRights(bool whiteKingSide, bool whiteQueenSide,
                           bool blackKingSide, bool blackQueenSide);
    std::string draw() const;

    // Pseudo-legal: correct piece pattern, free path, valid captures, special
    // move preconditions; MAY leave its own king in check.
    std::vector<Move> pseudoLegalMoves(Color side) const;

    // KEY FUNCTION FOR AI: returns ONLY moves that leave own king safe.
    // Does not mutate the board. AI chooses one of these moves.
    std::vector<Move> legalMoves(Color side) const;

    // Confirms move is legal for the current side, then updates the board.
    // Returns false and keeps the board unchanged on failure.
    bool makeMove(const Move &move);

    bool inCheck(Color side) const;
    bool isCheckmate(Color side) const;
    bool isStalemate(Color side) const;

private:
    std::array<Piece, 64> cells_{};
    Color turn_ = Color::White;
    bool whiteCastleKing_ = false, whiteCastleQueen_ = false;
    bool blackCastleKing_ = false, blackCastleQueen_ = false;
    std::optional<Point> enPassantTarget_;

    static bool inside(Point s);
    static int index(Point s);
    void addMovesFor(Point from, std::vector<Move> &out) const;
    bool clearPath(Point from, Point to) const;
    bool attacked(Point point, Color by) const;
    bool hasKing(Color side) const;
    void applyUnchecked(const Move &move);
};

Point parsePoint(const std::string &s);
Move parseMove(const std::string &from, const std::string &to,
               PieceType promotion = PieceType::None);
std::string describeMove(const Move &move);

} // namespace chess
#endif


// #ifndef MOVES_H
// #define MOVES_H
// #include "Util.h"
// class Moves{
//   //each function would validate intrinsically to see if it is a legal move under that piece.
//   //ie. Pawn should capture diagnoallly, or rook should only go straight lines
//   void pawn(Point &cur, const Point &move);
//   void knight(Point &cur, const Point &move);
//   void bishop(Point &cur, const Point &move);
//   void rook(Point &cur, const Point &move);
//   void queen(Point &cur, const Point &move);
//   void king(Point &cur, const Point &move);

//   public:
//     void chooseMove(Point piece, Point move); //allow player and bot to make move
                                             
// };
// #endif

