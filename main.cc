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
int main(){
  string command, white, black, level, dummy;
  char promote;
  Point piece, position;
  Validate validate;
  System sys{&validate};
  Board board{&validate};
  Subject *whiteP=nullptr, *blackP=nullptr;
  
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
      
      // human move
      while(true){
        try{
          cin >> piece;
          if(somthing){
            throw 
          }
        }
        
        if(!(cin >> piece)){
          if(cin.eof()) break; 
          //check valid
          cin.clear();
          getline(cin, dummy);
          cerr << "piece is invalid" << endl;
          continue;
        }
        else if(!(cin >> position)){
          if(cin.eof()) break;
          //check valid
          cin.clear();
          getline(cin, dummy);
          cerr << "position is invalid" << endl;
          continue;
        }
        else{
          //promotion
          if(position.row==1 || position.row==8){
            while(!(cin>>promote)){
              if(cin.eof()) break;
              //check valid
              getline(cin, dummy);
              cerr << "promote is invalid" << endl;

            }
          }
          else promote = 'z'; //no promotion
          if(whiteTurn) sys.notifyWhite(piece, position, promote);
          else sys.notifyBlack(piece, position, promote);
          break;
        }
      }
      
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