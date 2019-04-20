class Point;

// Represents a wire in the network
class Net
{
private:
  Point* head; // Closer to root
  Point* tail; // Closer to leaf
  const int weight; // Cost

public:
  static int cost(Point*, Point*);
  static int cost(int, int, int, int);
  // Statically compute distance between two points
  Net(Point*, Point*, bool);
  void flip(); // Turn the head and tail of the net
  Point* getHead() const;
  Point* getTail() const;
  void order(Point*, Point*); // Enforce a certain direction
  const int getWeight() const;
};
