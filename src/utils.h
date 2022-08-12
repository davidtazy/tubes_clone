#pragma once

#include "position.h"
#include <cassert>
#include <functional>
#include <ostream>
#include <set>
#include <vector>
template<typename Cell>
struct Grid {
  explicit Grid(int size)
    : grid(size, std::vector<Cell>(size)) {}

  int size() const {
    return grid.size();
  }

  const Cell & at(const Position & pos) const {
    return grid.at(pos.col).at(pos.row);
  }

  Cell & mutate(const Position & pos) {
    return grid.at(pos.col).at(pos.row);
  }

  void ForEach(const std::function<void(const Position &)> callback) const {
    for (int col = 0; col < grid.size(); ++col) {
      const auto & vrow = grid.at(col);
      for (int row = 0; row < vrow.size(); ++row) {
        Position pos{ Col{ col }, Row{ row } };
        callback(pos);
      }
    }
  }

  std::vector<Position> CardinalNeighbors(const Position & pos) const {
    std::vector<Position> candidates{
      { Col(pos.col + 1), Row(pos.row) },
      { Col(pos.col - 1), Row(pos.row) },
      { Col(pos.col), Row(pos.row + 1) },
      { Col(pos.col), Row(pos.row - 1) }
    };
    std::vector<Position> ret;
    int size = this->size();
    std::copy_if(candidates.begin(), candidates.end(), std::back_inserter(ret), [size](const Position & pos) {
      if (pos.col < 0 || pos.row < 0) {
        return false;
      }
      if ((pos.col >= size) || (pos.row >= size)) {
        return false;
      }
      return true;
    });

    return ret;
  }

private:
  std::vector<std::vector<Cell>> grid;
};
