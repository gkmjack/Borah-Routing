// Represent a point in the graph
class Point
{
private:
  bool used; // Whether or not this point is used in circuit
  bool steiner; // If used, whether or not it is a steiner point
  Point* parent; // The immediate parent in the tree structure

public:
  const int x;
  const int y;
  Point(int, int);
  void use(bool); // Mark the point as used
  int getX() const;
  int getY() const;
  bool isUsed() const;
  bool isSteiner() const;
  void setSteiner(); // Mark the point as a steiner point
  void setParent(Point*);
  Point* getParent() const;
};
