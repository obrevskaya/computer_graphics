#include "drawing.h"
#include <QBrush>
#include <QPainter>

drawer_t::drawer_t(QGraphicsScene *scene) : scene(scene) {}

void drawer_t::draw_line(const point_t &p1, const point_t &p2,
                         const color_t color) {
  scene->addLine(p1.x, p1.y, p2.x, p2.y,
                 {qRgba(color.r, color.g, color.b, color.alpha)});
}

void drawer_t::draw_point(double x, double y, const color_t &color) {
  scene->addLine(x, y, x, y, {{color.r, color.g, color.b, color.alpha}});
}

void drawer_t::clear() { scene->clear(); }

color_view::color_view(QWidget *parent) : QWidget(parent) {}

void color_view::set_color(const QColor &color) {
  w_color = color;

  update();
}

QColor color_view::get_color() { return w_color; }

void color_view::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);

  QPainter painter(this);

  painter.setPen(QPen(QColor(0, 0, 0)));
  painter.setBrush(QBrush(this->w_color));

  painter.drawRect(0, 0, this->width(), this->height());
}
