#include "color_view.h"
#include <QBrush>
#include <QPainter>

ColorView::ColorView(QWidget* parent)
    : QWidget(parent)
{
}

void ColorView::set_color(const QColor& color)
{
    w_color = color;

    update();
}

QColor ColorView::get_color()
{
    return w_color;
}

void ColorView::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    painter.setPen(QPen(QColor(0, 0, 0)));
    painter.setBrush(QBrush(this->w_color));

    painter.drawRect(0, 0, this->width(), this->height());
}
