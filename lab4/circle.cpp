#include "circle.h"


Circle::Circle(double _r, QPointF _center, method_t _method = STANDART, const QColor &_color = Qt::black)
    : r(_r), center(_center),
      maxX(_center.x() + _r), minX(_center.x() - _r),
      maxY(_center.y() + _r), minY(_center.y() - _r),
      method(_method),
      color(_color)
{
}
