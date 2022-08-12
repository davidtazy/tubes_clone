#include "ui_helper.h"

DrawHelper::DrawHelper(int w, int h, int board_size)
  : width(w),
    height(h),
    board_size(board_size) {}

std::vector<Line> DrawHelper::CalcLines(int board_size) const {
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

Rect DrawHelper::ToRect(const Position & pos) const {

  const int x = pos.col * width / board_size;
  const int y = pos.row * height / board_size;
  const int w = width / board_size;
  const int h = height / board_size;

  return { { x, y }, w, h };
}

Position DrawHelper::toPosition(int x, int y) const {

  const int col = x * board_size / width;
  const int row = y * board_size / height;

  if ((col >= board_size) || (row >= board_size)) {
    throw std::runtime_error(" position is over boundaries");
  }
  return { Col{ col }, Row(row) };
}

Point DrawHelper::toPoint(const Position & pos) const {

  const int dx = width / (2 * board_size);
  const int x = dx + pos.col * width / board_size;

  const int dy = height / (2 * board_size);
  const int y = dy + pos.row * height / board_size;

  return { x, y };
}

std::ostream & operator<<(std::ostream & out, const Point & point) {
  return out << "{" << point.x << ',' << point.y << "}";
}

std::ostream & operator<<(std::ostream & out, const Line & line) {
  return out << "[" << line.a << ',' << line.b << "]";
}