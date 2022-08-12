#include "utils.h"

std::ostream & operator<<(std::ostream & out, const Position & pos) {
  return out << "cr{" << pos.col << ',' << pos.row << "}";
}

std::ostream & operator<<(std::ostream & out, const Point & point) {
  return out << "{" << point.x << ',' << point.y << "}";
}

std::ostream & operator<<(std::ostream & out, const Line & line) {
  return out << "[" << line.a << ',' << line.b << "]";
}
