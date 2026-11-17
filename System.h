#ifndef SYSTEM_H
#define SYSTEM_H
#include "Observer.h"
#include "Util.h"
#include "Player.h"
#include "Computer.h"


class System: public Subject, public Observer{
  
  public:
  Point getstate(); //might NOT be needed b/c system only notify white or black, but still keep it in case
  void notify() override; // get notified by validator to check game status
                        //(ie. if someone wins after the latest move, game is over)
  void notifyPlayer(); //{Player->notify()}
  void notifyBot();//{Computer->notify()}
  void gameStart(Player &player, Computer &bot, int level); //start the game, assign player and bot colours
  void resign();
  void equal(); // set up the colour
  void finishSetup(); //Validate the final board in setup mode



}

#endif