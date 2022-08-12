#include "board.h"
#include "catch2/catch.hpp"
#include "game.h"
#include "ui.h"
#include <iostream>
#include <vector>

const Position pos00{ Col(0), Row(0) };
const Position pos01{ Col(0), Row(1) };
const Position pos10{ Col(1), Row(0) };
const Position pos20{ Col(2), Row(0) };
const Position pos11{ Col(1), Row(1) };
const Position pos21{ Col(2), Row(1) };
const Position pos22{ Col(2), Row(2) };
const Position pos12{ Col(1), Row(2) };

struct FakeRenderer : IRenderer {
  void update(const Board & board, const Tubes & tubes_p) override {
    cpt++;
    tubes = tubes_p;
  }
  void SetInputListener(IInputListener * listener_p) override {}

  int cpt{ 0 };

  Tubes tubes;
};

TEST_CASE("full game") {
  FakeRenderer renderer;
  auto game = GameBuilder().with_board_size(3).generate({ "brg", "xxx", "brg" }).with_renderer(&renderer).build();

  game->logic.mousePressEvent(pos00);
  game->logic.mouseMoveEvent(pos01);
  game->logic.mouseReleaseEvent();

  game->logic.mousePressEvent(pos10);
  game->logic.mouseMoveEvent(pos11);
  game->logic.mouseReleaseEvent();

  REQUIRE_FALSE(game->logic.IsComplete());

  game->logic.mousePressEvent(pos20);
  game->logic.mouseMoveEvent(pos21);
  REQUIRE_FALSE(game->logic.IsComplete());
  game->logic.mouseReleaseEvent();

  REQUIRE(game->logic.IsComplete());

  /*renderer.tubes.WalkPaths([](const Position & pos, Color color) {
    std::cout << "color " << static_cast<int>(color) << " " << pos << "\n";
  });*/
}

TEST_CASE("game logic") {

  SECTION("draw if first pressing on colored cell") {
    FakeRenderer renderer;
    auto game = GameBuilder().with_board_size(2).generate({ "b ", " b" }).with_renderer(&renderer).build();

    game->logic.mousePressEvent(pos00);
    game->logic.mouseMoveEvent(pos01);
    game->logic.mouseReleaseEvent();

    REQUIRE(game->GetBoard().at(pos01) == Color::Blue);
  }

  SECTION("do not draw if first pressing on black cell") {
    FakeRenderer renderer;
    auto game = GameBuilder().with_board_size(2).generate({ "b ", " b" }).with_renderer(&renderer).build();

    game->logic.mousePressEvent(pos01);
    game->logic.mouseMoveEvent(pos01);
    game->logic.mouseReleaseEvent();

    REQUIRE(game->GetBoard().at(pos01) == Color::Black);
  }

  SECTION("can continue draw from uncomplete tube") {
    FakeRenderer renderer;
    auto game = GameBuilder()
                  .with_board_size(3)
                  .generate({ "bxx", "xxx", "xxb" })
                  .with_renderer(&renderer)
                  .build();

    game->logic.mousePressEvent(pos00);
    game->logic.mouseMoveEvent(pos10);
    game->logic.mouseMoveEvent(pos20);
    game->logic.mouseReleaseEvent();
    //pause in drawing tube
    game->logic.mousePressEvent(pos20);
    game->logic.mouseMoveEvent(pos21);
    game->logic.mouseReleaseEvent();

    REQUIRE(game->GetBoard().at(pos21) == Color::Blue);
  }

  SECTION("tube restart from user cell selection") {
    FakeRenderer renderer;
    auto game = GameBuilder()
                  .with_board_size(3)
                  .generate({ "bxx", "xxx", "xxb" })
                  .with_renderer(&renderer)
                  .build();

    game->logic.mousePressEvent(pos00);
    game->logic.mouseMoveEvent(pos10);
    game->logic.mouseMoveEvent(pos20);
    game->logic.mouseReleaseEvent();
    //pos20 is now on the path
    REQUIRE(game->GetBoard().at(pos20) == Color::Blue);

    game->logic.mousePressEvent(pos10);
    game->logic.mouseMoveEvent(pos11);
    game->logic.mouseReleaseEvent();

    // pos20 is no more on the path
    REQUIRE(game->GetBoard().at(pos20) == Color::Black);

    //Pos11 is now on the path
    REQUIRE(game->GetBoard().at(pos11) == Color::Blue);
  }

  SECTION("when tube is in progress, starting from other endpoints remove all path") {
    FakeRenderer renderer;
    auto game = GameBuilder()
                  .with_board_size(3)
                  .generate({ "bxx", "xxx", "xxb" })
                  .with_renderer(&renderer)
                  .build();

    game->logic.mousePressEvent(pos00);
    game->logic.mouseMoveEvent(pos00);
    game->logic.mouseMoveEvent(pos10);
    game->logic.mouseMoveEvent(pos20);
    game->logic.mouseReleaseEvent();
    //restarting from other ends point
    game->logic.mousePressEvent(pos22);
    game->logic.mouseMoveEvent(pos22);
    game->logic.mouseReleaseEvent();

    // tube is emptied
    REQUIRE(game->GetBoard().at(pos10) == Color::Black);
    REQUIRE(game->GetBoard().at(pos20) == Color::Black);
  }

  SECTION("when tube is complete, starting from any endpoints remove all path") {
    FakeRenderer renderer;
    auto game = GameBuilder()
                  .with_board_size(3)
                  .generate({ "bxx", "xxx", "xxb" })
                  .with_renderer(&renderer)
                  .build();

    game->logic.mousePressEvent(pos00);
    game->logic.mouseMoveEvent(pos00);
    game->logic.mouseMoveEvent(pos10);
    game->logic.mouseMoveEvent(pos20);
    game->logic.mouseMoveEvent(pos21);
    game->logic.mouseMoveEvent(pos22);
    game->logic.mouseReleaseEvent();
    //pause in drawing tube
    game->logic.mousePressEvent(pos22);
    game->logic.mouseMoveEvent(pos22);
    game->logic.mouseReleaseEvent();

    REQUIRE(game->GetBoard().at(pos21) == Color::Black);
    REQUIRE(game->GetBoard().at(pos10) == Color::Black);
    REQUIRE(game->GetBoard().at(pos20) == Color::Black);
  }

  SECTION("when mouse released, game automaticaly complete the path if last point in path is neighbor of the endpoint") {
    FakeRenderer renderer;
    auto game = GameBuilder()
                  .with_board_size(3)
                  .generate({ "bxx", "xxx", "xxb" })
                  .with_renderer(&renderer)
                  .build();

    game->logic.mousePressEvent(pos00);
    game->logic.mouseMoveEvent(pos00);
    game->logic.mouseMoveEvent(pos10);
    game->logic.mouseMoveEvent(pos20);
    game->logic.mouseMoveEvent(pos21);
    game->logic.mouseReleaseEvent();

    REQUIRE(game->logic.IsTubeComplete(Color::Blue));
  }
}