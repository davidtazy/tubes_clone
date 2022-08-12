#include "board.h"
#include "catch2/catch.hpp"
#include "game.h"
#include "ui.h"
#include "utils.h"
#include <vector>

struct Cell {
  int val{};
};

TEST_CASE("grid ", "[core]") {
  Grid<Cell> board(5);

  auto cell = board.at({ Col{ 2 }, Row{ 2 } });

  SECTION("throw if over bound col") {
    REQUIRE_THROWS(board.at({ Col{ 6 }, Row{ 2 } }));
  }
  SECTION("throw if over bound row") {
    REQUIRE_THROWS(board.at({ Col{ 2 }, Row{ 6 } }));
  }

  SECTION("go through all elements") {
    int cpt = 0;
    board.ForEach([&cpt](const Position & pos) {
      cpt++;
    });
    REQUIRE(cpt == 25);
  }

  SECTION("modify an element") {

    Position pos{ Col(2), Row(2) };
    board.mutate(pos).val = 2;

    REQUIRE(board.at(pos).val == 2);
  }
}

TEST_CASE("calc lines to draw", "[core]") {

  const int w{ 200 };
  const int h{ 100 };
  const int board_size{ 2 };
  DrawHelper dh{ w, h, board_size };

  auto lines = dh.CalcLines(2);
  REQUIRE(lines == std::vector<Line>{ Line{ Point{ w / 2, 0 }, Point{ w / 2, h } },
                                      Line{ Point{ 0, h / 2 }, Point{ w, h / 2 } } });
}

TEST_CASE(" calc rect from position") {
  const int w{ 200 };
  const int h{ 100 };
  const int board_size{ 2 };
  DrawHelper dh{ w, h, board_size };

  auto rect = dh.ToRect({ Col{ 1 }, Row{ 1 } });

  REQUIRE(rect.top_left == Point{ w / 2, h / 2 });
  REQUIRE(rect.width == w / 2);
  REQUIRE(rect.height == h / 2);
}

TEST_CASE("calc position from point in widget") {
  const int w{ 200 };
  const int h{ 100 };
  const int board_size{ 2 };
  DrawHelper dh{ w, h, board_size };

  REQUIRE(dh.toPosition(0, 0) == Position{ Col(0), Row(0) });

  REQUIRE(dh.toPosition(99, 25) == Position{ Col(0), Row(0) });
  REQUIRE(dh.toPosition(101, 25) == Position{ Col(1), Row(0) });

  REQUIRE(dh.toPosition(50, 49) == Position{ Col(0), Row(0) });
  REQUIRE(dh.toPosition(50, 51) == Position{ Col(0), Row(1) });

  SECTION(" over bound will throw") {
    REQUIRE_THROWS(dh.toPosition(201, 50));
    REQUIRE_THROWS(dh.toPosition(100, 101));
  }
}

TEST_CASE("game logic") {
  Position pos00{ Col(0), Row(0) };
  Position pos01{ Col(0), Row(1) };
  Position pos10{ Col(1), Row(0) };
  Position pos20{ Col(2), Row(0) };
  Position pos11{ Col(1), Row(1) };
  Position pos21{ Col(2), Row(1) };
  Position pos22{ Col(2), Row(2) };
  Position pos12{ Col(1), Row(2) };

  //create board
  // B x
  // x B
  Board board(2);
  board.mutate(pos00) = Color::Blue;
  board.mutate(pos11) = Color::Blue;

  GameLogic logic(board);
  struct FakeRenderer : IRenderer {
    void update(const Board & board) override {
      cpt++;
    }
    void SetInputListener(IInputListener * listener_p) override {}

    int cpt{ 0 };
  };

  SECTION("draw if first pressing on colored cell") {
    FakeRenderer renderer;
    auto game = GameBuilder().with_board_size(2).generate({ "b ", " b" }).with_renderer(&renderer).build();

    game->logic.mousePressEvent(pos00);
    game->logic.mouseMoveEvent(pos01);
    game->logic.mouseReleaseEvent();

    REQUIRE(game->board.at(pos01) == Color::Blue);
  }

  SECTION("do not draw if first pressing on black cell") {
    FakeRenderer renderer;
    auto game = GameBuilder().with_board_size(2).generate({ "b ", " b" }).with_renderer(&renderer).build();

    game->logic.mousePressEvent(pos01);
    game->logic.mouseMoveEvent(pos01);
    game->logic.mouseReleaseEvent();

    REQUIRE(game->board.at(pos01) == Color::Black);
  }

  SECTION("do not draw if not a starting cell") {
    FakeRenderer renderer;
    auto game = GameBuilder()
                  .with_board_size(3)
                  .generate({ "bxx", "xxx", "xxb" })
                  .with_renderer(&renderer)
                  .build();

    game->board.mutate(pos10) = Color::Blue;

    game->logic.mousePressEvent(pos10);
    game->logic.mouseMoveEvent(pos11);
    game->logic.mouseReleaseEvent();

    REQUIRE(game->board.at(pos11) == Color::Black);
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

    REQUIRE(game->board.at(pos21) == Color::Blue);
  }

  SECTION("cannot start from beginning of the tube") {
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
    //pause in drawing tube
    game->logic.mousePressEvent(pos00);
    game->logic.mouseMoveEvent(pos01);
    game->logic.mouseReleaseEvent();

    REQUIRE(game->board.at(pos01) == Color::Black);
  }

  SECTION("cannot start from endpoint of a complete tube") {
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
    game->logic.mouseMoveEvent(pos12);
    game->logic.mouseReleaseEvent();

    REQUIRE(game->board.at(pos12) == Color::Black);
  }
}