#ifndef SUBJECT_H
#define SUBJECT_H
export class Subject{
    Observer *observers[2] = {}; //Max observers should be 2, 
                                 // System -> (Player, Bot) or Validator -> (System, Board)
    int numObservers = 0;
  public:
    Subject(const Observer *observers, int numObservers);
    Subject(Subject &other);
    Subject(Subject &&other);
    Subject &
    virtual ~Subject()=0;
    void notifyObservers() const;
}
#endif