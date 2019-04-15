#include "circuit.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

typedef struct {
  Point* p_e;
  Point* p_i;
  Point* n_p;
  Point* n_c;
  // Can't use a net, because the net to be removed may be between the
  // steiner point and its parent
  int gain;
} pair_t;

void Circuit::borah_route()
{
  while(1) {
    pair_t best_move = NULL;
    for(auto it_p=used_points.begin(); it_p!=used_points.end(); it_p++) {
      for(auto it_n=nets.begin(); it_n!=nets.end(); it_n++) {
        Point* a = (*it_n)->getA();
        Point* b = (*it_n)->getB();
        Point* parent, child;
        if (b->getParent() == a) {
          parent = a;
          child = b;
        } else {
          parent = b;
          child = a;
        }
        // Decide which one between this pair is closer to the head
        
      }
    }
  }
}
