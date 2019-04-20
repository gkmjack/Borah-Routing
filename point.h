class Point
{
private:
  bool used;
  bool steiner;
  Point* parent;

public:
  const int x;
  const int y;
  Point(int, int);
  void use(bool);
  int getX() const;
  int getY() const;
  bool isUsed() const;
  bool isSteiner() const;
  void setSteiner();
  void setParent(Point*);
  Point* getParent() const;
};
