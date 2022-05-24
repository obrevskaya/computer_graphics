#include "logic_handler.h"

#include <iostream>
#include <math.h>

LogicHandler::LogicHandler(const QColor& seg_col, const QColor& vis_col, const QColor& cut_col)
    : handler_state(START_CUTTER)
    , seg_col(seg_col)
    , vis_col(vis_col)
    , cut_col(cut_col)
    , cutter(cut_col)
    , enter(seg_col)
    , vis_fig(vis_col)
{
}

void LogicHandler::next_state()
{
    if (handler_state == START_CUTTER)
        handler_state = PROC_CUTTER;
    else if (handler_state == PROC_CUTTER)
        handler_state = START_CUTTER;
    else if (handler_state == START_ENTER)
        handler_state = PROC_ENTER;
    else if (handler_state == PROC_ENTER)
        handler_state = START_ENTER;
}

void LogicHandler::cutter_close()
{
    if (handler_state == PROC_CUTTER)
        next_state();
    cutter.close_with_pop();
}

void LogicHandler::enter_close()
{
    if (handler_state == PROC_ENTER)
        next_state();
    enter.close_with_pop();
}

void LogicHandler::add_point(int x, int y, bool is_horizontal, bool is_vertical, bool is_paral)
{
    states_push(states, state_t { cutter, enter, handler_state });
    approx_point(x, y, is_horizontal, is_vertical, is_paral);
    if (handler_state == START_CUTTER) {
        cutter = Polygon(cut_col);
        cutter.add_point(Point(x, y));
        is_render = true;
    } else if (handler_state == PROC_CUTTER) {
        if (is_render)
            cutter.add_point(Point(x, y));
        else
            cutter.change_last(Point(x, y));
        is_render = true;
    } else if (handler_state == START_ENTER) {
        enter = Polygon(seg_col);
        enter.add_point(Point(x, y));
        vis_fig = Polygon(cut_col);
        is_render = true;
    } else if (handler_state == PROC_ENTER) {
        if (is_render)
            enter.add_point(Point(x, y));
        else
            enter.change_last(Point(x, y));
        is_render = true;
    }
    if (handler_state != PROC_CUTTER && handler_state != PROC_ENTER)
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
    } else if (handler_state == PROC_ENTER) {
        if (is_render) {
            enter.add_point(Point(x, y));
            is_render = false;
        } else
            enter.change_last(Point(x, y));
    }
}

void LogicHandler::approx_point(int& x, int& y, bool is_horizontal, bool is_vertical, bool is_paral)
{
    if (handler_state == START_CUTTER || handler_state == START_ENTER)
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

    if (handler_state == PROC_ENTER) {
        if (is_horizontal) {
            y = enter.get_last().get_y();
            return;
        }
        if (is_vertical) {
            x = enter.get_last().get_x();
            return;
        }
        if (is_paral) {
            double angle = atan((enter.get_last().get_y() - y) / (enter.get_last().get_x() - x));
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
            double dist = sqrt(pow((enter.get_last().get_x() - x), 2) + pow((enter.get_last().get_y() - y), 2));
            dist *= cos(min_dif);
            if (x < enter.get_last().get_x())
                dist *= -1;
            x = enter.get_last().get_x() + dist * cos(rib_angle);
            y = enter.get_last().get_y() + dist * sin(rib_angle);

            Point n_in = Point(cutter.get_edge(min_ind).get_p2().get_y() - cutter.get_edge(min_ind).get_p1().get_y(),
                -(cutter.get_edge(min_ind).get_p2().get_x() - cutter.get_edge(min_ind).get_p1().get_x()));
            if (rib_angle)
                x -= (x - enter.get_last().get_x() + (y - enter.get_last().get_y()) * n_in.get_y() / n_in.get_x());
            return;
        }
    }

    if (cutter.edges_count() == 0)
        return;
    int dist = 0;
    Point min_dist;
    for (size_t i = 0; i < cutter.edges_count(); i++) {
        Point proj = cutter.get_edge(i).find_distance(Point(x, y));
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
    states_push(states, state_t { cutter, enter, handler_state });
    cutter = Polygon(cut_col);
    enter = Polygon(cut_col);
    vis_fig = Polygon(cut_col);
    handler_state = START_CUTTER;
}

void LogicHandler::new_clip()
{
    handler_state = START_CUTTER;
}

void LogicHandler::new_seg()
{
    handler_state = START_ENTER;
}

bool LogicHandler::undo()
{
    if (states.empty())
        return false;
    state_t state = states_pop(states);
    cutter = state.cutter;
    enter = state.enter;
    handler_state = (handler_state_t)state.handler_state;
    if (states.empty())
        return false;
    return true;
}

void LogicHandler::draw(QPainter& painter)
{
    cutter.paint(painter);
    enter.paint(painter);
    vis_fig.paint(painter);
    vis_fig = Polygon(Qt::white);
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
    enter.set_fg(color);
}

bool LogicHandler::cut()
{
    int normal = cutter.is_convex();
    if (!normal)
        return false;

    vis_fig = enter;
    for (auto cut_line : cutter.get_edges()) {
        Polygon new_vis_fig(vis_col);
        for (auto vis_line : vis_fig.get_edges()) {
            Point inter;
            bool is_inter = cut_line.find_intersection(inter, vis_line, normal);

            if (is_inter) {
                new_vis_fig.add_point(inter);
            }

            bool is_vis = cut_line.is_visible(vis_line.get_p2(), normal);
            if (is_vis)
                new_vis_fig.add_point(vis_line.get_p2());
        }

        new_vis_fig.close();
        vis_fig = new_vis_fig;
    }
    return true;
}

int LogicHandler::scalar_prod(Point p1, Point p2) const
{
    return p1.get_x() * p2.get_x() + p1.get_y() * p2.get_y();
}
