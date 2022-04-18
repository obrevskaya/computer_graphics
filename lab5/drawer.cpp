#include "drawer.h"
#include "polygones.h"
#include <QCoreApplication>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QThread>
#include <QTime>
#include <QWidget>
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

static void bresenham_int_spec(QPainter &painter, QImage &image,
                               const point_t &p_b, const point_t &p_e) {
  if (p_b.x == p_e.x && p_b.y == p_e.y) {
    painter.drawPoint(p_b.x, p_b.y);
    return;
  }
  point_t cur;
  int dx, dy, dx_real, dy_real;
  bool fl = 0;
  int sx, sy, f;
  dx = p_e.x - p_b.x;
  dy = p_e.y - p_b.y;
  sx = sign(dx);
  sy = sign(dy);
  dx = abs(dx);
  dy = abs(dy);
  dx_real = dx;
  dy_real = dy;
  if (dy >= dx) {
    int tmp = dx;
    dx = dy;
    dy = tmp;
    fl = 1;
  }
  f = 2 * dy - dx;
  cur = p_b;
  int len = dx + 1;
  int last_y = -1;
  for (int i = 0; i < len; i++) {
    // гарантируем, что последний пиксель линии, у которой dx > dy,
    // будет закрашен несмотря на заполнение по левой границе.
    // это нужно, чтобы в одной горизонтали не образовалось двух ограничивающих
    // пикселей, нарисованных разными линиями
    if (dx_real > dy_real && cur.y == p_e.y) {
      painter.drawPoint(p_e.x, p_e.y);
      break;
    }
    // заполняем по левой границе, чтобы обеспечить единственность
    // ограничивающего пикселя в одной строке
    if (cur.y != last_y) {
      last_y = cur.y;
      // точки не должны быть начальной и конечной, потому что иначе
      // на краях отрезка будут вставлены лишние пиксели, при этом обработкой
      // краёв занимается цикл по ребрам после отрисовки всего контура
      if (image.pixelColor(cur.x, cur.y) == painter.pen().color() &&
          (cur.x != p_b.x || cur.y != p_b.y) &&
          (cur.x != p_e.x || cur.y != p_e.y)) {
        painter.drawPoint(cur.x + 1, cur.y);
      } else {
        painter.drawPoint(cur.x, cur.y);
      }
    }
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
  // в результате будет нарисован контур, у которого в каждой горизонтали
  // чётное количество ограничивающих пикселей за исключением ситуаций
  // экстремумов
  for (size_t i = 0; i < edges.size(); i++)
    bresenham_int_spec(painter, *outline_image, edges[i].p1, edges[i].p2);

  // обработка экстремумов
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

void drawer_t::list_edges_flag_method(polygone_t &polygone, const color_t &fg,
                                      const color_t &bg, bool is_delay,
                                      int sleep_time) {
  if (polygone.edges.empty())
    return;

  point_t min, max;
  polygone_sizes(min, max, polygone);

  QImage outline_image(QSize(max.x + 5, max.y + 5), QImage::Format_RGBA64);
  outline_image.fill(Qt::transparent);
  draw_outline(&outline_image, QColor(bg.r, bg.g, bg.b), polygone.edges);
  QPainter painter(&outline_image);
  painter.setPen(QColor(bg.r, bg.g, bg.b));

  for (int y = min.y; y < max.y; y++) {
    bool flag = false;
    for (int x = min.x; x < max.x; x++) {
      if (outline_image.pixelColor(x, y) == QColor(bg.r, bg.g, bg.b))
        flag = !flag;
      if (flag)
        painter.drawPoint(x, y);
    }
    if (is_delay) {
      sleepFeature(sleep_time);
      pixmap_add(outline_image);
    }
  }

  painter.setPen(QColor(fg.r, fg.g, fg.b));
  draw_edges(polygone.edges);
  pixmap_add(outline_image);
  polygone.filled = true;
}

void drawer_t::clear() { scene->clear(); }
