#ifndef LOGICHANDLER_H
#define LOGICHANDLER_H

#include <QPainter>
#include <vector>

#include "figures.h"
#include "states.h"

class LogicHandler {
    enum handler_state_t {
        FIRST_RECT_POINT,
        LAST_RECT_POINT,
        FIRST_LINE_POINT,
        LAST_LINE_POINT,
    };

public:
    LogicHandler(const QColor& seg_col, const QColor& vis_col, const QColor& cut_col);
    void clear();
    void new_clip();
    void new_seg();
    bool undo();
    void draw(QPainter& painter);
    void cut();
    void add_point(int x, int y, bool is_horizontal, bool is_vertical);
    void render_point(int x, int y, bool is_horizontal, bool is_vertical);
    void set_seg_col(const QColor& color);
    void set_vis_col(const QColor& color);
    void set_cut_col(const QColor& color);

private:
    typedef bool point_code[4];
    void next_state();
    void approx_point(int& x, int& y, bool is_horizontal, bool is_vertical);
    Point findR(const Point& p1, const Point& p2, const Point& Q);
    bool cut_line(Line& res, Line& src);
    void find_point_code(int& sum, point_code& code, const Point& p);
    bool log_mult(const point_code& code1, const point_code& code2);

    handler_state_t handler_state;

    QColor seg_col;
    QColor vis_col;
    QColor cut_col;

    bool flag;

    Rectangle rect;
    std::vector<Line> lines;
    std::vector<Line> vis_lines;
    std::stack<state_t> states;
};

#endif // LOGICHANDLER_H
