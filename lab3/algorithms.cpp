#include "algorithms.h"
#include <cmath>

inline int sign(double x)
{
    if (x < 0)
        return -1;
    if (x > 0)
        return 1;
    return 0;
}

static int pack_255(const int& i)
{
    int res = i > 255 ? 255 : i;
    return res < 0 ? 0 : res;
}

static color_t update(const color_t& color, const double& e)
{
    return { color.r, color.g, color.b, pack_255(int(255 * e)) };
}

size_t dda(const line_t& line, const color_t& color, drawer_t& drawer, bool is_draw, bool is_count_steps)
{
    double dx = line.p2.x - line.p1.x, dy = line.p2.y - line.p1.y;
    double max_d = std::max(std::abs(dx), std::abs(dy));
    double sign_x = dx / max_d, sign_y = dy / max_d;
    double x = line.p1.x, y = line.p1.y;
    double buf_x = x, buf_y = y;

    size_t step = 1;

    for (size_t i = 0; i < max_d; i++) {
        if (is_draw)
            drawer.draw_point(std::round(x), std::round(y), color);

        x += sign_x;
        y += sign_y;

        if (!is_count_steps)
            continue;

        if (std::round(buf_x) != std::round(x) && std::round(buf_y) != std::round(y))
            step++;

        buf_x = x;
        buf_y = y;
    }
    return step;
}

size_t bresenham_int(const line_t& line, const color_t& color, drawer_t& drawer, bool is_draw, bool is_count_steps)
{
    double dx = line.p2.x - line.p1.x, dy = line.p2.y - line.p1.y;
    double sign_x = sign(dx), sign_y = sign(dy);
    dx = std::abs(dx), dy = std::abs(dy);

    bool is_changed = false;
    is_changed = dy >= dx;
    if (is_changed)
        std::swap(dx, dy);

    double m = 2 * dy, m1 = 2 * dx;
    double e = m - dx;
    int x = round(line.p1.x), y = round(line.p1.y);
    int buf_x = x, buf_y = y;

    size_t step = 1;

    for (size_t i = 0; i <= dx; i++) {
        if (is_draw)
            drawer.draw_point(x, y, color);

        if (e >= 0) {
            if (is_changed)
                x += sign_x;
            else
                y += sign_y;
            e -= m1;
        }
        if (e <= 0) {
            if (is_changed)
                y += sign_y;
            else
                x += sign_x;
            e += m;
        }

        if (!is_count_steps)
            continue;

        if (buf_x != x && buf_y != y)
            step++;

        buf_x = x;
        buf_y = y;
    }

    return step;
}

size_t bresenham_float(const line_t& line, const color_t& color, drawer_t& drawer, bool is_draw, bool is_count_steps)
{
    double dx = line.p2.x - line.p1.x, dy = line.p2.y - line.p1.y;
    int sign_x = sign(dx), sign_y = sign(dy);

    dx = std::abs(dx), dy = std::abs(dy);

    bool is_changed = false;
    is_changed = dy >= dx;
    if (is_changed)
        std::swap(dx, dy);

    double m = dy / dx;
    double e = m - 0.5;
    double x = round(line.p1.x), y = round(line.p1.y);
    double buf_x = x, buf_y = y;

    size_t step = 1;

    for (size_t i = 0; i <= dx; i++) {
        if (is_draw)
            drawer.draw_point(x, y, color);

        if (e >= 0) {
            if (is_changed)
                x += sign_x;
            else
                y += sign_y;
            e--;
        }
        if (e <= 0) {
            if (is_changed)
                y += sign_y;
            else
                x += sign_x;
            e += m;
        }

        if (!is_count_steps)
            continue;

        if (buf_x != x && buf_y != y)
            step++;

        buf_x = x;
        buf_y = y;
    }

    return step;
}

size_t bresenham_antialised(const line_t& line, const color_t& color, drawer_t& drawer, bool is_draw, bool is_count_steps)
{
    double dx = line.p2.x - line.p1.x, dy = line.p2.y - line.p1.y;
    int sign_x = sign(dx), sign_y = sign(dy);

    dx = std::abs(dx), dy = std::abs(dy);

    bool is_changed = false;
    is_changed = dy >= dx;
    if (is_changed)
        std::swap(dx, dy);

    double m = dy / dx;
    double e = 0.5;
    color_t color1 = update(color, e);
    double x = round(line.p1.x), y = round(line.p1.y);

    if (is_draw)
        drawer.draw_point(x, y, color1);

    double W = 1 - m;
    double buf_x = x, buf_y = y;
    size_t step = 1;

    for (size_t i = 0; i <= dx; i++) {
        if (e < W) {
            if (!is_changed)
                x += sign_x;
            else
                y += sign_y;
            e += m;
        } else {
            x += sign_x;
            y += sign_y;
            e -= W;
        }

        color1 = update(color, e);
        if (is_draw)
            drawer.draw_point(x, y, color1);

        if (!is_count_steps)
            continue;

        if (buf_x != x && buf_y != y)
            step++;

        buf_x = x;
        buf_y = y;
    }

    return step;
}

size_t wu(const line_t& line, const color_t& color, drawer_t& drawer, bool is_draw, bool is_count_steps)
{
    point_t p1 = line.p1, p2 = line.p2;

    auto fpart = [](double x) -> double { return x - int(x); };
    auto ipart = [=](double x) -> double { return x - fpart(x); };
    auto rfpart = [=](double x) -> double { return 1 - fpart(x); };
    auto round = [=](double x) -> double { return ipart(x + 0.5); };

    color_t color1, color2;

    bool steep = std::abs(p2.y - p1.y) > std::abs(p2.x - p1.x);
    if (steep) {
        std::swap(p1.x, p1.y);
        std::swap(p2.x, p2.y);
    }
    if (p1.x > p2.x) {
        std::swap(p1.x, p2.x);
        std::swap(p1.y, p2.y);
    }

    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double gradient = (dx == 0) ? 1 : dy / dx;

    int xpx11;
    double intery;
    {
        double xend = round(p1.x);
        double yend = p1.y + gradient * (xend - p1.x);
        double xgap = rfpart(p1.x + 0.5);
        xpx11 = int(xend);
        int ypx11 = ipart(yend);

        color1 = update(color, rfpart(yend) * xgap);
        color2 = update(color, fpart(yend) * xgap);
        if (is_draw) {
            if (steep) {
                drawer.draw_point(ypx11, xpx11, color1);
                drawer.draw_point(ypx11 + 1, xpx11, color2);
            } else {
                drawer.draw_point(xpx11, ypx11, color1);
                drawer.draw_point(xpx11, ypx11 + 1, color2);
            }
        }

        intery = yend + gradient;
    }

    int xpx12;
    {
        double xend = std::round(p2.x);
        double yend = p2.y + gradient * (xend - p2.x);
        double xgap = rfpart(p2.x + 0.5);
        xpx12 = int(xend);
        int ypx12 = ipart(yend);

        color1 = update(color, rfpart(yend) * xgap);
        color2 = update(color, fpart(yend) * xgap);
        if (is_draw) {
            if (steep) {
                drawer.draw_point(ypx12, xpx12, color1);
                drawer.draw_point(ypx12 + 1, xpx12, color2);
            } else {
                drawer.draw_point(xpx12, ypx12, color1);
                drawer.draw_point(xpx12, ypx12 + 1, color2);
            }
        }
    }

    size_t step = 0;

    for (int x = xpx11 + 1; x < xpx12; x++) {
        color1 = update(color, rfpart(intery));
        color2 = update(color, fpart(intery));

        if (is_draw) {
            if (steep) {
                drawer.draw_point(ipart(intery), x, color1);
                drawer.draw_point(ipart(intery) + 1, x, color2);
            } else {
                drawer.draw_point(x, ipart(intery), color1);
                drawer.draw_point(x, ipart(intery) + 1, color2);
            }
        }

        if (is_count_steps && x < xpx12) {
            if (ipart(intery) != ipart(intery + gradient))
                ++step;
        }

        intery += gradient;
    }

    return step;
}
