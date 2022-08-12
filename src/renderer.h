#pragma once
#include "utils.h"

struct IInputListener {
  virtual void mouseMoveEvent(const Position & pos) = 0;
  virtual void mousePressEvent(const Position & pos) = 0;
  virtual void mouseReleaseEvent() = 0;
};

struct IRenderer {

  virtual void update(const Board & board) = 0;
  virtual void SetInputListener(IInputListener * listener_p) = 0;
};