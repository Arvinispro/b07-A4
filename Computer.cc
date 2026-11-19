#include "Computer.h"
#include <vector>
Computer::Computer(Subject *sys, int level){ // set validate in the obse
  //construct
//  subject->attach(this);
} 
Computer::~Computer(){
  // subject->detach(this);

  //destruct
}
void Computer::notify(){ // be called by system to make move
  //make move
} 
std::vector<Point> &Computer::getState(){ // query the move made by bot
  // return <lastPiece, lastMove>;
} 