#include "circuit.h"
#include "net.h"
#include "point.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

extern Circuit* c;

typedef struct {
  Point* e; // External point
  Point* i; // Steiner point
  Net* n;   // Target net
  int gain;

  Point* rh;
  Point* rt; // Mark the net that will be removed
  std::vector<Net*> flipped; // These nets will be turned around

} pair_t;

inline void compare(int& s, int& x_s, int& y_s, int x, int y, int ex, int ey)
{
  int cost = Net::cost(x, y, ex, ey);
  if(!s || cost < s) {
    x_s = x;
    y_s = y;
    s = cost;
  }
}

Point* find_steiner_point(Point* a, Point* b, Point* e)
{
  int x_l, x_r, y_b, y_t; // left, right, bot, top
  if (a->x < b->x) {
    x_l = a->x;
    x_r = b->x;
  } else {
    x_l = b->x;
    x_r = a->x;
  }

  if(a->y < b->y) {
    y_b = a->y;
    y_t = b->y;
  } else {
    y_b = b->y;
    y_t = a->y;
  }
  // Find the four edges
  int shortest = 0, x_s, y_s;
  int x, y;
  // Bot side
  x = x_l;
  y = y_b;
  for (; x < x_r; x++) {
    compare(shortest, x_s, y_s, x, y, e->x, e->y);
  }
  // Right side
  x = x_r;
  y = y_b;
  for (; y < y_t; y++) {
    compare(shortest, x_s, y_s, x, y, e->x, e->y);
  }
  // Top side
  x = x_r;
  y = y_t;
  for (; x > x_l; x--) {
    compare(shortest, x_s, y_s, x, y, e->x, e->y);
  }
  // Left side
  x = x_l;
  y = y_t;
  for (; y > y_b; y--) {
    compare(shortest, x_s, y_s, x, y, e->x, e->y);
  }

  return *(c->findPoint(x_s, y_s));

}

void Circuit::borah_route()
{
  while(1) {
    pair_t best_move;
    best_move.gain = 0;
    for (auto it_p = used_points.begin(); it_p!=used_points.end(); it_p++) {
      for(auto it_n = nets.begin(); it_n!=nets.end(); it_n++) {
        Point* head = (*it_n)->getHead();
        Point* tail = (*it_n)->getTail();
        if(head == *it_p || tail == *it_p)
          continue;
        Point* steiner = find_steiner_point(head, tail, *it_p);
        /*
        std::cout << "E: " << (*it_p)->x << " " <<(*it_p)->y<<std::endl;
        std::cout << "H: " << head->x << " " <<head->y<<std::endl;
        std::cout << "T: " << tail->x << " " <<tail->y<<std::endl;
        std::cout << "S: " << steiner->x << " " <<steiner->y
        <<std::endl<<std::endl;
        */
        if(steiner->isUsed())
          continue;
        // Mark the steiner point;

        int gain;
        Point *rh, *rt;
        // =============SUPER IMPORTANT===============
        // if the external point is a child of the tail
        if(closest_ancestor(tail, *it_p) == tail) {
          std::set<Net*>::iterator r = longest_redundancy(tail, *it_p);
          if((*r)->getWeight() >= Net::cost(steiner, tail)) {
            rh = (*r)->getHead();
            rt = (*r)->getTail();
            gain = (*r)->getWeight() - Net::cost(steiner, *it_p);
          } else {
            rh = steiner;
            rt = tail;
            gain = Net::cost(steiner, tail) - Net::cost(steiner, *it_p);
          }
        }
        // Otherwise
        else {
          std::set<Net*>::iterator r = longest_redundancy(head, *it_p);
          if((*r)->getWeight() >= Net::cost(steiner, head)) {
            rh = (*r)->getHead();
            rt = (*r)->getTail();
            gain = (*r)->getWeight() - Net::cost(steiner, *it_p);
          } else {
            rh = head;
            rt = steiner;
            gain = Net::cost(steiner, head) - Net::cost(steiner, *it_p);
          }
        }


        if (gain > best_move.gain) {
          best_move.e = *it_p;
          best_move.i = steiner;
          best_move.n = *it_n;
          best_move.gain = gain;
          best_move.rh = rh;
          best_move.rt = rt;
        }

      }
    }
    if(best_move.gain <= 0) {
      break;
    } else {
      use(best_move.i->x, best_move.i->y, true);
      nets.erase(nets.find(best_move.n));
      nets.insert(new Net(best_move.i, best_move.n->getHead()));
      nets.insert(new Net(best_move.i, best_move.n->getTail()));
      nets.insert(new Net(best_move.e, best_move.i));
      auto it = findNet(best_move.rh, best_move.rt);
      nets.erase(it);


      linkTree();
      std::cout << "New cost: " << totalCost() << std::endl;
    }
  }
  std::cout << "All done!! "<< std::endl;
}
