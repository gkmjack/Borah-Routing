#include "circuit.h"
#include "point.h"
#include "net.h"

void Circuit::dump_state(FILE* output)
{
  for(int y = size; y > 0; y--) {
    for(int x = 1; x <= size; x++) {
      Point* p = *findPoint(x,y);
      if(!p->isUsed())
        fputs("0", output);
      else
        fputs(p->isSteiner()?"2":"1", output);
    }
    fputs("\n", output);
  }
  // Dump all the point usage

  for(auto it = nets.begin(); it!=nets.end(); it++) {
    Point* head = (*it)->getHead();
    Point* tail = (*it)->getTail();
    fprintf(output,"(%d %d %d %d)\n",
    head->x, head->y, tail->x, tail->y);
  }
  // Dump all the nets

  fputs("*****************\n", output);
}
