#include "System.h"
#include "Util.h"
#include <vector>
#include <optional>
/*System has the following functionalities:
  1. Notify white and black to make moves
  2. Check the game state when a move is applied
  3. Initialize the game by constructing white, black, validate, and board*/

System::System(Subject *valitdate): validate{validate}{
  //construct
  if(validate) valitdate->attach(this);
}
System::~System(){
  if(validate) validate->detach(this);

  //destruct
}
moveNode &System::getState(){
  if
} //if not used, can be deleted
void System::notify(){// get notified by validator to check game status, if game ends, must set gameReady back to false
  if(/*checkmate*/){
    if(whiteTurn){// move is done by black
      // black win
    }
    else{
      //white win
    }
    gameReady = false;
  }
  else if('draw'){
      //draw and end game
      gameReady = false;
  }
  else{
    //keep going
  }
} 
                      //(ie. if someone wins after the latest move, game is over)
void System::notifyWhite(const std::vector<Point> move){
  if(move.size() > 0){
    this -> promote = std::nullopt;
    this->move = move;
    this -> observers[0] ->notify();
  }
} // no promotion
void System::notifyWhite(const std::vector<Point> move, const char promote){
  if(move.size() > 0 && promote){
    this -> promote = promote;
    this->move = move;
    this -> observers[0] ->notify();
  }
} // promotion
void System::notifyBlack(const std::vector<Point> move){
  if(move.size() > 0){
    this -> promote = std::nullopt;
    this->move = move;
    this -> observers[1] ->notify();
  }
} // no promotion
void System::notifyBlack(const std::vector<Point> move, const char promote){
  if(move.size() > 0 && promote){
    this -> promote = promote;
    this->move = move;
    this -> observers[1] ->notify();
  }
}
void System::gameStart(){ //start the game, make gameReady = true, notify board::display()
  
  //initialize game, some initializations have been done in main.cc
  gameReady = true;
  //display
} 
void System::resign(){ //set gameReady=false
  //resign
  gameReady = false;
} 
void System::equal(std::string &colour){ // set up the colour
  if(colour == "white") whiteTurn = true;
  if(colour == "black") whiteTurn = false;
}
void System::finishSetup(){
  //Validate the final board in setup mode

} 
