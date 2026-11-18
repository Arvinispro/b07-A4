#ifndef VALIDATE_H
#define VALIDATE_H
#include "Subject.h"
#include "Observer.h"
#include "Player.h"
#include "Computer.h"
#include "Util.h"



export class Validate: public Subject, public Observer{
  Player *player;
  Computer *bot;
  Point state;
  public:
  void getstate(); //store two validated moves, one for board for display, 
            //one for system tracking game status (ie. did player win after this move?)
  void validate(); //validate the move recieved from player/bot by seeing if that is legal externally
                  //ie. is the king under check? is the destination occupied by other piece?
}
#endif