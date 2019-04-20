#include "circuit.h"
#include "point.h"
#include "net.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


std::vector<Point*>::const_iterator Circuit::findPoint(int x, int y) const
{
  int index = (y-1)*size+(x-1);
  std::vector<Point*>::const_iterator it = all_points.begin();
  for (int i = 0; i < index; i++) it++;
  return it;
}

std::set<Net*>::iterator Circuit::findNet(Point* p1, Point* p2) const
{
  auto it = nets.begin();
  while(it != nets.end()) {
    Point* a = (*it)->getHead();
    Point* b = (*it)->getTail();
    if((a == p1 && b == p2) || (a == p2 && b == p1)) {
      return it;
    }
    it++;
  }
  if(it == nets.end()) {
    std::cout << "Couldn't find the net." << std::endl;
    exit(1);
  }
  return it;

}

Circuit::Circuit(int size):
size(size)
{
  all_points = std::vector<Point*>(size*size);
  for (int y = 1; y <= size; y++) {
    for (int x = 1; x <= size; x++) {
      all_points[(y-1)*size+(x-1)] = new Point(x,y);
    }
  }
  nets = std::set<Net*>();
}

void Circuit::use(int x, int y, bool state)
{
  Point* p = *findPoint(x,y);
  p->use(state);
  if(state) {
    used_points.insert(p);
  } else {
    used_points.erase(used_points.find(p));
  }
}

bool Circuit::isUsed(int x, int y) const
{
  Point* p = *findPoint(x,y);
  return p->isUsed();
}

void Circuit::generateMST()
{
  nets.clear();
  // Erase existing contents

  std::set<Point*> unmapped = used_points;

  std::set<Point*> mapped;
  if(unmapped.size()) {
    mapped.insert(*unmapped.begin());
    unmapped.erase(unmapped.begin());
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

    nets.insert(new Net(*min_m, *min_u, false));
    // Create a new net
    mapped.insert(*min_u);
    unmapped.erase(min_u);
    // Transfer the net into the other list
  }
  linkTree();
}

int Circuit::totalCost() const
{
  int total = 0;
  for (auto it = nets.begin(); it != nets.end(); it++)
    total += (*it)->getWeight();

  return total;
}

void Circuit::linkTree()
{
  for (auto it = all_points.begin(); it != all_points.end(); it++)
    (*it)->setParent(NULL);

  std::set<Net*> ump_nets = nets;
  std::set<Point*> ump_pts = used_points;
  std::set<Point*> mp_pts;
  // Initialize things

  if(ump_nets.size()) {
    mp_pts.insert(*ump_pts.begin());
    ump_pts.erase(ump_pts.begin());
    (*mp_pts.begin())->setParent(NULL);
  } else {
    std::cout << "Generating tree on an empty set" << std::endl;
    exit(2);
  }
  // Set up the root

  while(!ump_nets.empty()) {
    for(auto it = ump_nets.begin(); it != ump_nets.end(); it++) {
      Point* a = (*it)->getHead();
      Point* b = (*it)->getTail();
      if(mp_pts.find(a) != mp_pts.end() && mp_pts.find(b) != mp_pts.end()) {
        std::cout << "A loop is contained!" << std::endl;
        exit(2);
      }
      else if (ump_pts.find(a) != ump_pts.end()
            && ump_pts.find(b) != ump_pts.end())
        continue;
      else {
        std::set<Point*>::iterator child;
        if (mp_pts.find(a) != mp_pts.end()) {
          (*it)->order(a, b);
          child = ump_pts.find(b);
        } else {
          (*it)->order(b, a);
          child = ump_pts.find(a);
        }

        // Mark parent and child
        ump_nets.erase(it);
        mp_pts.insert(*child);
        ump_pts.erase(child);
        break;
      }
    }

  }
  std::cout << "Tree formation complete" << std::endl;
}

Point* Circuit::closest_ancestor(Point* p1, Point* p2)
{
  // Find the closest common ancestor between the two nodes
  std::vector<Point*> p1_anc, p2_anc;
  bool go_again = true;
  Point* temp = p1;
  while(temp) {
    p1_anc.push_back(temp);
    temp = temp->getParent();
  }
  temp = p2;
  while(temp) {
    p2_anc.push_back(temp);
    temp = temp->getParent();
  }
  // Put the ancestors of each node in a vector
  Point* cca = NULL; // Closest common ancestor
  for (auto it1 = p1_anc.begin(); it1 != p1_anc.end(); it1++) {
    bool found = false;
    for (auto it2 = p2_anc.begin(); it2 != p2_anc.end(); it2++) {
      if (*it1 == *it2) {
        found = true;
        cca = *it1;
        break;
      }
    }
    if(found) break;
  }
  if(cca) return cca;
  else {
    std::cout << "Can't find the closest common ancestor" << std::endl;
    exit(2);
  }
}

void Circuit::longest_redundancy(Point* p1, Point* p2, loop_t* loop)
{
  Point* cca = closest_ancestor(p1, p2);

  // Find the longest among nets up to that ancestor
  loop->longest = nets.end();
  bool left = true;
  Point* temp;
  std::vector<Net*> up_trace;
redo:
  temp = left ? p1 : p2;
  up_trace.clear();
  while(temp != cca) {
    auto it = findNet(temp, temp->getParent());
    if (loop->longest == nets.end() ||
        (*it)->getWeight() > (*(loop->longest))->getWeight()) {
      loop->longest = it;
      loop->flipped = up_trace;
      loop->left = left;
    }
    up_trace.push_back(*it);
    temp = temp->getParent();
  }
  if(left) {
    left = false;
    goto redo;
  }
}
