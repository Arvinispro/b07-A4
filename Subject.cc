#include "Subject.h"
Subject::Subject(std::vector<Observer *> observers){}
void Subject::notifyObservers() const{}
void Subject::attach(Observer *o){}
void Subject::detach(Observer *o){}