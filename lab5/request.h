#ifndef REQUEST_H
#define REQUEST_H

#include "drawer.h"
#include "polygones.h"
#include <stack>

enum {
    SET_POLYGONES,
    ADD_POINT,
    CHANGE_POINT,
    REMOVE_POINT,
    CLOSE_POLYGON,
    ADD_HOLE,
    FILL,
    GET_POLYGONES,
    CLEAR_SCREEN,
};

struct sleep_data_t {
    bool is_sleep;
    int sleep_time;
};

struct params_t {
    int command;
    color_t color;
    std::vector<double>& time;
    point_t point;
    polygones_t polygones;
    union {
        size_t index;
        sleep_data_t sleep_data;
    };
};

class request {
    const params_t& params;
    drawer_t& drawer;

protected:
    void add_point(polygones_t& polygones, const point_t& point);
    void change_point(polygones_t& polygones, size_t index, point_t point);
    void remove_point(polygones_t& polygones, size_t index);
    void add_hole(polygones_t& polygones);
    void close_polygon(polygones_t& polygones);
    void fill(polygones_t& polygones, const color_t& color, bool is_sleep, int sleep_time, std::vector<double>& time);

public:
    request(drawer_t& drawer, const params_t& params);
    polygones_t handle();
};

#endif // REQUEST_H
