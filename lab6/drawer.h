#ifndef DRAWER_H
#define DRAWER_H

#include <QGraphicsScene>
#include <polygones.h>

struct drawer_t {
  QGraphicsScene *scene;

public:
  explicit drawer_t(QGraphicsScene *scene);

  void redraw(polygones_t &polygones);

  void draw_edge(const edge_t &edge);

  void draw_edges(const std::vector<edge_t> &edges);

  void draw_outline(QImage *outline_image, const QColor &outline_color,
                    const std::vector<edge_t> &edges);

  void draw_point(double x, double y, const color_t &color);

  void draw_points(const std::vector<point_t> &points);

  void pixmap_add(QImage image);

  int seed_method(polygone_t &polygone, const point_t &seed_p,
                  const color_t &fg, const color_t &bg, bool is_delay,
                  int sleep_time);

  void clear();
};

#endif // DRAWER_H
