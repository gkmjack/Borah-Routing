#include <vector>
#include <set>
#include <stdio.h>

class Point;
class Net;

// This structure encodes the information on how to remove a simple loop
typedef struct {
  std::set<Net*>::iterator longest; // Longest net in the loop
  std::vector<Net*> flipped; /* Nets that need to be turned,
                                if the longest net is removed*/
  bool left; // Denote which half of the loop the longest net belongs to
} loop_t;


class Circuit
{
private:
  std::vector<Point*> all_points; // All points, used or not
  std::set<Point*> used_points;
  std::set<Net*> nets;
  const int size;


public:
  Circuit(int);
  void use(int, int, bool); // Mark a point as used
  bool isUsed(int, int) const;
  void generateMST(); // Generate MST using Prim's algorithm
  int totalCost() const;

  std::vector<Point*>::const_iterator findPoint(int, int) const;
  std::set<Net*>::iterator findNet(Point*,Point*) const;

  void linkTree();
  // If the graph is in a tree structure, let each node know its parent
  Point* closest_ancestor(Point*, Point*);
  // Closest common ancestor between two points in the net
  void longest_redundancy(Point*, Point*, loop_t*);
  // Find the longest net in a forming loop
  void borah_route(FILE*);
  void dump_state(FILE*);
  // Dump the current state of circuit into a file for display
};
