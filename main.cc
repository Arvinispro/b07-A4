#include <iostream>
#include <Player.h>
#include <Computer.h>
#include <Board.h>
#include <Moves.h>
#include <Observer.h>
#include <Subject.h>
#include <System.h>
#include <Validate.h>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;
int main(){
  string command, white, black, level, dummy;
  char promote;
  Point piece, position;
  System sys;
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
          sys.setWhite(new Player{});
        }
        else if( white == "computer"){
          cin >> level;
          if(cin.eof()) break;
          if(level == "[1]"){
            sys.setWhite(new Computer{1});
          }
          else if(level == "[2]"){
            sys.setWhite(new Computer{2});
          }
          else if(level == "[3]"){
            sys.setWhite(new Computer{3});
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
          sys.setBlack(new Player{}); 
        }
        else if( black == "computer"){
          cin >> level;
          if(cin.eof()) break;

          if(level == "[1]"){
            sys.setBlack(new Computer{1});
          }
          else if(level == "[2]"){
            sys.setBlack(new Computer{2});

          }
          else if(level == "[3]"){
            sys.setBlack(new Computer{3});
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
          if(position.getRow()==1 || position.getRow()==8){
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
  }// while
}