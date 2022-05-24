#ifndef FIGURES_H
#define FIGURES_H

#include <QColor>
#include <QPainter>

class Figure {
public:
    Figure(const QColor& fg = QColor(0, 0, 0), const QColor& bg = QColor(255, 255, 255))
        : fg(fg)
        , bg(bg)
    {
    }
    virtual ~Figure() { }

    virtual void paint(QPainter& painter) const = 0;

    void set_fg(const QColor& color) { fg = color; }
    void set_bg(const QColor& color) { bg = color; }

protected:
    QColor fg;
    QColor bg;
};

class Point : public Figure {
public:
    explicit Point(const QColor& fg)
        : Figure(fg)
    {
    }

    Point(double x, double y, const QColor& fg = QColor(0, 0, 0))
        : Figure(fg)
        , x(x)
        , y(y)
    {
    }
    virtual void paint(QPainter& painter) const override;

    double get_x() const;
    double get_y() const;
    void set_x(int x);
    void set_y(int y);

    Point operator-(const Point& p);
    Point operator+(const Point& p);
    bool operator==(const Point& other);
    bool operator<(const Point& p);

private:
    int x = 0;
    int y = 0;
};

class Line : public Figure {
public:
    explicit Line(const QColor& fg)
        : Figure(fg)
        , p1(fg)
        , p2(fg)
    {
    }
    Line(const QColor& fg, int x, int y)
        : Figure(fg)
        , p1(Point(x, y, fg))
        , p2(Point(x, y, fg))
    {
    }

    virtual void paint(QPainter& painter) const override;

    Point get_p1();
    Point get_p2();
    void set_p1(const Point& p);
    void set_p2(const Point& p);

private:
    Point p1;
    Point p2;
};

class Rectangle : public Figure {
public:
    explicit Rectangle(const QColor& fg)
        : Figure(fg)
        , p1(fg)
        , p2(fg)
    {
    }
    Rectangle(const QColor& fg, int x, int y)
        : Figure(fg)
        , p1(Point(x, y, fg))
        , p2(Point(x, y, fg))
    {
    }
    explicit Rectangle(int x = 0, int y = 0);

    virtual void paint(QPainter& painter) const override;

    Point get_p1() const;
    Point get_p2() const;
    void set_p1(const Point& p);
    void set_p2(const Point& p);

    int get_top() const;
    int get_bottom() const;
    int get_right() const;
    int get_left() const;

private:
    Point p1;
    Point p2;
};

#endif // FIGURES_H
