#include "request.h"
#include "polygones.h"
#include <cmath>

static bool is_new_point = false;

request::request(drawer_t &drawer, const params_t &params)
    : params(params), drawer(drawer) {}

polygones_t request::handle() {
  static polygones_t polygones = polygones_init();
  switch (params.command) {
  case SET_POLYGONES:
    polygones = params.polygones;
    polygones_unfill(polygones);
    break;
  case ADD_POINT:
    add_point(polygones, params.point);
    break;
  case CLOSE_POLYGON:
    close_polygon(polygones);
    break;
  case ADD_HOLE:
    add_hole(polygones);
    break;
  case CHANGE_POINT:
    change_point(polygones, params.index, params.point);
    break;
  case REMOVE_POINT:
    remove_point(polygones, params.index);
    break;
  case FILL:
    fill(polygones, params.point, params.color, params.sleep_data.is_sleep,
         params.sleep_data.sleep_time, params.time);
    break;
  case CLEAR_SCREEN:
    polygones.clear();
    drawer.clear();
    break;
  default:
    break;
  }
  return polygones;
}

void request::add_point(polygones_t &polygones, const point_t &point) {
  if (polygones.empty())
    polygones.push_back(polygone_t{});
  drawer.draw_point(point.x, point.y, point.color);
  polygone_add_point(polygones.back(), point, is_new_point);
  drawer.draw_edge(polygones.back().edges.back());
  is_new_point = false;
}

void request::change_point(polygones_t &polygones, size_t index,
                           point_t point) {
  int polygones_i = -1;
  int polygones_j = -1;
  size_t count_edges = 0;
  for (size_t i = 0; polygones_i == -1 && i < polygones.size(); i++) {
    for (size_t j = 0; polygones_i == -1 && j < polygones[i].edges.size(); j++)
      if (count_edges + j == index) {
        polygones_i = i;
        polygones_j = j;
      }
    count_edges += polygones[i].edges.size() - 1;
  }
  if (polygones_i == -1 || polygones_j == -1) {
    add_point(polygones, point);
    return;
  }
  if (points_is_equal(point, polygones[polygones_i].edges[polygones_j].p1))
    return;
  polygone_change_point(polygones[polygones_i], polygones_j, point);
  drawer.redraw(polygones);
}

void request::remove_point(polygones_t &polygones, size_t index) {
  int polygones_i = -1;
  int polygones_j = -1;
  for (size_t i = 0; polygones_i == -1 && i < polygones.size(); i++)
    for (size_t j = 0; polygones_i == -1 && j < polygones[i].edges.size(); j++)
      if (i + j == index) {
        polygones_i = i;
        polygones_j = j;
      }
  if (polygones_i == -1 || polygones_j == -1)
    return;
  polygone_remove_point(polygones[polygones_i], polygones_j);
  drawer.redraw(polygones);
}

void request::add_hole(polygones_t &polygones) {
  if (polygones.size() == 0)
    return;

  polygones.pop_back();
  polygones.back().index_close = polygones.back().edges.size();
  polygones.back().closed = false;
  is_new_point = true;
}

void request::close_polygon(polygones_t &polygones) {
  if (polygones.size() == 0)
    return;
  if (polygones.back().edges.empty())
    return;
  polygones.back().closed = true;
  if (points_is_equal(
          polygones.back().edges[polygones.back().edges.size() - 1].p2,
          polygones.back().edges[0].p1)) {
    polygones.push_back(polygone_t{});
    return;
  }
  add_point(polygones, polygones.back().edges[polygones.back().index_close].p1);
  polygones.push_back(polygone_t{});
}

void request::fill(polygones_t &polygones, const point_t &seed_p,
                   const color_t &color, bool is_sleep, int sleep_time,
                   std::vector<double> &time) {
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::microseconds;
  polygones[polygones.size() - 2].color = color;
  polygones[polygones.size() - 2].seed = seed_p;
  polygones[polygones.size() - 2].filled = false;
  for (size_t i = 0; i < polygones.size(); i++) {
    auto cur_time = high_resolution_clock::now();
    drawer.seed_method(polygones[i], polygones[i].seed,
                       polygones[i].edges.back().p1.color, polygones[i].color,
                       is_sleep, sleep_time);
    time.push_back((double)duration_cast<microseconds>(
                       high_resolution_clock::now() - cur_time)
                       .count());
  }
}
