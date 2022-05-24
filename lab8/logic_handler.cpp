#include "logic_handler.h"

#include <iostream>
#include <math.h>

LogicHandler::LogicHandler(const QColor& seg_col, const QColor& vis_col, const QColor& cut_col)
    : handler_state(START_CUTTER)
    , seg_col(seg_col)
    , vis_col(vis_col)
    , cut_col(cut_col)
    , cutter(cut_col)
{
}

void LogicHandler::next_state()
{
    if (handler_state == START_CUTTER)
        handler_state = PROC_CUTTER;
    else if (handler_state == PROC_CUTTER)
        handler_state = START_CUTTER;
    else if (handler_state == FIRST_LINE_POINT)
        handler_state = LAST_LINE_POINT;
    else if (handler_state == LAST_LINE_POINT)
        handler_state = FIRST_LINE_POINT;
}

void LogicHandler::cutter_close()
{
    if (handler_state == PROC_CUTTER)
        next_state();
    cutter.close();
}

void LogicHandler::add_point(int x, int y, bool is_horizontal, bool is_vertical, bool is_paral)
{
    states_push(states, state_t { cutter, lines, handler_state });
    approx_point(x, y, is_horizontal, is_vertical, is_paral);
    if (handler_state == START_CUTTER) {
        cutter = Cutter(cut_col);
        cutter.add_point(Point(x, y));
    } else if (handler_state == PROC_CUTTER) {
        if (is_render)
            cutter.add_point(Point(x, y));
        else
            cutter.change_last(Point(x, y));
        is_render = true;
    } else if (handler_state == FIRST_LINE_POINT) {
        lines.push_back(Line(seg_col, x, y));
    } else if (handler_state == LAST_LINE_POINT) {
        lines[lines.size() - 1].set_p2(Point(x, y));
    }
    if (handler_state != PROC_CUTTER)
        next_state();
}

void LogicHandler::render_point(int x, int y, bool is_horizontal, bool is_vertical, bool is_paral)
{
    approx_point(x, y, is_horizontal, is_vertical, is_paral);
    if (handler_state == PROC_CUTTER) {
        if (is_render) {
            cutter.add_point(Point(x, y));
            is_render = false;
        } else
            cutter.change_last(Point(x, y));
    } else if (handler_state == LAST_LINE_POINT) {
        lines[lines.size() - 1].set_p2(Point(x, y));
    }
}

void LogicHandler::approx_point(int& x, int& y, bool is_horizontal, bool is_vertical, bool is_paral)
{
    if (handler_state == START_CUTTER)
        return;

    if (handler_state == PROC_CUTTER) {
        if (is_horizontal) {
            y = cutter.get_last().get_y();
            return;
        }
        if (is_vertical) {
            x = cutter.get_last().get_x();
            return;
        }
        return;
    }

    if (handler_state == LAST_LINE_POINT) {
        if (is_horizontal) {
            y = lines[lines.size() - 1].get_p1().get_y();
            return;
        }
        if (is_vertical) {
            x = lines[lines.size() - 1].get_p1().get_x();
            return;
        }
        if (is_paral) {
            double angle = atan((lines[lines.size() - 1].get_p1().get_y() - y) / (lines[lines.size() - 1].get_p1().get_x() - x));
            double min_dif = 0, rib_angle = 0;
            int min_ind = 0;
            for (size_t i = 0; i < cutter.edges_count(); i++) {
                rib_angle = atan((cutter.get_edge(i).get_p1().get_y()
                                     - cutter.get_edge(i).get_p2().get_y())
                    / (cutter.get_edge(i).get_p1().get_x() - cutter.get_edge(i).get_p2().get_x()));
                if (i == 0 || fabs(fabs(angle - rib_angle) - M_PI_2) > min_dif) {
                    min_dif = fabs(fabs(angle - rib_angle) - M_PI_2);
                    min_ind = i;
                }
            }
            rib_angle = atan((cutter.get_edge(min_ind).get_p1().get_y()
                                 - cutter.get_edge(min_ind).get_p2().get_y())
                / (cutter.get_edge(min_ind).get_p1().get_x() - cutter.get_edge(min_ind).get_p2().get_x()));
            min_dif = angle - rib_angle;
            double dist = sqrt(pow((lines[lines.size() - 1].get_p1().get_x() - x), 2) + pow((lines[lines.size() - 1].get_p1().get_y() - y), 2));
            dist *= cos(min_dif);
            if (x < lines[lines.size() - 1].get_p1().get_x())
                dist *= -1;
            x = lines[lines.size() - 1].get_p1().get_x() + dist * cos(rib_angle);
            y = lines[lines.size() - 1].get_p1().get_y() + dist * sin(rib_angle);

            Point n_in = Point(cutter.get_edge(min_ind).get_p2().get_y() - cutter.get_edge(min_ind).get_p1().get_y(),
                -(cutter.get_edge(min_ind).get_p2().get_x() - cutter.get_edge(min_ind).get_p1().get_x()));
            if (rib_angle)
                x -= (x - lines[lines.size() - 1].get_p1().get_x() + (y - lines[lines.size() - 1].get_p1().get_y()) * n_in.get_y() / n_in.get_x());

            return;
        }
    }

    int dist = 0;
    Point min_dist;
    for (size_t i = 0; i < cutter.edges_count(); i++) {
        Point proj = cutter.get_edge(i).distance(Point(x, y));
        if (i == 0 || (pow(proj.get_x() - x, 2) + pow(proj.get_y() - y, 2)) < dist) {
            dist = pow(proj.get_x() - x, 2) + pow(proj.get_y() - y, 2);
            min_dist = proj;
        }
    }
    if (dist < 36) {
        x = min_dist.get_x();
        y = min_dist.get_y();
    }
}

void LogicHandler::clear()
{
    states_push(states, state_t { cutter, lines, handler_state });
    cutter = Cutter(cut_col);
    handler_state = START_CUTTER;
    lines.clear();
}

void LogicHandler::new_clip()
{
    handler_state = START_CUTTER;
}

void LogicHandler::new_seg()
{
    handler_state = FIRST_LINE_POINT;
}

bool LogicHandler::undo()
{
    if (states.empty())
        return false;
    state_t state = states_pop(states);
    cutter = state.cutter;
    lines = state.lines;
    handler_state = (handler_state_t)state.handler_state;
    if (states.empty())
        return false;
    return true;
}

void LogicHandler::draw(QPainter& painter)
{
    cutter.paint(painter);
    for (auto _line : lines)
        _line.paint(painter);
    for (Line _line : vis_lines)
        _line.paint(painter);
    vis_lines.clear();
}

void LogicHandler::set_cut_col(const QColor& color)
{
    cut_col = color;
    cutter.set_fg(color);
}

void LogicHandler::set_vis_col(const QColor& color)
{
    vis_col = color;
}

void LogicHandler::set_seg_col(const QColor& color)
{
    seg_col = color;
    for (Line& _line : lines)
        _line.set_fg(color);
}

bool LogicHandler::cut()
{
    Line vis_line(vis_col);
    int conv = cutter.is_convex();
    if (!conv)
        return false;
    for (Line _line : lines) {
        bool is_vis = cut_line(vis_line, _line, conv);
        if (is_vis)
            vis_lines.push_back(vis_line);
    }
    return true;
}

int LogicHandler::scalar_prod(Point p1, Point p2)
{
    return p1.get_x() * p2.get_x() + p1.get_y() * p2.get_y();
}

bool LogicHandler::cut_line(Line& res, Line& src, int conv)
{
    int m = cutter.edges_count();
    Point D = src.get_p2() - src.get_p1();
    double t_low = 0, t_high = 1;
    for (int i = 0; i < m; i++) {
        Point f = cutter.get_edge(i).get_p1(), p2 = cutter.get_edge(i).get_p2();
        Point n_in = Point(-conv * (p2.get_y() - f.get_y()), conv * (p2.get_x() - f.get_x()));
        Point W = src.get_p1() - f;
        double Wsc = scalar_prod(W, n_in);
        double Dsc = scalar_prod(D, n_in);

        if (Dsc) {
            double t = -Wsc / Dsc;
            if (Dsc > 0) {
                if (t > 1)
                    return false;
                else if (t > t_low) {
                    t_low = t;
                }
            } else {
                if (t < 0)
                    return false;
                else if (t < t_high) {
                    t_high = t;
                }
            }
        } else if (Wsc < 0)
            return false;
    }
    if (t_low > t_high)
        return false;
    res.set_p1(src.get_p1() + (src.get_p2() - src.get_p1()) * t_low);
    res.set_p2(src.get_p1() + (src.get_p2() - src.get_p1()) * t_high);
    return true;
}
