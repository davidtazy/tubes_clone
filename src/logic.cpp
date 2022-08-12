#include "logic.h"
#include <algorithm>
#include <cassert>

GameLogic::GameLogic(Board & board)
  : board(board) {

  board.ForEach([this](const Position & pos) {
    auto color = this->board.at(pos);
    if (color != Color::Black) {
      auto & tube = this->tubes[color];
      tube.end_points.insert(pos);
    }
  });

  assert(tubes.size());

  for (const auto & [color, tube] : tubes) {
    assert(tube.end_points.size() == 2);
  }
}

void GameLogic::mouseMoveEvent(const Position & pos) {
  auto color = board.at(pos);
  if (current_color.has_value() && color == Color::Black) {

    if (current_tube->Insert(pos)) {
      board.mutate(pos) = current_color.value();
      update();
    }
  }
}
void GameLogic::mousePressEvent(const Position & pos) {
  auto color = board.at(pos);
  if (CanStartFromThisPosition(pos, color)) {
    current_color = color;
    current_tube = &tubes[color];
    current_tube->Insert(pos);
    update();
  }
}
void GameLogic::mouseReleaseEvent() {
  current_color = std::nullopt;
  current_tube = nullptr;
}

bool GameLogic::CanStartFromThisPosition(const Position & pos, Color color) const {

  if (color == Color::Black) {
    return false;
  }
  auto & tube = tubes.at(color);

  // check if it is endpoint of a tube
  if (tube.IsEndPoint(pos)) {
    return true;
  }

  // check if it is in progress point of a tube
  if (tube.IsInProgressPoint(pos)) {
    return true;
  }

  // other tests to come

  return false;
}

void GameLogic::update() {
  //create board from tubes
  // todo

  if (view) {
    view->update(board);
  }
}