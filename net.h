class Point;

class Net
{
private:
  Point* a;
  Point* b;
  const int weight;

public:
  static int cost(Point* a, Point* b);
  Net(Point*, Point*);
  Point* getA() const;
  Point* getB() const;
  const int getWeight() const;
};
