#include <vector>
#include <set>

class Point;
class Net;

typedef struct {
  std::set<Net*>::iterator longest;
  std::vector<Net*> flipped;
  bool left;
} loop_t;

class Circuit
{
private:
  std::vector<Point*> all_points;
  std::set<Point*> used_points;
  std::set<Net*> nets;
  const int size;


public:
  Circuit(int);
  void use(int, int, bool);
  bool isUsed(int, int) const;
  void generateMST();
  int totalCost() const;

  std::vector<Point*>::const_iterator findPoint(int, int) const;
  std::set<Net*>::iterator findNet(Point*,Point*) const;

  void linkTree();
  Point* closest_ancestor(Point*, Point*);
  void longest_redundancy(Point*, Point*, loop_t*);
  void borah_route();
};
