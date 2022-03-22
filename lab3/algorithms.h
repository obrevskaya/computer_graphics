#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "drawing.h"

size_t dda(const line_t& line, const color_t& color,
    drawer_t& drawer, bool is_draw, bool is_count_steps);

size_t bresenham_int(const line_t& line, const color_t& color,
    drawer_t& drawer, bool is_draw, bool is_count_steps);

size_t bresenham_float(const line_t& line, const color_t& color,
    drawer_t& drawer, bool is_draw, bool is_count_steps);

size_t bresenham_antialised(const line_t& line, const color_t& color,
    drawer_t& drawer, bool is_draw, bool is_count_steps);

size_t wu(const line_t& line, const color_t& color,
    drawer_t& drawer, bool is_draw, bool is_count_steps);

#endif // ALGORITHMS_H
