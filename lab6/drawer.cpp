#include "drawer.h"
#include "QMessageBox"
#include "polygones.h"
#include <QCoreApplication>
#include <QGraphicsPixmapItem>
#include <QMainWindow>
#include <QPainter>
#include <QThread>
#include <QTime>
#include <QWidget>
#include <stack>
#include <time.h>

drawer_t::drawer_t(QGraphicsScene *scene) : scene(scene) {}

void drawer_t::redraw(polygones_t &polygones) {
  clear();
  for (size_t i = 0; i < polygones.size(); i++) {
    polygones[i].filled = false;
    for (size_t j = 0; j < polygones[i].edges.size(); j++) {
      draw_point(polygones[i].edges[j].p1.x, polygones[i].edges[j].p1.y,
                 polygones[i].edges[j].p1.color);
      if (j == polygones[i].edges.size() - 1)
        draw_point(polygones[i].edges[j].p2.x, polygones[i].edges[j].p2.y,
                   polygones[i].edges[j].p2.color);
    }
  }
  for (size_t i = 0; i < polygones.size(); i++)
    draw_edges(polygones[i].edges);
  for (size_t i = 0; i < polygones.size(); i++)
    seed_method(polygones[i], polygones[i].seed,
                polygones[i].edges.back().p1.color, polygones[i].color, 0, 0);
  for (size_t i = 0; i < polygones.size(); i++)
    draw_edges(polygones[i].edges);
}

void drawer_t::draw_edge(const edge_t &edge) {
  color_t color = edge.color;
  point_t p1 = edge.p1, p2 = edge.p2;
  scene->addLine(p1.x, p1.y, p2.x, p2.y,
                 {qRgba(color.r, color.g, color.b, color.alpha)});
}

void drawer_t::draw_edges(const std::vector<edge_t> &edges) {
  for (size_t i = 0; i < edges.size(); i++)
    draw_edge(edges[i]);
}

static int sign(double d) {
  if (d < 0)
    return -1;
  else
    return 1;
}

static void bresenham_int(QPainter &painter, QImage &image, const point_t &p_b,
                          const point_t &p_e) {
  if (p_b.x == p_e.x && p_b.y == p_e.y) {
    painter.drawPoint(round(p_b.x), round(p_b.y));
    return;
  }
  point_t cur;
  int dx, dy;
  bool fl = 0;
  int sx, sy, f;
  dx = p_e.x - p_b.x;
  dy = p_e.y - p_b.y;
  sx = sign(dx);
  sy = sign(dy);
  dx = abs(dx);
  dy = abs(dy);
  if (dy >= dx) {
    int tmp = dx;
    dx = dy;
    dy = tmp;
    fl = 1;
  }
  f = 2 * dy - dx;
  cur = p_b;
  int len = dx;
  for (int i = 0; i < len; i++) {
    painter.drawPoint(cur.x, cur.y);
    if (f >= 0) {
      if (fl == 1)
        cur.x += sx;
      else
        cur.y += sy;
      f -= 2 * dx;
    }
    if (f < 0) {
      if (fl == 1)
        cur.y += sy;
      else
        cur.x += sx;
    }
    f += 2 * dy;
  }
}

void drawer_t::draw_outline(QImage *outline_image, const QColor &outline_color,
                            const std::vector<edge_t> &edges) {
  QPainter painter(outline_image);
  painter.setPen(outline_color);
  for (size_t i = 0; i < edges.size(); i++)
    bresenham_int(painter, *outline_image, edges[i].p1, edges[i].p2);

  for (size_t i = 0; i < edges.size(); i++)
    for (size_t j = 0; j < edges.size(); j++)
      if (edges[i].p2.x == edges[j].p1.x && edges[i].p2.y == edges[j].p1.y)
        if (sign(edges[i].p2.y - edges[i].p1.y) ==
            sign(edges[j].p1.y - edges[j].p2.y))
          painter.drawPoint(edges[i].p2.x + 1, edges[i].p2.y);
}

void drawer_t::draw_point(double x, double y, const color_t &color) {
  const double r = 2.5;
  scene->addEllipse(x - r, y - r, 2 * r, 2 * r,
                    {qRgba(color.r, color.g, color.b, color.alpha)},
                    {qRgba(color.r, color.g, color.b, color.alpha)});
}

void drawer_t::draw_points(const std::vector<point_t> &points) {
  for (auto &p : points)
    draw_point(p.x, p.y, p.color);
}

void sleepFeature(int sleep_time) {
  QTime end = QTime::currentTime().addMSecs(sleep_time);
  while (QTime::currentTime() < end)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}

void drawer_t::pixmap_add(QImage image) {
  QPixmap pixmap = QPixmap::fromImage(image);
  QGraphicsPixmapItem *pm = scene->addPixmap(pixmap);
  pm->update();
}

static bool is_in_range(const point_t &p, const QSize &sizes) {
  if (p.x > sizes.width() || p.x < 0)
    return false;
  if (p.y > sizes.height() || p.y < 0)
    return false;
  return true;
}

bool check_point(double x, double y, double max_y, QImage image,
                 QColor pen_color) {
  int count = 0;
  for (int i = y; i < max_y; i++)
    if (image.pixelColor(x, i) == pen_color)
      count++;
  return count % 2;
}

int drawer_t::seed_method(polygone_t &polygone, const point_t &seed_p,
                          const color_t &fg, const color_t &bg, bool is_delay,
                          int sleep_time) {

  if (polygone.edges.empty())
    return 0;

  point_t min, max;
  polygone_sizes(min, max, polygone);

  QImage image(QSize(max.x + 5, max.y + 5), QImage::Format_RGBA64);
  image.fill(Qt::transparent);
  draw_outline(&image, QColor(fg.r, fg.g, fg.b), polygone.edges);
  QPainter painter(&image);
  painter.setPen(QColor(bg.r, bg.g, bg.b));

  QColor pen_color = QColor(fg.r, fg.g, fg.b);
  QColor brush_color = QColor(bg.r, bg.g, bg.b);

  if (!check_point(seed_p.x, seed_p.y, max.y, image, pen_color))
    return 1;

  std::stack<point_t> stack;
  stack.push(seed_p);
  while (!stack.empty()) {
    point_t p = stack.top();
    stack.pop();
    painter.drawPoint(p.x, p.y);
    point_t tmp = p;
    p.x++;
    int left_border, right_border;
    while (image.pixelColor(p.x, p.y) != pen_color) {
      if (!is_in_range(p, QSize(max.x + 5, max.y + 5))) {
        pixmap_add(image);
        return 0;
      }
      painter.drawPoint(p.x, p.y);
      p.x++;
    }
    right_border = p.x - 1;
    p = tmp;
    p.x--;
    while (image.pixelColor(p.x, p.y) != pen_color) {
      if (!is_in_range(p, QSize(max.x + 5, max.y + 5))) {
        pixmap_add(image);
        return 0;
      }
      painter.drawPoint(p.x, p.y);
      p.x--;
    }
    left_border = p.x + 1;

    p.y++;
    for (size_t i = 0; i < 2; i++) {
      p.x = left_border;
      bool found;
      while (p.x <= right_border) {
        found = false;
        while (image.pixelColor(p.x, p.y) != brush_color &&
               image.pixelColor(p.x, p.y) != pen_color && p.x <= right_border) {
          if (!is_in_range(p, QSize(max.x + 5, max.y + 5))) {
            pixmap_add(image);
            return 0;
          }
          found = true;
          p.x++;
        }
        if (found) {
          p.x--;
          stack.push(p);
          p.x++;
        }
        while ((image.pixelColor(p.x, p.y) == brush_color ||
                image.pixelColor(p.x, p.y) == pen_color) &&
               p.x <= right_border) {
          if (!is_in_range(p, QSize(max.x + 5, max.y + 5))) {
            pixmap_add(image);
            return 0;
          }
          p.x++;
        }
      }
      p.y -= 2;
      if (is_delay) {
        sleepFeature(sleep_time);
        pixmap_add(image);
      }
    }
  }

  pixmap_add(image);
  polygone.filled = true;
}

void drawer_t::clear() { scene->clear(); }
