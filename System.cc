#include "System.h"
#include "Util.h"
#include <vector>
/*System has the following functionalities:
  1. Notify white and black to make moves
  2. Check the game state when a move is applied
  3. Initialize the game by constructing white, black, validate, and board*/

System::System(Subject *valitdate){
  //construct
  //subject->attach(this);
}
System::~System(){
  // subject->detach(this);

  //destruct
}
std::vector<Point> &System::getState(){} //if not used, can be deleted
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
void System::notifyWhite(const Point &piece, const Point &position, const char &promote){ //{set move into state, white->notify()}
  //white -> notify(); //white in observers
}
void System::notifyBlack(const Point &piece, const Point &position, const char &promote){//{set move into state, black->notify()}
  //black -> notify(); //black in observers
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
