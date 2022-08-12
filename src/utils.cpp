#include "utils.h"

std::ostream & operator<<(std::ostream & out, const Position & pos) {
  return out << "cr{" << pos.col << ',' << pos.row << "}";
}
