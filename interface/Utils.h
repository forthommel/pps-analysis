#ifndef Utils_h
#define Utils_h

struct XYCoord {
  XYCoord(): x(-999.9), y(-999.9) {;}
  XYCoord(double x, double y): x(x), y(y) {;}
  XYCoord& operator=(const XYCoord& c) { x=c.x; y=c.y; return *this; }

  bool operator==(const XYCoord& c) const { return ((x==c.x) and (y==c.y)); }
  bool operator!=(const XYCoord& c) const { return !(*this==c); }

  double x, y;
};

struct RPHits {
  bool BothArmsLeft() const { return (LN!=XYCoord() and LF!=XYCoord()); }
  bool BothArmsRight() const { return (RN!=XYCoord() and RF!=XYCoord()); }
  bool BothSides() const { return (BothArmsLeft() and BothArmsRight()); }
  XYCoord LN, LF, RN, RF;
};

#endif
