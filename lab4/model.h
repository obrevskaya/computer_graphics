#ifndef MODEL_H
#define MODEL_H

#include "ellipses.h"
#include "circles.h"
#include "methods.h"

#include <QColor>
#include <QPointF>
#include <stack>


typedef enum
{
    CIRCLE,
    ELLIPSE,
} figure_t;

class Model
{
    QColor color;
    method_t methode;
    QColor bgColor;
    Ellipses ellipses;
    Circles circles;
    std::stack<Ellipses> ellipsesHistory;
    std::stack<Circles> circlesHistory;
    std::stack<std::pair<int, figure_t>> intHist;

//    std::vector<int> history;
public:
    Model();
    void setColor(QColor &color);
    void setBgColor(QColor &color);
    void addSpector(QPointF center, double r, double angle);
    void setMethod(method_t methode);
    QColor getBgColor();
    QColor getLineColor();
    void addCircle(const QPointF &center, double r);
    void addEllipse(const QPointF &center, double a, double b);
    const Ellipses &getEllipses();
    const Circles &getCircles();
    void backState();
    void clear();
    void addCircles(const QPointF &center, double r, double dr, int n);
    void addEllipses(const QPointF &center, double a, double b, double da, double db, int n);
};

#endif // MODEL_H
