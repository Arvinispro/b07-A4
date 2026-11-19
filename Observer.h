#ifndef OBSERVER_H
#define OBSERVER_H
class Observer{
  
  public:
    Observer();
    virtual void notify() = 0;
    virtual ~Observer() = 0;
};
#endif