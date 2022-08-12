#include "game.h"

GameBuilder & GameBuilder::with_board_size(int size) {
  board_size = size;
  return *this;
}

GameBuilder & GameBuilder::with_renderer(IRenderer * renderer) {
  this->renderer = renderer;
  return *this;
}

GameBuilder & GameBuilder::generate(const std::vector<std::string> & pattern) {
  this->pattern = pattern;
  return *this;
}

std::unique_ptr<Game> GameBuilder::build() {

  if (board_size <= 0) {
    throw std::runtime_error("cannot greate a game with board size null");
  }

  Board board{ board_size };

  if (pattern.size()) {
    auto to_color = [](char c) {
      switch (c) {
        case 'x':
        case ' ':
          return Color::Black;
        case 'b':
          return Color::Blue;
        case 'r':
          return Color::Red;
        case 'g':
          return Color::Green;
        case 'o':
          return Color::Orange;
        case 'y':
          return Color::Yellow;
        default:
          throw std::runtime_error("not handled color");
      }
    };
    //generate from pattern
    for (int row = 0; row < pattern.size(); row++) {
      std::string srow = pattern.at(row);
      for (int col = 0; col < srow.size(); col++) {
        board.mutate({ Col(col), Row(row) }) = to_color(srow.at(col));
      }
    }
  } else {
    //generate from random
    //todo
  }

  auto game = std::make_unique<Game>(board);

  if (renderer) {
    game->renderer = renderer;
    renderer->SetInputListener(&game->logic);
    game->logic.SetRenderer(renderer);
  }

  return std::move(game);
}