class Point
{
private:
  const int x;
  const int y;
  bool used;

public:
  Point(int, int);
  void use(bool);
  int getX() const;
  int getY() const;
  bool isUsed() const;
};
