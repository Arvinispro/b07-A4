#include "ComputerMove.h"
#include "Board.h"
#include "Moves.h"
#include <cctype>
#include <cstdlib>
#include <vector>

namespace chess {
namespace {

using Position = std::vector<std::vector<char>>;

char pieceAt(const Position &position, Point square) {
    return position[8 - square.row][square.col - 'a'];
}

void put(Position &position, Point square, char piece) {
    position[8 - square.row][square.col - 'a'] = piece;
}

bool isPawn(char piece) {
    return piece == 'P' || piece == 'p';
}

bool isCapture(const Position &position, const LegalMove &move) {
    return Moves::isPiece(pieceAt(position, move.to)) ||
           (isPawn(pieceAt(position, move.from)) && move.from.col != move.to.col);
}

// The caller supplies a legal move or a capture whose king safety is being checked.
void applyMove(Position &position, const LegalMove &move) {
    Point from = move.from;
    Point to = move.to;
    char piece = pieceAt(position, from);

    if (isPawn(piece) && from.col != to.col && !Moves::isPiece(pieceAt(position, to))) {
        put(position, {from.row, to.col}, ' ');  // En passant
    }
    if ((piece == 'K' || piece == 'k') && std::abs(from.col - to.col) == 2) {
        bool kingSide = to.col > from.col;
        Point rookFrom{from.row, kingSide ? 'h' : 'a'};
        Point rookTo{from.row, kingSide ? 'f' : 'd'};
        put(position, rookTo, pieceAt(position, rookFrom));
        put(position, rookFrom, ' ');
    }
    if (move.promotion != 'z') {
        piece = static_cast<char>(Moves::isWhite(piece) ? std::toupper(move.promotion)
                                                       : std::tolower(move.promotion));
    }
    put(position, from, ' ');
    put(position, to, piece);
}

bool attacks(const Position &position, Point from, Point to) {
    char piece = pieceAt(position, from);
    if (!Moves::attacks(piece, from, to)) {
        return false;
    }
    char type = static_cast<char>(std::tolower(piece));
    if (type != 'b' && type != 'r' && type != 'q') {
        return true;
    }

    int rowStep = (to.row > from.row) - (to.row < from.row);
    int colStep = (to.col > from.col) - (to.col < from.col);
    Point square{from.row + rowStep, static_cast<char>(from.col + colStep)};
    while (square.row != to.row || square.col != to.col) {
        if (Moves::isPiece(pieceAt(position, square))) {
            return false;
        }
        square.row += rowStep;
        square.col = static_cast<char>(square.col + colStep);
    }
    return true;
}

bool inCheck(const Position &position, bool white) {
    Point king{};
    for (int row = 1; row <= 8; ++row) {
        for (char col = 'a'; col <= 'h'; ++col) {
            if (pieceAt(position, {row, col}) == (white ? 'K' : 'k')) {
                king = {row, col};
            }
        }
    }
    if (!Moves::onBoard(king)) {
        return false;
    }
    for (int row = 1; row <= 8; ++row) {
        for (char col = 'a'; col <= 'h'; ++col) {
            Point from{row, col};
            char piece = pieceAt(position, from);
            if (Moves::isPiece(piece) && Moves::isWhite(piece) != white &&
                attacks(position, from, king)) {
                return true;
            }
        }
    }
    return false;
}

bool safeCapture(const Position &position, Point from, Point to, bool white) {
    Position after = position;
    char piece = pieceAt(position, from);
    char promotion = isPawn(piece) && to.row == (white ? 8 : 1) ? 'Q' : 'z';
    applyMove(after, {from, to, promotion});
    return !inCheck(after, white);
}

bool canBeCaptured(const Position &position, Point target, bool white,
                   const std::optional<Point> &enPassant) {
    for (int row = 1; row <= 8; ++row) {
        for (char col = 'a'; col <= 'h'; ++col) {
            Point from{row, col};
            char piece = pieceAt(position, from);
            if (!Moves::isPiece(piece) || Moves::isWhite(piece) == white) {
                continue;
            }
            if (attacks(position, from, target) && safeCapture(position, from, target, !white)) {
                return true;
            }
            if (enPassant && isPawn(piece) && target.row == from.row &&
                target.col == enPassant->col && attacks(position, from, *enPassant) &&
                safeCapture(position, from, *enPassant, !white)) {
                return true;
            }
        }
    }
    return false;
}

int countExposedPieces(const Position &position, const LegalMove &move, bool white) {
    std::optional<Point> enPassant;
    if (isPawn(pieceAt(position, move.to)) && std::abs(move.from.row - move.to.row) == 2) {
        enPassant = Point{(move.from.row + move.to.row) / 2, move.to.col};
    }

    int total = 0;
    for (int row = 1; row <= 8; ++row) {
        for (char col = 'a'; col <= 'h'; ++col) {
            Point square{row, col};
            char piece = pieceAt(position, square);
            if (Moves::isPiece(piece) && piece != 'K' && piece != 'k' &&
                Moves::isWhite(piece) == white && canBeCaptured(position, square, white, enPassant)) {
                ++total;
            }
        }
    }
    return total;
}

LegalMove randomMove(const std::vector<LegalMove> &moves, std::mt19937 &random) {
    std::uniform_int_distribution<std::size_t> pick(0, moves.size() - 1);
    return moves[pick(random)];
}

}

std::optional<LegalMove> chooseComputerMove(const Board &board, const Validate &validate,
                                           bool white, int level, std::mt19937 &random) {
    if (level < 1 || level > 3) {
        return std::nullopt;
    }
    const std::vector<LegalMove> moves = validate.legalMoves(white);
    if (moves.empty()) {
        return std::nullopt;
    }
    if (level == 1) {
        return randomMove(moves, random);
    }

    const Position &position = board.position();
    std::vector<LegalMove> choices;
    std::vector<LegalMove> capturesOrChecks;
    int fewestExposed = 0;
    for (const LegalMove &move : moves) {
        Position after = position;
        applyMove(after, move);
        if (level == 3) {
            int exposed = countExposedPieces(after, move, white);
            if (choices.empty() || exposed < fewestExposed) {
                fewestExposed = exposed;
                choices.clear();
                capturesOrChecks.clear();
            }
            if (exposed > fewestExposed) {
                continue;
            }
        }

        choices.push_back(move);
        if (isCapture(position, move) || inCheck(after, !white)) {
            capturesOrChecks.push_back(move);
        }
    }
    if (!capturesOrChecks.empty()) {
        return randomMove(capturesOrChecks, random);
    }
    return randomMove(choices, random);
}

}
