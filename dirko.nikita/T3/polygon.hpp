#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>
namespace dirko
{
  struct Point
  {
    int x, y;
  };
  bool operator==(const Point &lhs, const Point &rhs);
  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream &operator>>(std::istream &in, Point &point);
  std::istream &operator>>(std::istream &in, Polygon &polygon);
}

#endif
