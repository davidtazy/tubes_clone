#pragma once

#include <ostream>
#include <position.h>
#include <vector>

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

  DrawHelper(int w, int h, int board_size);

  std::vector<Line> CalcLines(int board_size) const;

  Rect ToRect(const Position & pos) const;

  Position toPosition(int x, int y) const;

  Point toPoint(const Position & pos) const;

private:
  int width,
    height, board_size;
};