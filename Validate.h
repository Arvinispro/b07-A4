#ifndef VALIDATE_H
#define VALIDATE_H
#include "Subject.h"
#include "Observer.h"
#include "Util.h"



class Validate: public Subject, public Observer{
  Subject *p1, *p2;
  Point piece, move;
  public:
  Validate();

  ~Validate() override;
  void notify() override;
  moveNode getState() override; //store two validated moves, one for board for display, 
            //one for system tracking game status (ie. did player win after this move?)
  void validate(); //validate the move recieved from player/bot by seeing if that is legal externally
                  //ie. is the king under check? is the destination occupied by other piece?
  void setPlayers(Subject *p1, Subject *p2); //set subjects of validater
  

};
#endif