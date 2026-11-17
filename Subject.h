#ifndef SUBJECT_H
#define SUBJECT_H
export class Subject{
    Observer *observers[2] = {}; //Max observers should be 2, 
                                 // System -> (Player, Bot) or Validator -> (System, Board)
    int numObservers = 0;
  public:
    Subject(const Observer *observers, int numObservers);
    void notifyObservers() const;
    virtual ~Subject()=0;
}
#endif