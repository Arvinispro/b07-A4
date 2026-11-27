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
#include <algorithm>
#include <cctype>
#include <optional>
#include <memory>


using namespace std;
vector<Point> humanMove(string &dummy){
  // validate inputs
  Point piece{'z', 0}, position{'z', 0};
  while(true){
    try{
      if(cin.eof()) return vector<Point>{};
      cerr << "from: " << endl;
      cin >> piece;
      cerr << "to: " << endl;
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

optional<char> promotion(string &dummy){
  char promote;
  string allowed = "QRNB";
  while(true){
    try{
      cin >> promote;
      if(cin.eof()) return nullopt;

      if(cin.fail()) throw invalid_argument{"Character only"};
      bool check = !(allowed.find(promote)==string::npos);
      if(!check) throw invalid_argument{"Promotion not allowed"};
      break;
    }
    catch(invalid_argument &){
      cin.clear();
      getline(cin, dummy);
      // cerr << "Input promotion again" << endl;
      cerr << "promote to(QRNB): ";
    }
  }
  return promote;
}
int main(){ // input check
  string command, player, level, dummy;
  optional<char> promote;
  vector<Point> move;
  Validate validate;
  System sys{&validate};
  Board board{&validate};
  validate.setBoard(&board);
  unique_ptr<Subject> whiteP=nullptr, blackP=nullptr; //with unique_ptr, the content will auto delete when refer to new stuff. whiteP and blackP are owned by main.
  
  while (true) {
    cout << "game, move, setup, resign: " << endl;
    cin >> command;

    // Terminate the program if enter 'quit' or reach EOF.
    if ( cin.fail() || command == "quit" || cin.eof()) break; //delete everything

    if(command == "game"){
      while(!gameReady){
        // the user will keep input their choice for players until they are valid
        cin >> player;
        if(cin.eof()) break;
        if(cin.fail()){
          // cerr << "White Black: " << endl;
          cin.clear();
          continue;
        }

        if(player == "human"){
          whiteP = make_unique<Player>(&sys);
        }
        else if( player == "computer"){
          whiteComp = true;
          cin >> level;
          if(cin.eof()) break;
          if(level == "[1]"){
            whiteP = make_unique<Computer>(&sys, 1);
          }
          else if(level == "[2]"){
            whiteP = make_unique<Computer>(&sys, 2);
          }
          else if(level == "[3]"){
            whiteP = make_unique<Computer>(&sys, 3);
          }
          else{
            cerr << "please input 'computer [level]', level can be 1, 2, or 3" << endl;
            cerr << "White Black: " << endl;
            whiteComp = false;
            getline(cin, dummy);
            continue;
          }
        }
        else{
          cerr << "Please input either 'human' or 'computer [level]" << endl;
          cerr << "White Black: " << endl;
          getline(cin, dummy);
          continue;
        }
        cin >> player;
        if(cin.eof()) break;
        if(cin.fail()){
          cerr << "White Black: " << endl;
          cin.clear();
          continue;
        }

        if(player == "human"){
          blackP = make_unique<Player>(&sys); 
        }
        else if( player == "computer"){
          blackComp=true;
          cin >> level;
          if(cin.eof()) break;

          if(level == "[1]"){
            blackP = make_unique<Computer>(&sys, 1); 
          }
          else if(level == "[2]"){
            blackP = make_unique<Computer>(&sys, 2); 

          }
          else if(level == "[3]"){
            blackP = make_unique<Computer>(&sys, 3); 
          }
          else{
            cerr << "please input 'computer [level]', level can be 1, 2, or 3" << endl;
            cerr << "White Black: " << endl;
            blackComp=false;
            getline(cin, dummy);
            continue;
          }
        }
        else{
          cerr << "Please input either 'human' or 'computer [level]" << endl;
          cerr << "White Black: " << endl;
          getline(cin, dummy);
          continue;
        }
        validate.setPlayers(whiteP.get(), blackP.get()); //returns the raw address for validate. 
        sys.gameStart();
        board.display();
      }
    } 

    else if (command == "setup"){
      while(true){
        cout << "select +, -, = or done: ";
        cin >> command;
        if(cin.eof()) break;

        if(command == "+"){
          char piece;
          string allowed = "KQNBRPkqnbrp";
          Point position{'z', 0};
          try{
            cin >> piece >> position;
            if(cin.fail()) throw invalid_argument{"Expecting char"};
            bool check = !(allowed.find(piece)==string::npos);
            if(!check) throw invalid_argument{"Invalid piece"};

          }
          catch(invalid_argument &r) {
            cin.clear();
            getline(cin, dummy);
            cerr << r.what() << endl;
            continue;
          }
          board.plus(piece, position);
        }
        else if(command == "-"){
          Point piece{'z', 0};
          try{
            cin >> piece ;
          }
          catch(invalid_argument &r) {
            cin.clear();
            getline(cin, dummy);
            cerr << r.what() << endl;
            continue;
          }
          board.minus(piece);
        }
        else if(command == "="){
          cout << "white or black?: ";
          string color;
          cin >> color;
          if(cin.fail()){
            cin.clear();
            continue;
          }
          if(color != "white" && color != "black" && color != "White" && color != "Black") continue;
          for (char &c : color) c = tolower(static_cast<unsigned char>(c)); //turn to all lower case
          sys.equal(color); //color is either white or black
        }
        else if(command == "done"){
          try{
            sys.finishSetup(); //finishSetup must validate the board
          }
          catch(runtime_error &r){
            cerr << r.what() << endl;
            continue;
          }
          break;
        }
        else{
          cout << "command not found" << endl;
          getline(cin, dummy);
        }
      }
      
    }
    
    else if (command == "resign"){
      if(!gameReady) continue;
      sys.resign();
    }
    
    else if (command == "move"){ //will keep going until a move is valid
      if(!gameReady) continue;

      string pawns = "pP";
      if(whiteTurn){
        if(whiteComp) sys.notifyWhite(vector<Point>{}); //bot should disregard the inputs
        else{
          //human
          try{
            Point piece{'z', 0}, position{'z', 0};
            cin >> piece >> position;
            move = vector<Point>{piece, position};
          }
          catch(invalid_argument &){
            move = humanMove(dummy);
          }
          
          if(move.size() == 0){
            cerr << "Input is not complete" << endl;
            continue;
          }
          //promotion
          char piece = game[8-move.at(0).row][move.at(0).col-'a'];
          bool check = !(pawns.find(piece) == string::npos);
          if(check && (move.at(1).row ==1 || move.at(1).row==8)){
            promote = promotion(dummy);
            if(!promote) continue; //returned nullopt
            sys.notifyWhite(move, promote);
            continue;
          }
          sys.notifyWhite(move);
        }
      }
      else{ //black turn
        if(blackComp) sys.notifyBlack(vector<Point>{});
        else{
          //human
          try{
            Point piece{'z', 0}, position{'z', 0};
            cin >> piece >> position;
            move = vector<Point>{piece, position};
          }
          catch(invalid_argument &){
            move = humanMove(dummy);
          }
          if(move.size() == 0){
            cerr << "Input is not complete" << endl;
            continue;
          }
          //promotion
          char piece = game[8-move.at(0).row ][move.at(0).col-'a'];
          bool check = !(pawns.find(piece) == string::npos);
          if(check && (move.at(1).row ==1 || move.at(1).row==8)){
            promote = promotion(dummy);
            if(!promote) continue; //returned nullopt
            sys.notifyBlack(move, promote);
            continue;
          }
          sys.notifyBlack(move);
        }
      }
      // board.display();
    }
    
    
    else{
      cout << "command not found" << endl;
    }
  }// while
}