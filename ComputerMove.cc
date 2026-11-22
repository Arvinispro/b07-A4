#include "ComputerMove.h"
#include <stdexcept>
#include <vector>

namespace chess {
namespace {

Move randomMove(const std::vector<Move> &moves, std::mt19937 &random) {
    std::uniform_int_distribution<std::size_t> pick(0, moves.size() - 1);
    return moves[pick(random)];
}

std::optional<Point> captureSquare(const Board &board, const Move &move) {
    if (!board.at(move.to).empty()) {
        return move.to;
    }

    // A legal pawn move diagonally to an empty square is en passant.
    if (board.at(move.from).type == PieceType::Pawn && move.from.col != move.to.col) {
        return Point{move.from.row, move.to.col};
    }
    return std::nullopt;
}

int countExposedPieces(const Board &board, Color opponent) {
    bool counted[8][8] = {};
    int total = 0;
    for (const Move &reply : board.legalMoves(opponent)) {
        const auto captured = captureSquare(board, reply);
        if (!captured) {
            continue;
        }

        // Count each piece once even if several replies can capture it.
        int row = captured->row - 1;
        int col = captured->col - 'a';
        if (!counted[row][col]) {
            counted[row][col] = true;
            ++total;
        }
    }
    return total;
}

}

std::optional<Move> chooseComputerMove(const Board &board, Color side, int level,
                                       std::mt19937 &random) {
    if (level < 1 || level > 3) {
        throw std::invalid_argument("Expected computer level 1, 2, or 3");
    }

    const std::vector<Move> moves = board.legalMoves(side);
    if (moves.empty()) {
        return std::nullopt;
    }
    if (level == 1) {
        return randomMove(moves, random);
    }

    const Color opponent = side == Color::White ? Color::Black : Color::White;
    std::vector<Move> choices;
    std::vector<Move> capturesOrChecks;
    int fewestExposed = 0;
    for (const Move &move : moves) {
        Board after = board;
        after.setSideToMove(side);
        if (!after.makeMove(move)) {
            throw std::logic_error("Board rejected a move from legalMoves");
        }

        if (level == 3) {
            int exposed = countExposedPieces(after, opponent);
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
        if (captureSquare(board, move) || after.inCheck(opponent)) {
            capturesOrChecks.push_back(move);
        }
    }
    if (!capturesOrChecks.empty()) {
        return randomMove(capturesOrChecks, random);
    }
    return randomMove(choices, random);
}

}
