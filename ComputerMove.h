#ifndef COMPUTER_MOVE_H
#define COMPUTER_MOVE_H

#include "Validate.h"
#include <optional>
#include <random>

namespace chess {

// Returns no move if this side has no legal moves.
std::optional<LegalMove> chooseComputerMove(const Board &board, const Validate &validate,
                                           bool white, int level, std::mt19937 &random);

}

#endif
