#include "logic.h"
#include <algorithm>
#include <cassert>

GameLogic::GameLogic(const Board & board)
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

  if (current_color.has_value()              // filter if drawing not started
      && (color == Color::Black              // fill empty cell
          || current_color.value() == color) //tube path  also contains endpoints
  ) {

    if (current_tube->Insert(pos)) {
      update();
    }
  }
}
void GameLogic::mousePressEvent(const Position & pos) {
  auto color = board.at(pos);
  if (CanStartFromThisPosition(pos, color)) {
    current_color = color;
    current_tube = &tubes[color];
    current_tube->StartFrom(pos);
    update();
  }
}
void GameLogic::mouseReleaseEvent() {

  if (current_tube && current_tube->LastPointInPath().has_value()) {
    auto last_pos = current_tube->LastPointInPath().value();
    current_tube->TryToComplete(board.CardinalNeighbors(last_pos));
    update();
  }

  current_color = std::nullopt;
  current_tube = nullptr;
}

bool GameLogic::CanStartFromThisPosition(const Position & pos, Color color) const {

  // Cannot start from a empty/black cell
  return color != Color::Black;
}

void GameLogic::update() {

  // clear board
  board.ForEach([this](const Position & pos) {
    this->board.mutate(pos) = Color::Black;
  });

  //create board from tubes
  for (const auto & [color, tube] : tubes) {
    for (const auto & pos : tube.path) {
      board.mutate(pos) = color;
    }
    for (const auto & pos : tube.end_points) {
      board.mutate(pos) = color;
    }
  }

  if (view) {
    view->update(board);
  }
}

bool GameLogic::IsTubeComplete(Color color) const {
  auto it = tubes.find(color);
  if (it == tubes.end()) {
    return false;
  }

  const auto & tube = it->second;

  return tube.IsComplete();
}

bool GameLogic::IsComplete() const {

  return std::all_of(tubes.begin(), tubes.end(), [](const auto & p) {
    return p.second.IsComplete();
  });
}