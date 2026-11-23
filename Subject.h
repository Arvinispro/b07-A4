#ifndef SUBJECT_H
#define SUBJECT_H
#include "Observer.h"
#include "Util.h"
#include <vector>
class Subject{
  protected:
    std::vector<Observer *> observers; //Max observers should be 2, 
                                 // System -> (Player, Bot) or Validator -> (System, Board)
  public:
    Subject(std::vector<Observer *> observers);
    virtual ~Subject()=0;
    virtual moveNode getState()=0; //since all the getState are using Point
    void notifyObservers() const;
    void attach(Observer *o);
    void detach(Observer *o);
};
#endif