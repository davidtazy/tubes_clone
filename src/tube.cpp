#include "tube.h"
#include <algorithm>

const bool Tube::IsNewPoint(const Position & pos) const {
  using std::begin;
  using std::end;
  return std::none_of(begin(path), end(path), [pos](const auto & p) {
    return p == pos;
  });
}

bool Tube::Insert(const Position & pos) {

  bool ret = IsNewPoint(pos);
  if (ret) {
    path.push_back(pos);
  }
  return ret;
}

void Tube::StartFrom(const Position & pos) {
  if (IsEndPoint(pos)) {
    path.clear();
  } else {

    auto it = std::find(std::begin(path), std::end(path), pos);
    if (it == std::end(path)) {
      throw std::runtime_error("pos is not end point, neither in path");
    }

    path.erase(it, std::end(path));
  }
  Insert(pos);
}

bool Tube::IsEndPoint(const Position & pos) const {
  return std::any_of(std::begin(end_points), std::end(end_points), [pos](const auto & p) {
    return p == pos;
  });
}

bool Tube::IsComplete() const {

  if (path.size() < 2) {
    return false;
  }

  return IsEndPoint(path.front()) && IsEndPoint(path.back());
}

std::optional<Position> Tube::LastPointInPath() const {
  if (path.empty()) {
    return std::nullopt;
  }

  return { path.back() };
}

bool Tube::IsNeighborToLastPointInPath(const Position & pos) const {

  auto last_point = LastPointInPath();

  if (!last_point.has_value()) {
    return false;
  }

  return pos.IsNeighbor(last_point.value());
}

void Tube::TryToComplete(const std::vector<Position> & neighbors) {
  if (IsComplete()) {
    return;
  }

  auto other_end_point = *end_points.begin();
  if (!IsNewPoint(other_end_point)) {
    other_end_point = *end_points.rbegin();
  }

  //check if one neighbors is the other endpoint

  if (auto it = std::find(neighbors.begin(), neighbors.end(), other_end_point);
      it != neighbors.end()) {
    //it is, so add it
    Insert(other_end_point);
  }
}

void Tubes::AddEndPoint(Color color, const Position & pos) {
  auto & tube = this->tubes[color];
  tube.end_points.insert(pos);
}

Tube & Tubes::Mutate(Color color) {
  return tubes[color];
}

bool Tubes::IsValid() const {
  if (tubes.empty()) {
    return false;
  }

  for (const auto & [color, tube] : tubes) {
    if (tube.end_points.size() != 2) {
      return false;
    }
  }
  return true;
}

void Tubes::WalkPaths(std::function<void(const Position &, Color)> callback) const {
  for (const auto & [color, tube] : tubes) {
    for (const auto & pos : tube.path) {
      callback(pos, color);
    }
  }
}
void Tubes::WalkEndPoints(std::function<void(const Position &, Color)> callback) const {
  for (const auto & [color, tube] : tubes) {
    for (const auto & pos : tube.end_points) {
      callback(pos, color);
    }
  }
}

bool Tubes::IsTubeComplete(Color color) const {
  auto it = tubes.find(color);
  if (it == tubes.end()) {
    return false;
  }

  const auto & tube = it->second;

  return tube.IsComplete();
}

bool Tubes::IsComplete() const {

  return std::all_of(tubes.begin(), tubes.end(), [](const auto & p) {
    return p.second.IsComplete();
  });
}