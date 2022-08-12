#include "level_builder.h"
#include <sstream>

Color LevelBuilder::ToColor(int color) {

  if (color >= static_cast<int>(Color::Max)) {
    throw std::runtime_error("LevelBuilder::ToColor: cannot handle this color");
  }

  //hacky..
  return static_cast<Color>(color);
}

Board LevelBuilder::Gen(std::string str) {
  //first line is board size
  std::istringstream in(str);

  int board_size{ 0 };

  in >> board_size >> board_size;
  in.ignore();

  Board board(board_size);
  for (int row = 0; row < board_size; row++) {
    for (int col = 0; col < board_size; col++) {

      int c;
      in >> c;
      board.mutate({ Col{ col }, Row{ row } }) = ToColor(c);
    }
    in.ignore();
  }

  return board;
}
