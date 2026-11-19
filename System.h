#ifndef SYSTEM_H
#define SYSTEM_H
#include "Observer.h"
#include "Subject.h"
#include "Util.h"


class System: public Subject, public Observer{
  Subject *validate; //subject
  public:
  
    System(Subject *validate);
    ~System() override;
    std::vector<Point> &getState() override; //might NOT be needed b/c system only notify white or black, but MUST be kept due to purely virtual
    void notify() override; // get notified by validator to check game status, if game ends, must set gameReady back to false
                          //(ie. if someone wins after the latest move, game is over)
    void notifyWhite(const Point &piece, const Point &position, const char &promote); // white is 1st obs, {set move into state, white->notify()}
    void notifyBlack(const Point &piece, const Point &position, const char &promote); // black is 2nd obs, {set move into state, black->notify()}
    void gameStart(); //start the game, make gameReady = true, notify board::display()
    void resign(); //set gameReady=false
    void equal(std::string &colour); // set up the colour
    void finishSetup(); //Validate the final board in setup mode


};

#endif