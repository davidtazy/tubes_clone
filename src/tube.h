#pragma once

#include <functional>
#include <map>
#include <optional>
#include <set>
#include <vector>

#include "color.h"
#include "position.h"

struct Tube {
  std::vector<Position> path;
  std::set<Position> end_points;

  void Reset();
  bool Insert(const Position & pos);
  void StartFrom(const Position & pos);
  void TryToComplete(const std::vector<Position> & neighbors);

  const bool IsNewPoint(const Position & pos) const;
  bool IsEndPoint(const Position & pos) const;
  bool IsComplete() const;
  std::optional<Position> LastPointInPath() const;
  bool IsNeighborToLastPointInPath(const Position & pos) const;
};

struct Tubes {

  void AddEndPoint(Color color, const Position & position);
  Tube & Mutate(Color color);

  bool IsValid() const;

  void WalkPaths(std::function<void(const Position &, Color)> callback) const;
  void WalkEndPoints(std::function<void(const Position &, Color)> callback) const;

  bool IsTubeComplete(Color color) const;
  bool IsComplete() const;

private:
  std::map<Color, Tube> tubes;
};