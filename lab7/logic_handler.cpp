#include "logic_handler.h"

#include <iostream>
#include <math.h>

LogicHandler::LogicHandler(const QColor &seg_col, const QColor &vis_col,
                           const QColor &cut_col)
    : handler_state(FIRST_RECT_POINT), seg_col(seg_col), vis_col(vis_col),
      cut_col(cut_col), rect(Rectangle(cut_col)) {}

void LogicHandler::next_state() {
  if (handler_state == FIRST_RECT_POINT)
    handler_state = LAST_RECT_POINT;
  else if (handler_state == LAST_RECT_POINT)
    handler_state = FIRST_RECT_POINT;
  else if (handler_state == FIRST_LINE_POINT)
    handler_state = LAST_LINE_POINT;
  else if (handler_state == LAST_LINE_POINT)
    handler_state = FIRST_LINE_POINT;
}

void LogicHandler::add_point(int x, int y, bool is_horizontal,
                             bool is_vertical) {
  states_push(states, state_t{rect, lines, handler_state});
  approx_point(x, y, is_horizontal, is_vertical);
  if (handler_state == FIRST_RECT_POINT) {
    rect = Rectangle(cut_col, x, y);
  } else if (handler_state == LAST_RECT_POINT) {
    rect.set_p2(Point(x, y));
  } else if (handler_state == FIRST_LINE_POINT) {
    lines.push_back(Line(seg_col, x, y));
  } else if (handler_state == LAST_LINE_POINT) {
    lines[lines.size() - 1].set_p2(Point(x, y));
  }
  next_state();
}

void LogicHandler::render_point(int x, int y, bool is_horizontal,
                                bool is_vertical) {
  approx_point(x, y, is_horizontal, is_vertical);
  if (handler_state == LAST_RECT_POINT) {
    rect.set_p2(Point(x, y));
  } else if (handler_state == LAST_LINE_POINT) {
    lines[lines.size() - 1].set_p2(Point(x, y));
  }
}

void LogicHandler::approx_point(int &x, int &y, bool is_horizontal,
                                bool is_vertical) {
  if (handler_state == LAST_LINE_POINT) {
    if (is_horizontal)
      y = lines[lines.size() - 1].get_p1().get_y();
    if (is_vertical)
      x = lines[lines.size() - 1].get_p1().get_x();
  }

  int min_x = rect.get_left();
  int max_x = rect.get_right();
  int min_y = rect.get_bottom();
  int max_y = rect.get_top();

  int eps = 4;
  if (abs(rect.get_p1().get_x() - x) < eps && y >= min_y && y <= max_y)
    x = rect.get_p1().get_x();
  if (abs(rect.get_p2().get_x() - x) < eps && y >= min_y && y <= max_y)
    x = rect.get_p2().get_x();
  if (abs(rect.get_p1().get_y() - y) < eps && x >= min_x && x <= max_x)
    y = rect.get_p1().get_y();
  if (abs(rect.get_p2().get_y() - y) < eps && x >= min_x && x <= max_x)
    y = rect.get_p2().get_y();
}

Point LogicHandler::findR(const Point &p1, const Point &p2, const Point &Q) {
  double m = pow(10, 30);
  if (p1.get_x() != p2.get_x()) {
    m = static_cast<double>(p2.get_y() - p1.get_y()) /
        (p2.get_x() - p1.get_x());
    if (Q.get_x() <= rect.get_left()) {
      int y = static_cast<int>(round(m * (rect.get_left() - Q.get_x()))) +
              Q.get_y();
      if (y >= rect.get_bottom() && y <= rect.get_top()) {
        return Point(rect.get_left(), y);
      }
    }
    if (Q.get_x() >= rect.get_right()) {
      int y = static_cast<int>(round(m * (rect.get_right() - Q.get_x()))) +
              Q.get_y();
      if (y >= rect.get_bottom() && y <= rect.get_top()) {
        return Point(rect.get_right(), y);
      }
    }
  }
  if (qFuzzyCompare(m, 0)) {
    flag = false;
    return Q;
  }
  if (Q.get_y() >= rect.get_top()) {
    int x =
        static_cast<int>(round((rect.get_top() - Q.get_y()) / m)) + Q.get_x();
    if (x >= rect.get_left() && x <= rect.get_right()) {
      return Point(x, rect.get_top());
    }
  }
  if (Q.get_y() <= rect.get_bottom()) {
    int x = static_cast<int>(round((rect.get_bottom() - Q.get_y()) / m)) +
            Q.get_x();
    if (x >= rect.get_left() && x <= rect.get_right()) {
      return Point(x, rect.get_bottom());
    }
  }
  flag = false;
  return Q;
}

void LogicHandler::clear() {
  states_push(states, state_t{rect, lines, handler_state});
  rect = Rectangle(cut_col);
  handler_state = FIRST_RECT_POINT;
  lines.clear();
}

void LogicHandler::new_clip() { handler_state = FIRST_RECT_POINT; }

void LogicHandler::new_seg() { handler_state = FIRST_LINE_POINT; }

bool LogicHandler::undo() {
  if (states.empty())
    return false;
  state_t state = states_pop(states);
  rect = state.rect;
  lines = state.lines;
  handler_state = (handler_state_t)state.handler_state;
  if (states.empty())
    return false;
  return true;
}

void LogicHandler::draw(QPainter &painter) {
  rect.paint(painter);
  for (auto _line : lines)
    _line.paint(painter);
  for (Line _line : vis_lines)
    _line.paint(painter);
  vis_lines.clear();
}

void LogicHandler::set_cut_col(const QColor &color) {
  cut_col = color;
  rect.set_fg(color);
}

void LogicHandler::set_vis_col(const QColor &color) { vis_col = color; }

void LogicHandler::set_seg_col(const QColor &color) {
  seg_col = color;
  for (Line &_line : lines)
    _line.set_fg(color);
}

void LogicHandler::cut() {
  Line vis_line(vis_col);
  for (Line _line : lines) {
    bool is_vis = cut_line(vis_line, _line);
    if (is_vis)
      vis_lines.push_back(vis_line);
  }
}

bool LogicHandler::log_mult(const point_code &code1, const point_code &code2) {
  bool mlt = 0;
  for (size_t i = 0; i < 4; i++)
    mlt += code1[i] && code2[i];

  return mlt;
}

void LogicHandler::find_point_code(int &sum, point_code &t, const Point &p) {
  int min_x = rect.get_left();
  int max_x = rect.get_right();
  int min_y = rect.get_bottom();
  int max_y = rect.get_top();

  for (size_t i = 0; i < 4; i++)
    t[i] = false;

  if ((p.get_x()) < min_x)
    t[0] = true;
  else if ((p.get_x()) > max_x)
    t[1] = true;
  if ((p.get_y()) < min_y)
    t[2] = true;
  else if ((p.get_y()) > max_y)
    t[3] = true;

  sum = 0;
  for (size_t i = 0; i < 4; i++)
    sum += t[i];
}

bool LogicHandler::cut_line(Line &res, Line &src) {
  point_code T1, T2;
  int S1, S2;
  find_point_code(S1, T1, src.get_p1());
  find_point_code(S2, T2, src.get_p2());
  Point R1(0, 0), R2(0, 0);
  flag = true;

  if (S1 == 0 && S2 == 0) {
    R1 = src.get_p1();
    R2 = src.get_p2();
  } else {
    int p = log_mult(T1, T2);
    if (p != 0) //невидимый
      flag = false;
    else {
      int i = 0;
      Point Q(0, 0);
      if (S1 == 0) {
        R1 = src.get_p1();
        Q = src.get_p2();
        i = 1;
      } else if (S2 == 0) {
        R1 = src.get_p2();
        Q = src.get_p1();
        i = 1;
      }
      while (i < 2) {
        i++;
        if (i == 1) {
          Q = src.get_p1();
          R1 = findR(src.get_p1(), src.get_p2(), Q);
          Q = src.get_p2();
        } else
          R2 = findR(src.get_p1(), src.get_p2(), Q);
      }
    }
  }
  if (flag) {
    res.set_p1(R1);
    res.set_p2(R2);
    return true;
  }
  return false;
}
