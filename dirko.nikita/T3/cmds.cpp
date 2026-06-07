#include "cmds.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <numeric>
#include "polygon.hpp"

namespace
{
  bool isEven(const dirko::Polygon &p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOdd(const dirko::Polygon &p)
  {
    return p.points.size() % 2 != 0;
  }
  bool matchVertexies(const dirko::Polygon &p, size_t n)
  {
    return p.points.size() == n;
  }
  template< class Filter >
  void printFilteredSum(std::ostream &out, const std::vector< dirko::Polygon > &polygons, Filter filter)
  {
    std::vector< dirko::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), filter);
    std::vector< double > areas;
    areas.reserve(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), dirko::calcArea);
    out << std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >()) << "\n";
  }
  bool lessArea(const dirko::Polygon &a, const dirko::Polygon &b)
  {
    return dirko::calcArea(a) < dirko::calcArea(b);
  }

  bool lessVertex(const dirko::Polygon &a, const dirko::Polygon &b)
  {
    return a.points.size() < b.points.size();
  }
}

void dirko::area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }
  out << std::fixed << std::setprecision(1);

  if (param == "MEAN") {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), dirko::calcArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());
    out << sum / polygons.size() << "\n";
  } else if (param == "EVEN") {
    printFilteredSum(out, polygons, isEven);
  } else if (param == "ODD") {
    printFilteredSum(out, polygons, isOdd);
  } else {
    size_t n = std::stoul(param);
    if (n < 3) {
      throw std::invalid_argument("invalid");
    }
    printFilteredSum(out, polygons, std::bind(matchVertexies, std::placeholders::_1, n));
  }
}

void dirko::max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  if (polygons.empty()) {
    throw std::invalid_argument("invalid");
  }
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  if (param == "AREA") {
    auto it = std::max_element(polygons.begin(), polygons.end(), lessArea);
    out << std::fixed << std::setprecision(1) << dirko::calcArea(*it) << "\n";
  } else if (param == "VERTEXES") {
    auto it = std::max_element(polygons.begin(), polygons.end(), lessVertex);
    out << it->points.size() << "\n";
  } else {
    throw std::invalid_argument("invalid");
  }
}

void dirko::min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  if (polygons.empty()) {
    throw std::invalid_argument("invalid");
  }
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  if (param == "AREA") {
    auto it = std::min_element(polygons.begin(), polygons.end(), lessArea);
    out << std::fixed << std::setprecision(1) << dirko::calcArea(*it) << "\n";
  } else if (param == "VERTEXES") {
    auto it = std::min_element(polygons.begin(), polygons.end(), lessVertex);
    out << it->points.size() << "\n";
  } else {
    throw std::invalid_argument("invalid");
  }
}
//
// void dirko::count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
// {
//   std::string param;
//   if (!(in >> param)) {
//     throw std::invalid_argument("invalid");
//   }
//
//   if (param == "EVEN") {
//     out << std::count_if(polygons.begin(), polygons.end(), isEven) << "\n";
//   } else if (param == "ODD") {
//     out << std::count_if(polygons.begin(), polygons.end(), isOdd) << "\n";
//   } else if (isNumber(param)) {
//     size_t n = std::stoul(param);
//     if (n < 3) {
//       throw std::invalid_argument("invalid");
//     }
//     out << std::count_if(polygons.begin(), polygons.end(), std::bind(hasVertexCount, std::placeholders::_1, n)) <<
//     "\n";
//   } else {
//     throw std::invalid_argument("invalid");
//   }
// }
