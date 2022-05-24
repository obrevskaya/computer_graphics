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
    explicit Point(const QColor& fg = QColor(0, 0, 0))
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

    friend Point get_vector(const Point& p1, const Point& p2);

    Point operator-(const Point& p);
    Point operator+(const Point& p);
    Point operator*(double num);
    Point operator/(double num);
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
    Line(Point p1, Point p2)
        : p1(p1)
        , p2(p2)
    {
    }

    virtual void paint(QPainter& painter) const override;

    bool find_intersection(Point& p, Line& line, int norm);
    bool is_visible(const Point& p, int norm);
    Point find_distance(const Point& a);
    Point get_p1();
    Point get_p2();
    void set_p1(const Point& p);
    void set_p2(const Point& p);

private:
    Point p1;
    Point p2;
};

class Polygon : public Figure {
public:
    explicit Polygon(const QColor& fg)
        : Figure(fg)
    {
    }

    virtual void paint(QPainter& painter) const override;

    size_t edges_count() { return edges.size(); }
    Point get_last() { return points[points.size() - 2]; }
    Line get_edge(size_t index);
    std::vector<Line> get_edges() { return edges; }
    int is_convex();

    void add_point(Point p);
    void change_last(Point p);
    void close();
    void close_with_pop();

private:
    int sign(int num);
    std::vector<Point> points;
    std::vector<Line> edges;
};

#endif // FIGURES_H
