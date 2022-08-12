#include "game.h"
#include "ui.h"
#include <QApplication>
#include <QTimer>
#include <QWidget>
#include <iostream>
int main(int argc, char ** argv) {

  QApplication app(argc, argv);
  app.setQuitOnLastWindowClosed(true);

  auto view = new Rendered; //qt handle lifecycle
  //auto game = GameBuilder().generate({ "brg", "xxx", "brg" }).with_renderer(&view).build();
  //auto game = GameBuilder().generate({ "xxgyb", "xxxxx", "gxxox", "xxxrb", "yoxxr" }).with_renderer(&view).build();

  MainWindow main_view(view);

  main_view.show();

  return app.exec();
}
