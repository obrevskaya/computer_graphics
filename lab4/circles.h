#ifndef CIRCLES_H
#define CIRCLES_H

#include "circle.h"

#include <vector>


class Circles
{
public:
    double minX, maxX;
    double minY, maxY;
private:
    std::vector<Circle> arr;
    void update_size(const Circle &circle);
public:
    Circles();
    void push_back(const Circle &circle);
    int size() const;
    const Circle & getCircle(int index) const;
    const Circle &operator [](int index) const;
};

#endif // CIRCLES_H
