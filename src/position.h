#pragma once

#include <cassert>
#include <ostream>

struct Col {
  explicit Col(int col)
    : col(col){};
  int col;
};

struct Row {
  explicit Row(int row)
    : row(row){};
  int row;
};

struct Position {
  Position(const Col & col, const Row & row)
    : col(col.col),
      row(row.row) {
    assert(col.col < 10000 && row.row < 10000);
  }
  int col;
  int row;

  bool operator==(const Position & other) const {
    return col == other.col && row == other.row;
  }

  bool operator<(const Position & other) const {
    return hash() < other.hash();
  }

  bool IsNeighbor(const Position & other) const {
    return (other.col == col && (std::abs(other.row - row) == 1)) ||
           (other.row == row && (std::abs(other.col - col) == 1));
  }

private:
  int hash() const {
    return 10000 * col + row;
  }
};
std::ostream & operator<<(std::ostream & out, const Position & point);
