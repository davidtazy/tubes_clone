
#include "ui_helper.h"
#include "utils.h"
#include <catch2/catch.hpp>

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

  SECTION("go through cardinal neigbors") {

    //in the center
    std::vector<Position> ret = board.CardinalNeighbors({ Col(2), Row(2) });
    std::vector<Position> expected = { { Col(3), Row(2) },
                                       { Col(1), Row(2) },
                                       { Col(2), Row(3) },
                                       { Col(2), Row(1) } };
    REQUIRE(ret == expected);

    //top left
    ret = board.CardinalNeighbors({ Col(0), Row(0) });
    expected = { { Col(1), Row(0) },
                 { Col(0), Row(1) } };

    REQUIRE(ret == expected);

    //bottom right
    ret = board.CardinalNeighbors({ Col(4), Row(4) });
    expected = { { Col(3), Row(4) },
                 { Col(4), Row(3) } };

    REQUIRE(ret == expected);
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

TEST_CASE("test Position to Point conversion") {
  const int w{ 200 };
  const int h{ 100 };
  const int board_size{ 2 };
  DrawHelper dh{ w, h, board_size };

  REQUIRE(dh.toPoint({ Col{ 0 }, Row{ 0 } }) == Point{ 50, 25 });
  REQUIRE(dh.toPoint({ Col{ 1 }, Row{ 1 } }) == Point{ 150, 75 });
}
