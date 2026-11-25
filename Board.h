#ifndef BOARD_H
#define BOARD_H

#include "Observer.h"
#include "Subject.h"
#include "Util.h"

class Board : public Observer {
  Subject *validate;

public:
  Board(Subject *validate);
  ~Board() override;
  void notify() override;

  void display();
  void plus(char piece, Point location);
  void minus(Point location);
  void clear();
  void standardSetup();

  char at(Point location) const;
  void put(Point location, char piece);
  const std::vector<std::vector<char>> &position() const;
  void applyUnchecked(Point from, Point to, char promotion = 'z');
};

#endif
