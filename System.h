#ifndef SYSTEM_H
#define SYSTEM_H
#include "Observer.h"
#include "Subject.h"
#include "Util.h"
#include <vector>
#include <optional>
#include "Validate.h"

class System: public Subject, public Observer{
  Validate *validate; //subject
  std::vector<Point> move;
  std::optional<char> promote;

  bool validateGame();
  auto resetGame();
  void gameSummary();
  public:
  
    System(Validate *validate);
    ~System() override;
    moveNode getState() override; //might NOT be needed b/c system only notify white or black, but MUST be kept due to purely virtual
    void notify() override; // get notified by validator to check game status, if game ends, must set gameReady back to false
                          //(ie. if someone wins after the latest move, game is over)
    void notifyWhite(const std::vector<Point> move); // no promotion
    void notifyWhite(const std::vector<Point> move, const std::optional<char> promote); // promotion
    void notifyBlack(const std::vector<Point> move); // no promotion
    void notifyBlack(const std::vector<Point> move, const std::optional<char> promote); // promotion

    void gameStart(); //start the game, make gameReady = true, notify board::display()
    void resign(); //set gameReady=false
    void equal(std::string &colour); // set up the colour
    void finishSetup(); //Validate the final board in setup mode


};

#endif