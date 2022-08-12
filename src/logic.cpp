#include "logic.h"
#include "position.h"
#include <algorithm>
#include <cassert>
#include <functional>

GameLogic::GameLogic(const Board & board)
  : board(board) {

  board.ForEach([this](const Position & pos) {
    auto color = this->board.at(pos);
    if (color != Color::Black) {
      this->tubes.AddEndPoint(color, pos);
    }
  });

  if (!tubes.IsValid()) {
    throw std::runtime_error("GameLogic init from board failed");
  }
}

void GameLogic::mousePressEvent(const Position & pos) {
  auto color = board.at(pos);
  if (CanStartFromThisPosition(pos, color)) {
    current_color = color;
    current_tube = &tubes.Mutate(color);
    current_tube->StartFrom(pos);
    update();
  }
}

void GameLogic::mouseMoveEvent(const Position & pos) {
  auto color = board.at(pos);

  if (current_color.has_value()                          // filter if drawing not started
      && (color == Color::Black                          // fill empty cell
          || current_color.value() == color)             //tube path  also contains endpoints
      && current_tube->IsNeighborToLastPointInPath(pos)) //check if next Cell is the neighbor of the last position in path
  {

    if (current_tube->Insert(pos)) {
      update();
    }
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
  tubes.WalkPaths([this](const Position & pos, Color color) {
    this->board.mutate(pos) = color;
  });
  tubes.WalkEndPoints([this](const Position & pos, Color color) {
    this->board.mutate(pos) = color;
  });

  if (view) {
    view->update(board, tubes);
  }
}

bool GameLogic::IsTubeComplete(Color color) const {
  return tubes.IsTubeComplete(color);
}

bool GameLogic::IsComplete() const {

  return tubes.IsComplete();
}