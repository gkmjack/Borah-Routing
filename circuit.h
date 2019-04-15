#include <vector>
#include <set>

class Point;
class Net;


class Circuit
{
private:
  std::vector<Point*> all_points;
  std::set<Point*> used_points;
  std::set<Net*> nets;
  const int size;

  auto findPoint(int, int) const;
  auto findNet(Point*,Point*) const;

public:
  Circuit(int);
  void use(int, int, bool);
  bool isUsed(int, int) const;
  void generateMST();
  int totalCost() const;

  void linkTree();
};
