#ifndef DRAWING_H
#define DRAWING_H

#include <QColor>
#include <QGraphicsScene>
#include <QWidget>
#include <structures.h>

struct drawer_t {
  QGraphicsScene *scene;

public:
  explicit drawer_t(QGraphicsScene *scene);

  void draw_line(const point_t &p1, const point_t &p2, color_t color);

  void draw_point(double x, double y, const color_t &color);

  void clear();
};

class color_view : public QWidget {
  Q_OBJECT
public:
  explicit color_view(QWidget *parent = nullptr);
  void paintEvent(QPaintEvent *event);
  void set_color(const QColor &color);
  QColor get_color();

private:
  QColor w_color = QColor(255, 255, 255);
};

#endif // DRAWING_H
