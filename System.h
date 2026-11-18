#ifndef SYSTEM_H
#define SYSTEM_H
#include "Observer.h"
#include "Util.h"
#include "Player.h"
#include "Computer.h"


class System: public Subject, public Observer{
  Moves *white, *black;
  Subject *validate;
  Observer *board;
  public:
  System();
  ~System() override;
  Point getstate(); //might NOT be needed b/c system only notify white or black, but still keep it in case
  void notify() override; // get notified by validator to check game status, if game ends, must set gameReady back to false
                        //(ie. if someone wins after the latest move, game is over)
  void notifyWhite(const Point &piece, const Point &position, const char &promote); //{set move into state, white->notify()}
  void notifyBlack(const Point &piece, const Point &position, const char &promote); //{set move into state, black->notify()}
  void gameStart(); //start the game, make gameReady = true, notify board::display()
  void resign(); //set gameReady=false
  void equal(); // set up the colour
  void finishSetup(); //Validate the final board in setup mode

  void setWhite(Moves *white);
  void setBlack(Moves *black);



}

#endif