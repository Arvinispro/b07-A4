#include "Player.h"

Player::Player(Subject *sys)
    : Subject({}), sys{sys}, state{} {
    if (sys) sys->attach(this);
}

Player::~Player() {
    if (sys) sys->detach(this);
}

void Player::notify() {
    // Called by System when a human player makes a move
    if (!sys) return;

    // Get the move information from System
    moveNode request = sys->getState();

    // A move requires a starting and ending position
    if (request.move.size() != 2) return;

    // Store the move and promotion information
    state = request;

    // Notify Validate that a move has been submitted
    notifyObservers();
}

moveNode Player::getState() {
    // Return the last move made by the player
    return state;
}
