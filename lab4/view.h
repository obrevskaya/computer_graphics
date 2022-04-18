#ifndef VIEW_H
#define VIEW_H

#include <QColor>
#include <QGraphicsView>
#include <QPointF>

#include "circles.h"
#include "ellipses.h"

class View {
  QGraphicsView *view;
  QGraphicsScene *scene;
  QPainter painter;
  int minX, maxX;
  int minY, maxY;
  int xRange, yRange;
  bool hasData = false;

public:
  View();
  ~View();
  void setView(QGraphicsView *view);
  void setBgColor(QColor &color);
  void draw(bool x, const Circles &circles, const Ellipses &ellipses,
            QColor bgColor, bool draw = true);

private:
  QSize imgSize(const Circles &circles, const Ellipses &ellipses);
  QPointF transformPoint(const QPointF &point);
  QLineF transformLine(const QLineF &line);
  Circle transformCircle(const Circle &circle);
  Ellipse transformEllipse(const Ellipse &ellipse);
  void drawAxis(QPainter *p);
  QSize imgSize();

  void drawCircle(QPainter *p, const Circle &circle, bool draw = true);
  void standartDrawCircle(QPainter *p, const Circle &circle, bool draw = true);
  void drawPoint(QPainter *p, const QPointF &point, const QColor &color,
                 bool draw = true);
  void drawSym(QPainter *p, const QPointF &center, double x, double y,
               const QColor &color, bool draw = true);
  void canonicalDrawCircle(QPainter *p, const Circle &circle, bool draw);
  void parametricDrawCircle(QPainter *p, const Circle &circle, bool draw);
  void bresenhamDrawCircle(QPainter *p, const Circle &circle, bool draw);
  void midPointDrawCircle(QPainter *p, const Circle &circle, bool draw);

  void standartDrawEllipse(QPainter *p, const Ellipse &ellipse, bool draw);
  void drawEllipse(QPainter *p, const Ellipse &ellipse, bool draw);
  void canonicalDrawEllipse(QPainter *p, const Ellipse &ellipse, bool draw);
  void parametricDrawEllipse(QPainter *p, const Ellipse &ellipse, bool draw);
  void bresenhamDrawEllipse(QPainter *p, const Ellipse &ellipse, bool draw);
  void midPointDrawEllipse(QPainter *p, const Ellipse &ellipse, bool draw);
};

#endif // VIEW_H
