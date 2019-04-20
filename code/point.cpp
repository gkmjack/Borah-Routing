# include "point.h"

Point::Point(int x, int y):
x(x), y(y), parent(0)
{
  used = false;
  steiner = false;
}

void Point::use(bool state)
{
  used = state;
}

int Point::getX() const
{
  return x;
}

int Point::getY() const
{
  return y;
}

bool Point::isUsed() const
{
  return used;
}

bool Point::isSteiner() const
{
  return steiner;
}

void Point::setSteiner()
{
  steiner = true;
}

void Point::setParent(Point* p)
{
  parent = p;
}

Point* Point::getParent() const
{
  return parent;
}
