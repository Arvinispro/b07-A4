#include "chess_movement.h"
#include <iostream>
using namespace chess;
int main() {
    Board board;
    board.standardSetup();
    std::cout << board.draw();
    // This exact call gives Computer all legal choices for the current side.
    const auto options = board.legalMoves(board.sideToMove());
    std::cout << "White has " << options.size() << " legal choices:\n";
    for (const auto &m : options) std::cout << "  " << describeMove(m) << '\n';
    // AI Level 1 could choose a random option from this vector.
    std::cout << "Apply e2 e4: " << (board.makeMove(parseMove("e2", "e4")) ? "OK" : "Illegal") << '\n';
    std::cout << board.draw();
}
