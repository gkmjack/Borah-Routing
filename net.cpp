#include "net.h"
#include "point.h"
#include <cmath>
#include <iostream>

int Net::cost(Point* a, Point* b)
{
  if(a == NULL || b == NULL) return 0;
  return abs(a->getX()-b->getX()) + abs(a->getY()-b->getY());
}

int Net::cost(int ax, int ay, int bx, int by)
{
  return abs(ax-bx) + abs(ay-by);
}

Net::Net(Point* a, Point* b, bool directed):
head(a), tail(b), weight(Net::cost(a,b))
{
  if (directed) tail->setParent(head);
}

Point* Net::getHead() const
{
  return head;
}

Point* Net::getTail() const
{
  return tail;
}

void Net::flip()
{
  order(this->tail, this->head);
}

void Net::order(Point* a, Point* b)
{
  if (head == a && tail == b)
  {
    tail->setParent(head);
  }
  else if (tail == a && head == b)
  {
    Point* temp = head;
    head = tail;
    tail = temp;
    //head->setParent(NULL);
    tail->setParent(head);
  }
  else
  {
    std::cout << "The points in arguments are not ends." << std::endl;
    exit(2);
  }
}

const int Net::getWeight() const
{
  return weight;
}
