#pragma once
#include "logic.h"

struct Game {

  GameLogic logic;
  IRenderer * renderer{};

  const Board & GetBoard() const { return logic.GetBoard(); }

  explicit Game(const Board & board)
    : logic(board) {}
};
#include <memory>
struct GameBuilder {

  GameBuilder & with_board_size(int size);

  GameBuilder & with_renderer(IRenderer * renderer);

  GameBuilder & generate(const std::vector<std::string> & pattern);

  std::unique_ptr<Game> build();

private:
  int board_size{};
  std::vector<std::string> pattern;
  IRenderer * renderer;
};