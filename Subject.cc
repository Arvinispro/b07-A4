#include "Subject.h"
#include <vector>

Subject::Subject(std::vector<Observer *> observers): observers{observers}{}
void Subject::notifyObservers() const{
  for(const auto &obs: observers){
    obs -> notify();
  }
}
void Subject::attach(Observer *o){
  observers.emplace_back(o);
}
void Subject::detach(Observer *o){
  for (auto obs = observers.begin(); obs != observers.end(); ) {
    if (*obs == o) {
    obs = observers.erase(obs); 
    } else {
    ++obs; 
   }
  }
}