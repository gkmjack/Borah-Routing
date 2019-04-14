#include "circuit.h"
#include "point.h"
#include "net.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


Point* Circuit::index(int x, int y) const
{
  int i = (y-1)*size+(x-1);
  return points[i];
}

Circuit::Circuit(int size):
size(size)
{
  points = std::vector<Point*>(size*size);
  for (int y = 1; y <= size; y++) {
    for (int x = 1; x <= size; x++) {
      points[(y-1)*size+(x-1)] = new Point(x,y);
    }
  }
  nets = std::list<Net*>();
}

void Circuit::use(int x, int y, bool state)
{
  index(x,y)->use(state);
}

bool Circuit::isUsed(int x, int y) const
{
  return index(x,y)->isUsed();
}

void Circuit::generateMST()
{
  nets.clear();
  // Erase existing contents

  std::list<Point*> unmapped;
  for (auto it = points.begin(); it != points.end(); it++)
    if ((*it)->isUsed()) {
      unmapped.push_back(*it);
    }

  std::list<Point*> mapped(unmapped.size());
  if(unmapped.size()) {
    mapped.push_back(unmapped.front());
    unmapped.pop_front();
  } else {
    std::cout << "Generating MST on an empty set" << std::endl;
    exit(2);
  }
  // Set up the two lists

  while(!unmapped.empty()) {
    int min_cost = 0;
    auto min_m = mapped.end();
    auto min_u = unmapped.end();
    for(auto it_m = mapped.begin(); it_m != mapped.end(); it_m++) {
      for(auto it_u = unmapped.begin(); it_u != unmapped.end(); it_u++) {
        if(!min_cost || Net::cost(*it_m, *it_u) < min_cost) {
          min_cost = Net::cost(*it_m, *it_u);
          min_m = it_m;
          min_u = it_u;
        }
      }
    }
    // Find the minimum-cost net. The map can't contain just 1 element

    nets.push_back(new Net(*min_m, *min_u));
    // Create a new net
    mapped.push_back(*min_u);
    unmapped.erase(min_u);
    // Transfer the net into the other list
  }
}

int Circuit::totalCost() const
{
  int total = 0;
  for (auto it = nets.begin(); it != nets.end(); it++)
    total += (*it)->getWeight();

  return total;
}
