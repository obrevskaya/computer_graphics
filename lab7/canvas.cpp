#include "canvas.h"
#include <QApplication>
#include <QBrush>
#include <QPen>
#include <QPointF>

Canvas::Canvas(QWidget* parent)
    : QWidget { parent }
{
    setMouseTracking(true);
}

Canvas::~Canvas()
{
}

void Canvas::cut()
{
    logic_handler.cut();
    this->update();
}

void Canvas::set_bg(const QColor& color)
{
    bg = color;
}

void Canvas::set_seg_col(const QColor& color)
{
    logic_handler.set_seg_col(color);
    this->update();
}

void Canvas::set_vis_col(const QColor& color)
{
    logic_handler.set_vis_col(color);
    this->update();
}

void Canvas::set_cut_col(const QColor& color)
{
    logic_handler.set_cut_col(color);
    this->update();
}

void Canvas::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    painter.setBrush(bg);
    painter.setPen(QColor(0, 0, 0));
    painter.drawRect(0, 0, this->width(), this->height());

    logic_handler.draw(painter);
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
    process = not process;
    Qt::KeyboardModifiers key = QApplication::queryKeyboardModifiers();
    bool is_hor = false, is_vert = false;
    if (key == Qt::ShiftModifier)
        is_hor = true;
    if (key == Qt::ControlModifier)
        is_vert = true;
    logic_handler.add_point(event->pos().x(), event->pos().y(), is_hor, is_vert);
    this->update();
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
    if (!process)
        return;

    Qt::KeyboardModifiers key = QApplication::queryKeyboardModifiers();
    bool is_hor = false, is_vert = false;
    if (key == Qt::ShiftModifier)
        is_hor = true;
    if (key == Qt::ControlModifier)
        is_vert = true;
    logic_handler.render_point(event->pos().x(), event->pos().y(), is_hor, is_vert);
    this->update();
}

void Canvas::add_point(int x, int y)
{
    process = not process;
    logic_handler.add_point(x, y, false, false);
    this->update();
}

void Canvas::clear()
{
    is_clear = true;

    logic_handler.clear();

    this->update();
}

void Canvas::new_clip()
{
    logic_handler.new_clip();

    this->update();
}

void Canvas::new_seg()
{
    logic_handler.new_seg();

    this->update();
}

bool Canvas::undo()
{
    if (is_clear)
        is_clear = false;
    else
        process = not process;

    bool is_not_empty = logic_handler.undo();

    if (not is_not_empty)
        return false;

    this->update();

    return true;
}
