#include "circles.h"

#include <stdexcept>
#include <vector>

Circles::Circles()
    : arr(std::vector<Circle>()), minX(0), maxX(0), minY(0), maxY(0) {}

void Circles::push_back(const Circle &circle) {
  arr.push_back(circle);
  update_size(circle);
}

int Circles::size() const { return arr.size(); }

const Circle &Circles::getCircle(int index) const {
  if (index >= size() || index < 0)
    throw std::out_of_range("Out of range");

  return arr[index];
}

const Circle &Circles::operator[](int index) const {
  if (index >= size() || index < 0)
    throw std::out_of_range("Out of range");

  return arr[index];
}

void Circles::update_size(const Circle &circle) {
  if (maxX < circle.maxX)
    maxX = circle.maxX;
  if (minX > circle.minX)
    minX = circle.minX;
  if (maxY < circle.maxY)
    maxY = circle.maxY;
  if (minY > circle.minY)
    minY = circle.minY;
}
