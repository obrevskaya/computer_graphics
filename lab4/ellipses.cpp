#include "ellipses.h"

#include <stdexcept>
#include <vector>

Ellipses::Ellipses(): arr(std::vector<Ellipse>()), minX(0), maxX(0), minY(0), maxY(0)
{
}

void Ellipses::push_back(const Ellipse &ellipse)
{
    arr.push_back(ellipse);
    update_size(ellipse);
}

int Ellipses::size() const
{
    return arr.size();
}

const Ellipse &Ellipses::getEllipse(int index) const
{
    if (index >= size() || index < 0)
        throw std::out_of_range("Out of range");

    return arr[index];
}

const Ellipse &Ellipses::operator [](int index) const
{
    if (index >= size() || index < 0)
        throw std::out_of_range("Out of range");

    return arr[index];
}

void Ellipses::update_size(const Ellipse &ellipse)
{
    if (maxX < ellipse.maxX)
        maxX = ellipse.maxX;
    if (minX > ellipse.minX)
        minX = ellipse.minX;
    if (maxY < ellipse.maxY)
        maxY = ellipse.maxY;
    if (minY > ellipse.minY)
        minY = ellipse.minY;
}

