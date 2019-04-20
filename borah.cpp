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

  bool a_e; // Whether the external point is closer to root

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

void Circuit::borah_route(FILE* output)
{
  dump_state(output);
  while(1) {
    pair_t b_m;
    b_m.gain = 0;
    for (auto it_p = used_points.begin(); it_p!=used_points.end(); it_p++) {
      for(auto it_n = nets.begin(); it_n!=nets.end(); it_n++) {
        Point* head = (*it_n)->getHead();
        Point* tail = (*it_n)->getTail();
        if(head == *it_p || tail == *it_p)
          continue;
        Point* steiner = find_steiner_point(head, tail, *it_p);
        if(steiner->isUsed())
          continue;
        // Mark the steiner point;

        int gain;
        Point *rh, *rt;
        std::vector<Net*> flipped;
        bool a_e;
        loop_t loop;
        // =============SUPER IMPORTANT===============
        // if the external point is a child of the tail
        if(closest_ancestor(tail, *it_p) == tail) {
          longest_redundancy(tail, *it_p, &loop);
          if((*(loop.longest))->getWeight() > Net::cost(steiner, tail)) {
            rh = (*(loop.longest))->getHead();
            rt = (*(loop.longest))->getTail();
            gain = (*(loop.longest))->getWeight() - Net::cost(steiner, *it_p);
            flipped = loop.flipped;
            a_e = false;
          } else {
            rh = steiner;
            rt = tail;
            gain = Net::cost(steiner, tail) - Net::cost(steiner, *it_p);
            for (Point* p = *it_p; p!=tail; p=p->getParent()) {
              flipped.push_back(*findNet(p, p->getParent()));
            }
            a_e = false;
          }
        }
        // Otherwise
        else {
          longest_redundancy(head, *it_p, &loop);
          if((*(loop.longest))->getWeight() > Net::cost(steiner, head)) {
            rh = (*(loop.longest))->getHead();
            rt = (*(loop.longest))->getTail();
            gain = (*(loop.longest))->getWeight() - Net::cost(steiner, *it_p);
            flipped = loop.flipped;
            a_e = loop.left;
          } else {
            rh = head;
            rt = steiner;
            gain = Net::cost(steiner, head) - Net::cost(steiner, *it_p);
            // Nothing to flip
            a_e = true;
          }
        }


        if (gain > b_m.gain) {
          b_m.e = *it_p;
          b_m.i = steiner;
          b_m.n = *it_n;
          b_m.gain = gain;
          b_m.rh = rh;
          b_m.rt = rt;
          b_m.flipped = flipped;
          b_m.a_e = a_e;
        }

      }
    }
    if(b_m.gain <= 0) {
      break;
    }
    // If a good steiner point is found
    else {
      use(b_m.i->x, b_m.i->y, true);
      (*findPoint(b_m.i->x, b_m.i->y))->setSteiner();
      nets.erase(nets.find(b_m.n));
      nets.insert(new Net(b_m.i, b_m.n->getTail(), true));
      // Enforce a direction in the new net
      if (b_m.a_e) {
        nets.insert(new Net(b_m.i, b_m.n->getHead(), true));
        nets.insert(new Net(b_m.e, b_m.i, true));
      } else {
        nets.insert(new Net(b_m.n->getHead(), b_m.i, true));
        nets.insert(new Net(b_m.i, b_m.e, true));
      }
      // Flip all the nets that lost shortcut to the root
      for (auto it = b_m.flipped.begin(); it!=b_m.flipped.end(); it++) {
        (*it)->flip();
      }
      // Erase the longest net
      auto it = findNet(b_m.rh, b_m.rt);
      nets.erase(it);

      std::cout << "New cost: " << totalCost() << std::endl;
      dump_state(output);
    }
  }
  std::cout << "All done!! "<< std::endl;
}
