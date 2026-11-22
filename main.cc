#include "Player.h"
#include "Computer.h"
#include "Board.h"
#include "Util.h"
#include "Subject.h"
#include "System.h"
#include "Validate.h"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;
vector<Point> humanMove(string &dummy){
  // validate inputs
  Point piece{'z', 0}, position{'z', 0};
  while(true){
    try{
      if(cin.eof()) return vector<Point>{};
      cin >> piece;
      cin >> position;
      break;
    }
    catch(invalid_argument &){
      cin.clear();
      getline(cin, dummy);
      cerr << "Input the piece and position again" << endl;
    }
  }
  return vector<Point>{piece, position};
}

char promotion(string &dummy){
  char promote;
  string allowed = "QRNB";
  while(true){
    try{
      cin >> promote;
      if(cin.fail()) throw invalid_argument{"Character only"};
      if(!allowed.find(promote)) throw invalid_argument{"Promotion not allowed"};
      break;
    }
    catch(invalid_argument &){
      cin.clear();
      getline(cin, dummy);
      cerr << "Input promotion again" << endl;
    }
  }
  return promote;
}
int main(){
  string command, white, black, level, dummy;
  char promote;
  vector<Point> move;
  Validate validate;
  System sys{&validate};
  Board board{&validate};
  Subject *whiteP=nullptr, *blackP=nullptr;
  bool whiteComp=false, blackComp=false;
  
  while (true) {
    cin >> command;

    // Terminate the program if enter 'quit' or reach EOF.
    if ( cin.fail() || command == "quit" ) break;

    if(command == "game"){
      while(!gameReady){
        // the user will keep input their choice for players until they are valid
        cin >> white;
        if(cin.eof()) break;

        if(white == "human"){
          whiteP = new Player{&sys};
        }
        else if( white == "computer"){
          whiteComp = true;
          cin >> level;
          if(cin.eof()) break;
          if(level == "[1]"){
            whiteP = new Computer{&sys, 1};
          }
          else if(level == "[2]"){
            whiteP = new Computer{&sys, 2};
          }
          else if(level == "[3]"){
            whiteP = new Computer{&sys, 3};
          }
          else{
            cerr << "please input 'computer [level]', level can be 1, 2, or 3" << endl;
            whiteComp = false;
            getline(cin, dummy);
            break;
          }
        }
        else{
          cerr << "Please input either 'human' or 'computer [level]" << endl;
          getline(cin, dummy);
          break;
        }

        cin >> black;
        if(cin.eof()) break;
        if(black == "human"){
          blackP = new Player{&sys}; 
        }
        else if( black == "computer"){
          blackComp=true;
          cin >> level;
          if(cin.eof()) break;

          if(level == "[1]"){
            blackP = new Computer{&sys, 1};
          }
          else if(level == "[2]"){
            blackP = new Computer{&sys, 2};

          }
          else if(level == "[3]"){
            blackP = new Computer{&sys, 3};
          }
          else{
            cerr << "please input 'computer [level]', level can be 1, 2, or 3" << endl;
            blackComp=false;
            getline(cin, dummy);
            break;
          }
        }
        else{
          cerr << "Please input either 'human' or 'computer [level]" << endl;
          getline(cin, dummy);
          break;
        }
        validate.setPlayers(whiteP, blackP);
        sys.gameStart();
      }
    } 
    else if (command == "resign"){
      sys.resign();
    }
    //not finished
    else if (command == "move"){
      //check computer
      if(whiteTurn){
        if(whiteComp) sys.notifyWhite(vector<Point>{}); //bot should disregard the inputs
        else{
          //human
          move = humanMove(dummy);
          if(move.size() == 0){
            cerr << "Input is not complete" << endl;
            continue;
          }
          //promotion
          if(move.at(1).row ==1 || move.at(1).row==8){
            
    }
    else promote = 'z'; //no promotion
        }
      }
      else{
        if(blackComp) sys.notifyBlack(vector<Point>{});
        else{
          //human
        }
      }
      
      // human move
      
      
    }
    else if (command == "setup"){
      while(true){
        cin >> command;
        if(command == "+"){
          //add piece
          board.plus(/*piece, position*/);
        }
        else if(command == "-"){
          board.minus(/*piece, position*/);
        }
        else if(command == "="){
          sys.equal(/*colour*/);
        }
        else if(command == "done"){
          sys.finishSetup();
          break;
        }
        else{
          cout << "command not found" << endl;
        }
      }
      
    }
    else{
      cout << "command not found" << endl;
    }
  }// while
}