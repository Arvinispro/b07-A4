#include "Validate.h"
#include "Util.h"
#include <vector>
Validate::Validate(){
  //construct
}
Validate::~Validate(){
  p1 -> detach(this);
  p2 -> detach(this); // be aware when p1 and p2 are empty

  //destruct
}
void Validate::notify(){ 
  // do stuff when being notified by one of the players
}
void Validate::setPlayers(Subject *p1, Subject *p2){
  this -> p1 = p1;
  this -> p2 = p2;
}

std::vector<Point> &Validate::getState(){
  //return validated piece and move
}
void Validate::validate(){
  if (/*legal*/){
    this->piece = Point{1, 'e'};
    this->move = Point{3, 'e'}; //the validated move
    whiteTurn = !whiteTurn; //switch turn
    notifyObservers(); //run each obs' notify(), which asks for Validate::getstate() to apply moves
  }
  else{
    state = Point{"z0", "z0"};  //represent error msg
    //notify system to tell the same colour to redo the move 
  }
}