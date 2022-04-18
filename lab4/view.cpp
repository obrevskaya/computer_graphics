#include "view.h"
#include <cmath>

View::View() { scene = new QGraphicsScene(); }

View::~View() { delete scene; }

template <typename T> int sgn1(T val) { return (val > 0) - (val < 0); }

void View::setView(QGraphicsView *view) { this->view = view; }

QPointF View::transformPoint(const QPointF &point) {
  return QPointF(point.x() - minX, yRange - point.y() + minY);
}

QLineF View::transformLine(const QLineF &line) {
  return QLineF(transformPoint(line.p1()), transformPoint(line.p2()));
}

Circle View::transformCircle(const Circle &circle) {
  return Circle(circle.r, transformPoint(circle.center), circle.method,
                circle.color);
}

Ellipse View::transformEllipse(const Ellipse &ellipse) {
  return Ellipse(ellipse.a, ellipse.b, transformPoint(ellipse.center),
                 ellipse.method, ellipse.color);
}

void View::drawAxis(QPainter *p) {
  p->setPen(QColor(0, 0, 0));
  QLineF x = transformLine(QLineF(0, 0, 100, 0));
  QLineF y = transformLine(QLineF(0, 1, 0, 100));
  p->drawLine(x);
  p->drawLine(y);
  QLineF xArrow1 = transformLine(QLineF(99, -1, 97, -3));
  QLineF xArrow2 = transformLine(QLineF(99, 1, 97, 3));
  p->drawLine(xArrow1);
  p->drawLine(xArrow2);
  QLineF yArrow1 = transformLine(QLineF(-1, 99, -3, 97));
  QLineF yArrow2 = transformLine(QLineF(1, 99, 3, 97));
  p->drawLine(yArrow1);
  p->drawLine(yArrow2);
}

QSize View::imgSize(const Circles &circles, const Ellipses &ellipses) {
  minX = std::min(std::min(circles.minX, -5.0), ellipses.minX);
  maxX = std::max(std::max(circles.maxX, 100.0), ellipses.maxX);
  minY = std::min(std::min(circles.minY, -5.0), ellipses.minY);
  maxY = std::max(std::max(circles.maxY, 100.0), ellipses.maxY);
  minX -= 5;
  maxX += 5;
  minY -= 5;
  maxY += 5;
  xRange = maxX - minX;
  yRange = maxY - minY;
  hasData = true;
  return QSize(xRange, yRange);
}

QSize View::imgSize() {
  if (!hasData) {
    minX = -5.0;
    maxX = 100.0;
    minY = -5.0;
    maxY = 100.0;
    xRange = maxX - minX;
    yRange = maxY - minY;
  }

  return QSize(xRange, yRange);
}

void View::draw(bool x, const Circles &circles, const Ellipses &ellipses,
                QColor bgColor, bool draw) {
  QSize size = imgSize(circles, ellipses);
  QImage img = QImage(size, QImage::Format_ARGB32);
  img.fill(bgColor);
  view->setBackgroundBrush(bgColor);
  QPainter p(&img);

  drawAxis(&p);

  if (x)
    for (int i = 0; i < circles.size(); i++)
      drawCircle(&p, circles[i], draw);

  for (int i = 0; i < ellipses.size(); i++)
    drawEllipse(&p, ellipses[i], draw);
  if (!x)
    for (int i = 0; i < circles.size(); i++)
      drawCircle(&p, circles[i], draw);
  if (draw) {
    QPixmap pixmap = QPixmap::fromImage(img);
    scene->clear();
    scene->addPixmap(pixmap);
    view->setScene(scene);
  }
}

void View::drawCircle(QPainter *p, const Circle &circle, bool draw) {
  Circle realCircle = transformCircle(circle);
  if (realCircle.method == STANDART)
    standartDrawCircle(p, realCircle, draw);
  else if (realCircle.method == CANONICAL)
    canonicalDrawCircle(p, realCircle, draw);
  else if (realCircle.method == PARAMETRIC)
    parametricDrawCircle(p, realCircle, draw);
  else if (realCircle.method == BRESENHAM)
    bresenhamDrawCircle(p, realCircle, draw);
  else if (realCircle.method == MIDDLE_DOT)
    midPointDrawCircle(p, realCircle, draw);
}

void View::drawSym(QPainter *p, const QPointF &center, double x, double y,
                   const QColor &color, bool draw) {
  double c_y = center.y();
  double c_x = center.x();

  QPointF p1 = QPointF(c_x + x, c_y + y);
  QPointF p2 = QPointF(c_x + x, c_y - y);
  QPointF p3 = QPointF(c_x - x, c_y + y);
  QPointF p4 = QPointF(c_x - x, c_y - y);

  drawPoint(p, p1, color, draw);
  drawPoint(p, p2, color, draw);
  drawPoint(p, p3, color, draw);
  drawPoint(p, p4, color, draw);
}

void View::drawPoint(QPainter *p, const QPointF &point, const QColor &color,
                     bool draw) {
  if (draw) {
    p->setPen(color);
    p->drawPoint(point);
  }
}

void View::standartDrawCircle(QPainter *p, const Circle &circle, bool draw) {
  if (draw) {
    p->setBrush(QColor(0, 0, 0, 0));
    p->setPen(circle.color);
  }

  double x = circle.center.x();
  double y = circle.center.y();
  double r = circle.r;

  QPointF leftTop = QPointF(x - r, y + r);
  QPointF rightBottom = QPointF(x + r, y - r);
  if (draw)
    p->drawEllipse(QRectF(leftTop, rightBottom));
}

void View::canonicalDrawCircle(QPainter *p, const Circle &circle, bool draw) {
  const int r = qRound(circle.r);
  const int r2 = r * r;
  const int deltaX = qRound(r / sqrt(2));
  for (int x = 0; x <= deltaX; ++x) {
    const int y = qRound(sqrt(r2 - x * x));
    drawSym(p, circle.center, x, y, circle.color, draw);
    drawSym(p, circle.center, y, x, circle.color, draw);
  }
}

void View::parametricDrawCircle(QPainter *p, const Circle &circle, bool draw) {
  const int r = qRound(circle.r);
  const float dt = 1.0f / r;
  for (float t = M_PI / 4.0f; t >= -dt / 2.0f; t -= dt) {
    const int x = qRound(r * cos(t));
    const int y = qRound(r * sin(t));
    drawSym(p, circle.center, x, y, circle.color, draw);
    drawSym(p, circle.center, y, x, circle.color, draw);
  }
}

void View::bresenhamDrawCircle(QPainter *p, const Circle &circle, bool draw) {
  const int r = qRound(circle.r);
  int x = 0;
  int y = r;

  int d = 2 * (1 - r);
  while (y >= 0) {
    drawSym(p, circle.center, x, y, circle.color, draw);

    if (d < 0) { // пиксел внутри окружности
      const int d1 = 2 * (d + y) - 1;
      ++x;
      if (d1 <= 0) // горизонтальный шаг
        d += 2 * x + 1;
      else { // диагональный шаг
        --y;
        d += 2 * (x - y + 1);
      }
    } else if (d > 0) { // пиксел лежит вне окружности
      const int d2 = 2 * (d - x) - 1;
      --y;
      if (d2 > 0) // вертикальный шаг
        d += 1 - 2 * y;
      else { // диагональный шаг
        ++x;
        d += 2 * (x - y + 1);
      }
    } else { // пиксель лежит на окружности
      ++x;
      --y;
      d += 2 * (x - y + 1);
    }
  }
}

void View::midPointDrawCircle(QPainter *p, const Circle &circle, bool draw) {
  const int r = qRound(circle.r);
  int x = 0;
  int y = r;
  int d = 1 - r;
  do {
    drawSym(p, circle.center, x, y, circle.color, draw);
    drawSym(p, circle.center, y, x, circle.color, draw);

    ++x;
    if (d < 0) // средняя точка внутри окружности, ближе верхний пиксел,
               // горизонтальный шаг
      d += 2 * x + 1;
    else { // средняя точка вне окружности, ближе диагональный пиксел,
           // диагональный шаг
      --y;
      d += 2 * (x - y) + 1;
    }
  } while (x <= y);
}

void View::drawEllipse(QPainter *p, const Ellipse &ellipse, bool draw) {
  Ellipse realEllipse = transformEllipse(ellipse);
  if (realEllipse.method == STANDART)
    standartDrawEllipse(p, realEllipse, draw);
  else if (realEllipse.method == CANONICAL)
    canonicalDrawEllipse(p, realEllipse, draw);
  else if (realEllipse.method == PARAMETRIC)
    parametricDrawEllipse(p, realEllipse, draw);
  else if (realEllipse.method == BRESENHAM)
    bresenhamDrawEllipse(p, realEllipse, draw);
  else if (realEllipse.method == MIDDLE_DOT)
    midPointDrawEllipse(p, realEllipse, draw);
}

void View::standartDrawEllipse(QPainter *p, const Ellipse &ellipse, bool draw) {
  if (draw) {
    p->setBrush(QColor(0, 0, 0, 0));
    p->setPen(ellipse.color);
  }

  double x = ellipse.center.x();
  double y = ellipse.center.y();
  double a = ellipse.a;
  double b = ellipse.b;

  QPointF leftTop = QPointF(x - a, y + b);
  QPointF rightBottom = QPointF(x + a, y - b);
  if (draw)
    p->drawEllipse(QRectF(leftTop, rightBottom));
}

void View::canonicalDrawEllipse(QPainter *p, const Ellipse &ellipse,
                                bool draw) {
  const int a = ellipse.a;
  const int b = ellipse.b;
  const int a2 = a * a;
  const int b2 = b * b;

  const float bDivA = static_cast<float>(b) / a;
  const int deltaX = qRound(a2 / sqrt(a2 + b2));
  for (int x = 0; x <= deltaX; ++x) {
    const int y = qRound(sqrt(static_cast<float>(a2 - x * x)) * bDivA);
    drawSym(p, ellipse.center, x, y, ellipse.color, draw);
  }

  const float aDivB = static_cast<float>(a) / b;
  const int deltaY = qRound(b2 / sqrt(a2 + b2));
  for (int y = 0; y <= deltaY; ++y) {
    const int x = qRound(sqrt(static_cast<float>(b2 - y * y)) * aDivB);
    drawSym(p, ellipse.center, x, y, ellipse.color, draw);
  }
}

void View::parametricDrawEllipse(QPainter *p, const Ellipse &ellipse,
                                 bool draw) {
  const int a = ellipse.a;
  const int b = ellipse.b;
  const float dt = 1.0f / qMax(a, b);
  for (float t = M_PI / 2.0f; t >= -dt / 2.0f; t -= dt) {
    const int x = qRound(a * cos(t));
    const int y = qRound(b * sin(t));
    drawSym(p, ellipse.center, x, y, ellipse.color, draw);
  }
}

void View::bresenhamDrawEllipse(QPainter *p, const Ellipse &ellipse,
                                bool draw) {
  const int a = ellipse.a;
  const int b = ellipse.b;
  int x = 0;
  int y = b;

  const int a2 = a * a;
  const int b2 = b * b;

  // разность квадратов расстояний от центра окружности эллипса до диагонального
  // пиксела и до идеального эллипса
  int d = a2 + b2 - 2 * a2 * y;
  while (y >= 0) {
    drawSym(p, ellipse.center, x, y, ellipse.color, draw);
    if (d < 0) { // пиксел лежит внутри эллипса
      const int d1 = 2 * (d + a2 * y) - a2; // lг - lд
      ++x;
      if (d1 < 0) // горизонтальный шаг
        d += b2 * (2 * x + 1);
      else { // диагональный шаг
        --y;
        d += 2 * (b2 * x - a2 * y) + a2 + b2;
      }
    } else if (d > 0) { // пиксел лежит вне эллипса
      const int d2 = 2 * (d - b2 * x) - b2; // lв - lд
      --y;
      if (d2 >= 0) // вертикальный шаг
        d += a2 * (1 - 2 * y);
      else { // диагональный шаг
        ++x;
        d += 2 * (b2 * x - a2 * y) + a2 + b2;
      }
    } else { // пиксел лежит на эллипсе, диагональный шаг
      ++x;
      --y;
      d += 2 * (b2 * x - a2 * y) + a2 + b2;
    }
  }
}

void View::midPointDrawEllipse(QPainter *p, const Ellipse &ellipse, bool draw) {
  const int a = ellipse.a;
  const int b = ellipse.b;

  const int a2 = a * a;
  const int b2 = b * b;

  int x = 0;
  int y = b;

  int f = b2 + a2 * (y - 0.5f) * (y - 0.5) - static_cast<long long>(a2) * b2;
  const int deltaX = a2 / sqrt(b2 + a2);
  while (x <= deltaX) {
    drawSym(p, ellipse.center, x, y, ellipse.color, draw);

    ++x;
    if (f > 0) {
      --y;
      f += -2 * a2 * y; // f += dy;
    }
    f += b2 * (2 * x + 1); // f += df;
  }

  f += 0.75f * (a2 - b2) - (b2 * x + a2 * y);
  while (y >= 0) {
    drawSym(p, ellipse.center, x, y, ellipse.color, draw);

    --y;
    if (f < 0) {
      ++x;
      f += 2 * b2 * x; // f += dx;
    }
    f += a2 * (1 - 2 * y); // f += df;
  }
}
