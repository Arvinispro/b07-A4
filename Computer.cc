#include "Computer.h"
#include "ComputerMove.h"
#include "Validate.h" //ComputerMove requires validate and board
#include <random>
#include <vector>
#include <stdexcept>


Computer::Computer(Subject *sys, int level): Subject({}), sys{sys}, level{level}{ // set validate in the obse
  //construct
 sys->attach(this);
} 
Computer::~Computer(){
  sys->detach(this);

  //destruct
}
void Computer::notify(){ // be called by system to make move
  //assuming no invalid level
  std::mt19937 random{std::random_device{}()};
  LegalMove moveComp;
  try{
    moveComp = chess::chooseComputerMove(validate.getBoard(), validate, whiteTurn,  level, random);
    move = moveNode{std::vector<Point>({moveComp.from, moveComp.to}), moveComp.promotion};
  }
  catch(std::invalid_argument &r){
    std::cerr << r.what() << std::endl; //should never happen
    move = moveNode{std::vector<Point>({}), 'z'};
  }
  catch(std::logic_error &r){
    std::cerr << r.what() << std::endl; //no legal move should be caught in system
    move = moveNode{std::vector<Point>({}), 'z'};
  }
  notifyObservers();

} 
moveNode Computer::getState(){ // query the move made by bot
  return move;
} 