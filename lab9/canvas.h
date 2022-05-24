#ifndef CANVAS_H
#define CANVAS_H

#include "logic_handler.h"
#include <QColor>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <vector>

class Canvas : public QWidget {
    Q_OBJECT
public:
    explicit Canvas(QWidget* parent = nullptr);
    ~Canvas();
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    void add_point(int x, int y);
    void clear();
    void new_clip();
    void new_seg();
    bool undo();
    void set_bg(const QColor& color);
    void set_seg_col(const QColor& color);
    void set_vis_col(const QColor& color);
    void set_cut_col(const QColor& color);
    bool cut();
    void close();

private:
    QColor bg = QColor(255, 255, 255);
    LogicHandler logic_handler = LogicHandler(QColor(0, 0, 0), QColor(0, 0, 0), QColor(0, 0, 0));
    bool process = false;
    bool is_clear = false;
signals:
};

#endif // CANVAS_H
