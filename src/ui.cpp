#include "ui.h"
#include <QPainter>

void Rendered::update(const Board & board_p) {
  board = board_p;
  QWidget::update();
}

QColor toQColor(Color col) {
  switch (col) {
    case Color::Black:
      return Qt::black;
    case Color::Blue:
      return Qt::blue;
    case Color::Red:
      return Qt::red;
    case Color::Green:
      return Qt::green;

    default:
      throw std::runtime_error("not handled color");
  }
}

void Rendered::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  DrawHelper dh{ width(), height(), board.size() };

  // draw cells
  painter.save();
  board.ForEach([this, &painter, &dh](const Position & pos) {
    Rect rect = dh.ToRect(pos);

    QRect qrect;
    qrect.setTopLeft(QPoint(rect.top_left.x, rect.top_left.y));
    qrect.setWidth(rect.width);
    qrect.setHeight(rect.height);

    painter.setBrush(QBrush{ toQColor(this->board.at(pos)) });

    painter.drawRect(qrect);
  });
  painter.restore();

  //draw grids
  auto lines_to_draw = dh.CalcLines(board.size());
  for (const auto & line : lines_to_draw) {
    QLine qline{ line.a.x, line.a.y, line.b.x, line.b.y };
    painter.drawLine(qline);
  }

  //draw txt
  static int cpt = 0;
  painter.setPen(Qt::white);
  painter.setFont(QFont("Arial", 30));
  painter.drawText(rect(), Qt::AlignCenter, QString(" %0").arg(cpt));
  cpt++;
}

#include <QMouseEvent>

void Rendered::mouseMoveEvent(QMouseEvent * event) {
  DrawHelper dh{ width(), height(), board.size() };
  try {
    auto pos = dh.toPosition(event->pos().x(), event->pos().y());

    if (listener) {
      listener->mouseMoveEvent(pos);
    }
  } catch (std::exception e) {
    //ignore exception, user move cursor over widget boundaries
  }
}
void Rendered::mousePressEvent(QMouseEvent * event) {
  DrawHelper dh{ width(), height(), board.size() };
  try {
    auto pos = dh.toPosition(event->pos().x(), event->pos().y());

    if (listener) {
      listener->mousePressEvent(pos);
    }
  } catch (std::exception e) {
  }
}
void Rendered::mouseReleaseEvent(QMouseEvent * event) {
  if (listener) {
    listener->mouseReleaseEvent();
  }
}
