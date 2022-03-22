#ifndef STRUCTURES_H
#define STRUCTURES_H

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
};

struct line_t {
  point_t p1, p2;
};

struct spectrum_t {
  point_t center;
  double r = 1;
  double step = 1;
};

struct research_spectrum_t {
  spectrum_t spectrum;
  std::vector<double> *time = nullptr;
};

#endif // STRUCTURES_H
