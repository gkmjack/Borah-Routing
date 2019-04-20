class Point;

class Net
{
private:
  Point* head; // Closer to root
  Point* tail; // Closer to leaf
  const int weight;

public:
  static int cost(Point*, Point*);
  static int cost(int, int, int, int);
  Net(Point*, Point*, bool);
  void flip();
  Point* getHead() const;
  Point* getTail() const;
  void order(Point*, Point*);
  const int getWeight() const;
};
