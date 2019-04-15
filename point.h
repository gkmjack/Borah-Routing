class Point
{
private:
  const int x;
  const int y;
  bool used;
  Point* parent;

public:
  Point(int, int);
  void use(bool);
  int getX() const;
  int getY() const;
  bool isUsed() const;
  void setParent(Point*);
  Point* getParent() const;
};
