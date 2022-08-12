#pragma once

#include <cassert>
#include <functional>
#include <ostream>
#include <set>
#include <vector>
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

private:
  int hash() const {
    return 10000 * col + row;
  }
};
std::ostream & operator<<(std::ostream & out, const Position & point);

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

  //Cell& operator[](Position)

  void ForEach(const std::function<void(const Position &)> callback) const {
    for (int col = 0; col < grid.size(); ++col) {
      const auto & vrow = grid.at(col);
      for (int row = 0; row < vrow.size(); ++row) {
        Position pos{ Col{ col }, Row{ row } };
        callback(pos);
      }
    }
  }

  std::vector<std::vector<Cell>> grid;
};

enum class Color {
  Black,
  Red,
  Blue,
  Green,
  Max
};

struct Tube {
  std::vector<Position> path;
  std::set<Position> end_points;

  void Reset() {
    path.clear();
  }

  const bool IsNewPoint(const Position & pos) {
    using std::begin;
    using std::end;
    return std::none_of(begin(path), end(path), [pos](const auto & p) {
      return p == pos;
    });
  }

  bool Insert(const Position & pos) {

    bool ret = IsNewPoint(pos);
    if (ret) {
      path.push_back(pos);
    }
    return ret;
  }

  void StartFrom(const Position & pos) {
    if (IsEndPoint(pos)) {
      path.clear();
    } else {

      auto it = std::find(std::begin(path), std::end(path), pos);
      if (it == std::end(path)) {
        throw std::runtime_error("pos is not end point, neither in path");
      }

      path.erase(it, std::end(path));
    }
    Insert(pos);
  }

  bool IsEndPoint(const Position & pos) const {
    return std::any_of(std::begin(end_points), std::end(end_points), [pos](const auto & p) {
      return p == pos;
    });
  }
};

struct Point {
  int x, y;

  bool operator==(const Point & other) const {
    return x == other.x && y == other.y;
  }
};
std::ostream & operator<<(std::ostream & out, const Point & point);

struct Line {
  Point a, b;

  bool operator==(const Line & other) const {
    return a == other.a && b == other.b;
  }
};

struct Rect {
  Point top_left;
  int width;
  int height;

  bool operator==(const Rect & other) const {
    return top_left == other.top_left && width == other.width && height == other.height;
  }
};

std::ostream &
operator<<(std::ostream & out, const Line & line);

struct DrawHelper {

  DrawHelper(int w, int h, int board_size)
    : width(w),
      height(h),
      board_size(board_size) {}

  std::vector<Line> CalcLines(int board_size) {
    std::vector<Line> ret;

    const int lines_to_draw = board_size - 1;

    float vstep = width / static_cast<float>(board_size);
    float hstep = height / static_cast<float>(board_size);
    float v = vstep;
    float h = hstep;
    for (int i = 0; i < lines_to_draw; i++) {
      const int x = static_cast<int>(v);
      v += vstep;

      const int y = static_cast<int>(h);
      h += hstep;

      ret.push_back(Line{ { x, 0 }, { x, height } }); // vertical line
      ret.push_back(Line{ { 0, y }, { width, y } });  // horizon line
    }

    return ret;
  }

  Rect ToRect(const Position & pos) const {

    const int x = pos.col * width / board_size;
    const int y = pos.row * height / board_size;
    const int w = width / board_size;
    const int h = height / board_size;

    return { { x, y }, w, h };
  }

  Position toPosition(int x, int y) const {

    const int col = x * board_size / width;
    const int row = y * board_size / height;

    if ((col >= board_size) || (row >= board_size)) {
      throw std::runtime_error(" position is over boundaries");
    }
    return { Col{ col }, Row(row) };
  }

private:
  int width,
    height, board_size;
};