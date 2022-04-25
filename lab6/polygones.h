#ifndef POLYGONES_H
#define POLYGONES_H

#include <cmath>
#include <cstdint>
#include <vector>

struct color_t {
  int r = 0, g = 0, b = 0;
  int alpha = 255;
};

struct point_t {
  double x = 0, y = 0;
  color_t color;

  point_t() = default;
  point_t(double x_, double y_) : x(x_), y(y_) {}
  point_t(double x_, double y_, color_t color_) : x(x_), y(y_), color(color_) {}
};

struct edge_t {
  point_t p1;
  point_t p2;
  color_t color;
};

typedef struct {
  std::vector<edge_t> edges;
  bool closed;
  bool filled;
  size_t index_close = 0;
  point_t seed;
  color_t color;
} polygone_t;

typedef std::vector<polygone_t> polygones_t;

polygones_t polygones_init();

bool points_is_equal(const point_t &p1, const point_t &p2);

bool polygone_are_equal(const polygone_t &p1, const polygone_t &p2);

bool polygones_are_equal(const polygones_t &p1, const polygones_t &p2);

void polygone_add_point(polygone_t &polygone, const point_t &point,
                        bool is_new_point);

void polygone_change_point(polygone_t &polygone, const size_t index,
                           point_t point);

void polygone_remove_point(polygone_t &polygone, const size_t index);

void polygone_sizes(point_t &min, point_t &max, const polygone_t &polygone);

bool polygones_is_all_closed(const polygones_t &polygones);

void polygones_unfill(polygones_t &polygones);

#endif // POLYGONES_H
