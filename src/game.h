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

  GameBuilder & with_renderer(IRenderer * renderer);

  GameBuilder & generate(const std::vector<std::string> & pattern);

  GameBuilder & with_level(int level);

  std::unique_ptr<Game> build();

private:
  int level{ -1 };
  std::vector<std::string> pattern;
  IRenderer * renderer;
};