#include "Player.h"
#include <vector>
Player::Player(Subject *sys){
  //construct
  // subject->attach(this);
}
Player::~Player(){
  // subject->detach(this);

  //destruct
}
void Player::notify(){ // be called by system to make move
  //make moves
} 
std::vector<Point> &Player::getState(){ // query the move made by Player
  // return <lastPiece, lastMove>;
} 