#ifndef ELLIPSES_H
#define ELLIPSES_H

#include "ellipse.h"

#include <vector>


class Ellipses
{
public:
    double minX, maxX;
    double minY, maxY;
private:
    std::vector<Ellipse> arr;
    void update_size(const Ellipse &ellipse);
public:
    Ellipses();
    void push_back(const Ellipse &ellipse);
    int size() const;
    const Ellipse & getEllipse(int index) const;
    const Ellipse &operator [](int index) const;
};

#endif // ELLIPSES_H
