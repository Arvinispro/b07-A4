#ifndef COMPUTER_MOVE_H
#define COMPUTER_MOVE_H

#include "Moves.h"
#include <optional>
#include <random>

namespace chess {

// Returns no move if this side has no legal moves.
std::optional<Move> chooseComputerMove(const Board &board, Color side, int level,
                                       std::mt19937 &random);

}

#endif
