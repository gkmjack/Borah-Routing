#include "net.h"
#include "point.h"
#include <cmath>

int Net::cost(Point* a, Point* b)
{
  if(a == NULL || b == NULL) return 0;
  return abs(a->getX()-b->getX()) + abs(a->getY()-b->getY());
}

Net::Net(Point* a, Point* b):
a(a), b(b), weight(Net::cost(a,b))
{

}

const Point* Net::getA() const
{
  return a;
}

const Point* Net::getB() const
{
  return b;
}

const int Net::getWeight() const
{
  return weight;
}
