#ifndef PLAYER_H
#define PLAYER_H

#include "Subject.h"
#include "Observer.h"
#include "Util.h"

class Player : public Subject, public Observer {
    Subject *sys;  // System that notifies Player
    moveNode state; // Stores the last move and promotion

public:
    Player(Subject *sys);
    ~Player() override;

    void notify() override;
    moveNode getState() override;
};

#endif
