#ifndef CHESS_MOVEMENT_H
#define CHESS_MOVEMENT_H

#include <array>
#include <optional>
#include <string>
#include <vector>

namespace chess {

enum class Color { White, Black };
enum class PieceType { None, King, Queen, Rook, Bishop, Knight, Pawn };

struct Piece {
    PieceType type = PieceType::None;
    Color color = Color::White;
    bool empty() const { return type == PieceType::None; }
};

// Files and ranks are 0-based: a1 = {0,0}, h8 = {7,7}.
struct Square {
    int file = 0;
    int rank = 0;
};

bool operator==(Square a, Square b);
bool operator!=(Square a, Square b);

struct Move {
    Square from;
    Square to;
    PieceType promotion = PieceType::None; // Q, R, B or N for promotion
};

bool operator==(const Move &a, const Move &b);

class Board {
public:
    Board();
    void clear();
    void standardSetup();
    void put(Square where, Piece p);
    Piece at(Square where) const;
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
    std::optional<Square> enPassantTarget_;

    static bool inside(Square s);
    static int index(Square s);
    void addMovesFor(Square from, std::vector<Move> &out) const;
    bool clearPath(Square from, Square to) const;
    bool attacked(Square square, Color by) const;
    bool hasKing(Color side) const;
    void applyUnchecked(const Move &move);
};

Square parseSquare(const std::string &s);
Move parseMove(const std::string &from, const std::string &to,
               PieceType promotion = PieceType::None);
std::string describeMove(const Move &move);

} // namespace chess
#endif
