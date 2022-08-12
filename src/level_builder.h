#pragma once
#include "board.h"
#include <string>
struct LevelBuilder {

  static Color ToColor(int color);

  static Board Gen(std::string str);
};