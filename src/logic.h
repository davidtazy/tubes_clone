#pragma once

#include "board.h"
#include "position.h"
#include "renderer.h"
#include "tube.h"
#include <map>
#include <optional>
#include <set>

class GameLogic : public IInputListener {
  GameLogic(const GameLogic &) = delete;
  GameLogic & operator=(const GameLogic &) = delete;

  GameLogic(GameLogic &&) = delete;
  GameLogic & operator=(GameLogic &&) = delete;

public:
  explicit GameLogic(const Board & board);
  void SetRenderer(IRenderer * renderer) {
    view = renderer;
    update();
  };
  void mouseMoveEvent(const Position & pos) override;
  void mousePressEvent(const Position & pos) override;
  void mouseReleaseEvent() override;

  bool CanStartFromThisPosition(const Position & pos, Color color) const;

  const Board & GetBoard() const { return board; };

  bool IsTubeComplete(Color color) const;

  bool IsComplete() const;

private:
  void update();

  Board board;
  IRenderer * view{};

  Tubes tubes;

  std::optional<Color> current_color;
  Tube * current_tube{};
};