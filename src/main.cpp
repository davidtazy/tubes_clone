#include "game.h"
#include "ui.h"
#include <QApplication>
#include <QTimer>
#include <QWidget>
#include <iostream>
int main(int argc, char ** argv) {

  QApplication app(argc, argv);

  Rendered view;
  auto game = GameBuilder().with_board_size(5).generate({ "b    ", "    b" }).with_renderer(&view).build();

  app.setQuitOnLastWindowClosed(true);
  view.show();

  /*
  QTimer timer;
  timer.setInterval(100);
  QObject::connect(&timer, &QTimer::timeout, [&game]() {
    game->update();
  });
  timer.start();
*/
  return app.exec();
}
