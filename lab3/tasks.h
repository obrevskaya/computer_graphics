#ifndef TASKS_H
#define TASKS_H

#include "drawing.h"
#include "structures.h"
#include <stack>

enum {
    DRAW_LINE,
    DRAW_LINE_WITH_COUNT_STEPS,
    DRAW_SPECTRUM,
    MEASURE_TIME,
    CLEAR_SCREEN
};

enum method_t { DDA,
    BRES_INT,
    BRES_FLOAT,
    BRES_ANIT,
    WU,
    LIBRARY };

struct params_t {
    int command;
    int method = DDA;
    union {
        line_t line;
        spectrum_t spectrum;
        research_spectrum_t r_spectrum;
    };
};

class request {
    const params_t& params;
    drawer_t& drawer;

protected:
    int draw_line(const color_t& color, bool is_draw = true, bool is_count_steps = false);

    void draw_spectrum(const color_t& color, bool is_draw = true);

    double measure_avg_time(bool is_draw, size_t (*method)(const line_t&, const color_t&, drawer_t&, bool, bool));

    void measure_avg_times(bool is_draw = false);

public:
    request(drawer_t& drawer, const params_t& params);

    int handle(const color_t& color, bool is_draw = true);
};

#endif // TASKS_H
