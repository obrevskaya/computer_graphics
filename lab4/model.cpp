#include "model.h"
#include <cmath>

Model::Model() {
  this->methode = STANDART;
  this->color = Qt::black;
  this->bgColor = Qt::white;
}

inline double toRadians(double angle) { return angle * M_PI / 180; }

void Model::setColor(QColor &color) { this->color = color; }

void Model::setBgColor(QColor &color) { this->bgColor = color; }

void Model::backState() {
  if (intHist.size() == 0)
    return;

  // int elements = intHist.top().first;
  figure_t figure = intHist.top().second;
  intHist.pop();

  if (figure == CIRCLE) {
    circles = circlesHistory.top();
    circlesHistory.pop();
  } else {
    ellipses = ellipsesHistory.top();
    ellipsesHistory.pop();
  }
}

void Model::clear() {
  while (intHist.size() != 0)
    backState();
}

void Model::setMethod(method_t methode) { this->methode = methode; }

QColor Model::getBgColor() { return bgColor; }

QColor Model::getLineColor() { return color; }

void Model::addCircle(const QPointF &center, double r) {
  circlesHistory.push(circles);
  intHist.push(std::pair<int, figure_t>(1, CIRCLE));
  circles.push_back(Circle(r, center, methode, color));
}

void Model::addCircles(const QPointF &center, double r, double dr, int n) {
  circlesHistory.push(circles);
  intHist.push(std::pair<int, figure_t>(n, CIRCLE));
  for (int i = 0; i < n; i++) {
    circles.push_back(Circle(r, center, methode, color));
    r += dr;
  }
}

void Model::addEllipses(const QPointF &center, double a, double b, double da,
                        double db, int n) {
  ellipsesHistory.push(ellipses);
  intHist.push(std::pair<int, figure_t>(n, ELLIPSE));
  for (int i = 0; i < n; i++) {
    ellipses.push_back(Ellipse(a, b, center, methode, color));
    a += da;
    b += db;
  }
}

void Model::addEllipse(const QPointF &center, double a, double b) {
  ellipsesHistory.push(ellipses);
  intHist.push(std::pair<int, figure_t>(1, ELLIPSE));
  ellipses.push_back(Ellipse(a, b, center, methode, color));
}

const Ellipses &Model::getEllipses() { return ellipses; }

const Circles &Model::getCircles() { return circles; }
