class Point;

class Net
{
private:
  const Point* a;
  const Point* b;
  const int weight;

public:
  static int cost(Point* a, Point* b);
  Net(Point*, Point*);
  const Point* getA() const;
  const Point* getB() const;
  const int getWeight() const;
};
