#include "ui.h"
#include "ui_helper.h"
#include <QPainter>
#include <QPainterPath>
void Rendered::update(const Board & board_p, const Tubes & tubes_p) {
  board = board_p;
  tubes = tubes_p;
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
    case Color::Orange:
      return QColor(255, 175, 0);
    case Color::Yellow:
      return Qt::yellow;
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

  //draw paths
  std::map<Color, QVector<QPoint>> points;
  tubes.WalkPaths([&points, &dh](const Position & pos, Color color) {
    Point p = dh.toPoint(pos);
    points[color].push_back({ p.x, p.y });
  });

  painter.save();
  for (const auto & [color, lines] : points) {
    QColor qcolor = toQColor(color);
    QPen pen(qcolor.darker());
    pen.setStyle(Qt::DotLine);
    pen.setWidth(5);
    painter.setPen(pen);

    QPainterPath pp;
    for (auto point : lines) {
      if (pp.currentPosition() == QPointF{ 0, 0 }) {
        pp.moveTo(point);
      } else {
        pp.lineTo(point);
      }
    }
    painter.drawPath(pp);
  }
  painter.restore();

  //draw grids
  painter.setPen(Qt::white);
  auto lines_to_draw = dh.CalcLines(board.size());
  for (const auto & line : lines_to_draw) {
    QLine qline{ line.a.x, line.a.y, line.b.x, line.b.y };
    painter.drawLine(qline);
  }

  //draw win text
  if (tubes.IsComplete()) {
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 30));
    painter.drawText(rect(), Qt::AlignCenter, QString(" Win!!!"));
  }
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
