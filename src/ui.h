#pragma once
#include "board.h"
#include "game.h"
#include "renderer.h"
#include "tube.h"

#include <QWidget>

struct Rendered : public IRenderer, public QWidget {

  using QWidget::QWidget;

  void update(const Board & board, const Tubes & tubes) override;

  void paintEvent(QPaintEvent *) override;

  void SetInputListener(IInputListener * listener_p) override { listener = listener_p; }

protected:
  void mouseMoveEvent(QMouseEvent * event) override;
  void mousePressEvent(QMouseEvent * event) override;
  void mouseReleaseEvent(QMouseEvent * event) override;

private:
  Board board{ 4 };
  Tubes tubes;
  IInputListener * listener{};
};

#include <QMainWindow>
struct MainWindow : public QWidget {

  MainWindow(Rendered * board_view);

  std::unique_ptr<Game> game;
  int level{ 0 };
};