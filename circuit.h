#include <list>
#include <vector>

class Point;
class Net;


class Circuit
{
private:
  std::vector<Point*> points;
  std::list<Net*> nets;
  const int size;

  Point* index(int, int) const;

public:
  Circuit(int);
  void use(int, int, bool);
  bool isUsed(int, int) const;
  void generateMST();
  int totalCost() const;
};
