#include "tasks.h"
#include "algorithms.h"
#include <cmath>

request::request(drawer_t& drawer, const params_t& params)
    : params(params)
    , drawer(drawer)
{
}

int request::handle(const color_t& color, bool is_draw)
{
    switch (params.command) {
    case DRAW_LINE:
        draw_line(color, is_draw);
        return EXIT_SUCCESS;
    case DRAW_LINE_WITH_COUNT_STEPS:
        return draw_line(color, is_draw, true);
    case DRAW_SPECTRUM:
        draw_spectrum(color, is_draw);
        return EXIT_SUCCESS;
    case MEASURE_TIME:
        measure_avg_times(is_draw);
        break;
    case CLEAR_SCREEN:
        drawer.clear();
        break;
    default:
        break;
    }
    return EXIT_SUCCESS;
}

int request::draw_line(const color_t& color, bool is_draw, bool is_count_steps)
{
    size_t (*method)(const line_t&, const color_t&, drawer_t&, bool,
        bool);

    switch (params.method) {
    case DDA:
        method = dda;
        break;
    case BRES_INT:
        method = bresenham_int;
        break;
    case BRES_FLOAT:
        method = bresenham_float;
        break;
    case BRES_ANIT:
        method = bresenham_antialised;
        break;
    case WU:
        method = wu;
        break;
    case LIBRARY:
        drawer.draw_line(params.line.p1, params.line.p2, color);
        return EXIT_SUCCESS;
    default:
        return EXIT_FAILURE;
    }

    return method(params.line, color, drawer, is_draw, is_count_steps);
}

inline double to_radians(double angle)
{
    return angle * M_PI / 180;
}

void request::draw_spectrum(const color_t& color, bool is_draw)
{
    point_t center = params.spectrum.center;
    double r = params.spectrum.r;

    size_t (*method)(const line_t&, const color_t&, drawer_t&, bool,
        bool);

    switch (params.method) {
    case DDA:
        method = dda;
        break;
    case BRES_INT:
        method = bresenham_int;
        break;
    case BRES_FLOAT:
        method = bresenham_float;
        break;
    case BRES_ANIT:
        method = bresenham_antialised;
        break;
    case WU:
        method = wu;
        break;
    case LIBRARY:
        for (double i = 0.0; i <= 360.0; i += params.spectrum.step)
            drawer.draw_line(
                center,
                { center.x + r * cos(to_radians(i)), center.y + r * sin(to_radians(i)) },
                color);
        return;
    default:
        return;
    }

    for (double i = 0.0; i < 360.0; i += params.spectrum.step) {
        method({ center,
                   { center.x + r * cos(to_radians(i)), center.y + r * sin(to_radians(i)) } },
            color, drawer, is_draw, false);
    }
}

double request::measure_avg_time(bool is_draw, size_t (*method)(const line_t&, const color_t&, drawer_t&, bool, bool))
{
    const int iterations = 1000;
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;

    auto end = high_resolution_clock::now();
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; i++)
        for (double j = 0; j < 360.0; j += params.r_spectrum.spectrum.step) {
            method({ {},
                       { params.r_spectrum.spectrum.r * cos(to_radians(j)),
                           params.r_spectrum.spectrum.r * sin(to_radians(j)) } },
                {}, drawer, is_draw, false);
        }
    end = high_resolution_clock::now();
    return (double)duration_cast<microseconds>(end - start).count() / iterations;
}

void request::measure_avg_times(bool is_draw)
{
    params.r_spectrum.time->push_back(measure_avg_time(is_draw, dda));
    params.r_spectrum.time->push_back(measure_avg_time(is_draw, bresenham_int));
    params.r_spectrum.time->push_back(measure_avg_time(is_draw, bresenham_float));
    params.r_spectrum.time->push_back(measure_avg_time(is_draw, bresenham_antialised));
    params.r_spectrum.time->push_back(measure_avg_time(is_draw, wu));

    if (is_draw)
        drawer.clear();
}
