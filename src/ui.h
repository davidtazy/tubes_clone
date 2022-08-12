#pragma once
#include "board.h"
#include "renderer.h"
#include "utils.h"

#include <QWidget>

struct Rendered : public IRenderer, public QWidget {

  using QWidget::QWidget;

  void update(const Board & board) override;

  void paintEvent(QPaintEvent *) override;

  void SetInputListener(IInputListener * listener_p) override { listener = listener_p; }

protected:
  void mouseMoveEvent(QMouseEvent * event) override;
  void mousePressEvent(QMouseEvent * event) override;
  void mouseReleaseEvent(QMouseEvent * event) override;

private:
  Board board{ 4 };
  IInputListener * listener{};
};