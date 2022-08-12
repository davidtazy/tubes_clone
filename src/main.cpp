#include "game.h"
#include "ui.h"
#include <QApplication>
#include <QTimer>
#include <QWidget>
#include <iostream>
int main(int argc, char ** argv) {

  QApplication app(argc, argv);

  Rendered view;
  auto game = GameBuilder().with_board_size(3).generate({ "brg", "xxx", "brg" }).with_renderer(&view).build();

  app.setQuitOnLastWindowClosed(true);
  view.show();

  return app.exec();
}
