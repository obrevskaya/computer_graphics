#ifndef LOGICHANDLER_H
#define LOGICHANDLER_H

#include <QPainter>
#include <vector>

#include "figures.h"
#include "states.h"

class LogicHandler {
    enum handler_state_t {
        START_CUTTER,
        PROC_CUTTER,
        FIRST_LINE_POINT,
        LAST_LINE_POINT,
    };

public:
    LogicHandler(const QColor& seg_col, const QColor& vis_col, const QColor& cut_col);
    int get_handler_state() const { return handler_state; }
    void clear();
    void new_clip();
    void new_seg();
    bool undo();
    void draw(QPainter& painter);
    bool cut();
    void add_point(int x, int y, bool is_horizontal, bool is_vertical, bool is_paral);
    void render_point(int x, int y, bool is_horizontal, bool is_vertical, bool is_paral);
    void set_seg_col(const QColor& color);
    void set_vis_col(const QColor& color);
    void set_cut_col(const QColor& color);
    void cutter_close();

private:
    typedef bool point_code[4];
    void next_state();
    void approx_point(int& x, int& y, bool is_horizontal, bool is_vertical, bool is_paral);
    bool cut_line(Line& res, Line& src, int conv);
    int scalar_prod(Point p1, Point p2);

    handler_state_t handler_state;

    QColor seg_col;
    QColor vis_col;
    QColor cut_col;

    bool is_render = true;

    Cutter cutter;
    std::vector<Line> lines;
    std::vector<Line> vis_lines;
    std::stack<state_t> states;
};

#endif // LOGICHANDLER_H
