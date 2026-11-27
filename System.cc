#include "System.h"
#include "Util.h"
#include <vector>
#include <optional>
#include <stdexcept>
#include "Validate.h"

/*System has the following functionalities:
  1. Notify white and black to make moves
  2. Check the game state when a move is applied
  3. Initialize the game by constructing white, black, validate, and board*/

System::System(Validate *val): Subject({}), validate{val}{
  //construct
  if(validate) validate->attach(this);
}
System::~System(){
  if(validate) validate->detach(this);

  //destruct
}
moveNode System::getState(){
  return moveNode{move=this->move, promote=this->promote};
} //if not used, can be deleted

void System::notifyWhite(const std::vector<Point> move){
  if(move.size() > 0){ // might not need to check
    this -> promote = std::nullopt;
    this->move = move;
    // this -> observers[0] ->notify();
  }
  this -> observers[0] ->notify();
} // no promotion
void System::notifyWhite(const std::vector<Point> move, const std::optional<char> promote){
  if(move.size() > 0 && promote){
    this -> promote = promote;
    this->move = move;
    // this -> observers[0] ->notify();
  }
  this -> observers[0] ->notify();
} 
// promotion
void System::notifyBlack(const std::vector<Point> move){
  if(move.size() > 0){
    this -> promote = std::nullopt;
    this->move = move;
    // this -> observers[1] ->notify();
  }
  this -> observers[1] ->notify();
} // no promotion
void System::notifyBlack(const std::vector<Point> move, const std::optional<char> promote){
  if(move.size() > 0 && promote){
    this -> promote = promote;
    this->move = move;
    // this -> observers[1] ->notify();
  }
  this -> observers[1] ->notify();

}
bool System::validateGame(){
  // one white and black king
  int wk=0, bk=0;
  for(std::size_t i=0; i<game.size(); ++i){
    for(auto piece:game[i]){
      if(i==0 || i==7){
        // no pawns at row 1 or 8
        if(piece == 'p' || piece == 'P') return false;
      }
      if(piece=='k'){
        ++bk;
        continue;
      }
      else if(piece=='K'){
        ++wk;
        continue;
      }
    }
  }
  if(wk!=1 || bk!=1) return false;
  
  if(validate->inCheck(true) || validate -> inCheck(false)) return false; //either in check we negate
  return true;
}
auto System::resetGame(){
  observers.clear();
  gameReady = false, whiteTurn = true;
  whiteComp=false, blackComp=false;
  validate -> p1 = nullptr, validate -> p2 = nullptr;
  
  return std::vector<std::vector<char>>{
/*8*/ {'r','n','b','q','k','b','n','r'},
/*7*/ {'p','p','p','p','p','p','p','p'},
/*6*/ {' ','_',' ','_',' ','_',' ','_'},
/*5*/ {'_',' ','_',' ','_',' ','_',' '},
/*4*/ {' ','_',' ','_',' ','_',' ','_'},
/*3*/ {'_',' ','_',' ','_',' ','_',' '},
/*2*/ {'P','P','P','P','P','P','P','P'},
/*1*/ {'R','N','B','Q','K','B','N','R'}
/*      a   b   c   d   e   f   g   h    */
};
}
void System::gameSummary(){
  std::cout << "Summary" << std::endl;
  std::cout << "White:" << whiteScore << "Black:" << blackScore<< std::endl;

}
void System::notify(){// get notified by validator to check game status, if game ends, must set gameReady back to false
  
  if(whiteTurn){// move is done by black
    // black win
    if(validate -> isCheckmate(true)){ 
      ++blackScore;
      std::cout << "Black won!" << std::endl;
      gameSummary();
      game = resetGame();
    }
    if(validate -> isStalemate(true)){ 
      std::cout << "Draw" << std::endl;
      gameSummary();
      game = resetGame();
    }
  }
  else{ // move is done by white
    //white win
    if(validate -> isCheckmate(false)){ 
      ++whiteScore;
      std::cout << "White won!" << std::endl;
      gameSummary();
      game = resetGame();
    }
    if(validate -> isStalemate(false)){  //check black stalemate that caused by white's move
      std::cout << "Draw" << std::endl;
      gameSummary();
      game = resetGame();
    }
  }

} 
//(ie. if someone wins after the latest move, game is over)
void System::gameStart(){ //start the game, make gameReady = true
  if(validateGame()){
    //initialize game, some initializations have been done in main.cc
    
    // should we start the game when the config is stalemate?
    gameReady = true;
  }
  


} 
void System::resign(){ //set gameReady=false
  if(whiteTurn) ++blackScore; //white resigned
  else ++whiteScore;
  //resign
  game = resetGame();
} 
void System::equal(std::string &colour){ // set up the colour
  if(colour == "white") whiteTurn = true;
  if(colour == "black") whiteTurn = false;
}
void System::finishSetup(){
  //Validate the final board in setup mode
  if(!validateGame()){
    throw std::runtime_error{"Current setup is invalid"};
  }
} 
