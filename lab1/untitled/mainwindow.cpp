#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QTimer>
#include <QtMath>
#include <iostream>
#include <math.h>
#include <vector>

#define DRAW_POINT_EVEN 0
#define DRAW_POLYGON_EVEN 1
#define CLEAN_EVEN 2
#define DEL_POINT_EVEN 3

#define EPS 1e-5
#define R_POINT 7
#define ARROW 7
#define FIELD 5 / 10

int rad = 3;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  max_x = 100;
  min_x = 0;
  max_y = 100;
  min_y = 0;
  QGraphicsScene *scene = new QGraphicsScene();
  ui->graphicsView->setScene(scene);
  ui->graphicsView->scene()->clear();
}

MainWindow::~MainWindow() {
  delete ui->graphicsView->scene();
  delete ui;
}

void MainWindow::on_action_9_triggered() { QApplication::quit(); }

void MainWindow::on_action_6_triggered() {
  QMessageBox::about(
      this, "ПРОГРАММА",
      "Находит 2 подобных N-угольника с максимальным N\nМногоугольники "
      "задаются на плоскости координатами вершин контуров. Вершины в контуре "
      "перечисляются в порядке обхода против часовой стрелки.");
}

void MainWindow::on_action_8_triggered() {
  QMessageBox::about(this, "АВТОР", "Обревская Вероника ИУ7-42Б");
}

void MainWindow::showEvent(QShowEvent *ev) {
  QMainWindow::showEvent(ev);
  drawPoints();
}

void MainWindow::drawPoint(QPainter *p, QColor col, QPoint point) {
  p->setBrush(QColor(col));
  p->setPen(QColor(col));
  int real_x, real_y;
  from_abs_coordinates(point.x(), point.y(), &real_x, &real_y);

  p->drawEllipse(real_x - rad, real_y - ui->menubar->height() - rad, R_POINT,
                 R_POINT);

  p->setBrush(QColor(0, 0, 0));
  p->setPen(QColor(0, 0, 0));
}

void MainWindow::drawPoints() {
  QImage image =
      QImage(ui->graphicsView->geometry().width(),
             ui->graphicsView->geometry().height(), QImage::Format_RGB32);
  QPainter p(&image);
  image.fill(QColor(255, 255, 255));
  drawAxis(&p);
  for (size_t i = 0; i + 1 < polygons.size(); i++) {
    QPoint buf;
    QPolygon polygon = QPolygon();
    int res_x, res_y;
    for (size_t j = 0; j < polygons[i].size(); j++) {
      from_abs_coordinates(polygons[i][j].x(), polygons[i][j].y(), &res_x,
                           &res_y);
      buf = QPoint(res_x, res_y - ui->menubar->height());
      polygon << buf;
    }
    p.setBrush(QColor(0, 0, 0, 0));
    p.setPen(QPen(Qt::blue, 2));

    p.drawPolygon(polygon);
  }

  for (size_t i = 0; i < polygons.size(); i++) {
    QPoint buf;
    for (size_t j = 0; j < polygons[i].size(); j++) {
      buf = QPoint(polygons[i][j].x(), polygons[i][j].y());
      drawPoint(&p, QColor(0, 0, 255), buf);
    }
  }

  if (is_draw_polygon) {
    QPolygon polygon = QPolygon();

    int res_x, res_y;
    QPoint buf;
    if (polygons.size() > 0) {
      for (size_t i = 0; i < polygons[polygons.size() - 1].size(); i++) {
        from_abs_coordinates(polygons[polygons.size() - 1][i].x(),
                             polygons[polygons.size() - 1][i].y(), &res_x,
                             &res_y);
        buf = QPoint(res_x, res_y - ui->menubar->height());
        polygon << buf;
      }
    }

    p.setBrush(QColor(0, 0, 0, 0));
    p.setPen(QPen(Qt::blue, 2));

    p.drawPolygon(polygon);
    lastevents.push_back(DRAW_POLYGON_EVEN);
    drawAxis(&p);
    is_draw_polygon = 0;

    std::vector<QPoint> vec;
    polygons.push_back(vec);
  }

  if (is_find_polygon) {
    is_find_polygon = 0;

    QPolygon polygon = QPolygon();

    int res_x, res_y;
    QPoint buf;
    for (size_t i = 0; i < polygons[ind1].size(); i++) {
      from_abs_coordinates(polygons[ind1][i].x(), polygons[ind1][i].y(), &res_x,
                           &res_y);
      buf = QPoint(res_x, res_y - ui->menubar->height());
      polygon << buf;
    }

    p.setBrush(QColor(0, 0, 0, 0));
    p.setPen(QPen(Qt::red, 5, Qt::SolidLine));

    p.drawPolygon(polygon);

    QPolygon polygon2 = QPolygon();
    for (size_t i = 0; i < polygons[ind2].size(); i++) {
      from_abs_coordinates(polygons[ind2][i].x(), polygons[ind2][i].y(), &res_x,
                           &res_y);
      buf = QPoint(res_x, res_y - ui->menubar->height());
      polygon2 << buf;
    }

    p.setBrush(QColor(0, 0, 0, 0));
    p.setPen(QPen(Qt::red, 5, Qt::SolidLine));

    p.drawPolygon(polygon2);
    drawAxis(&p);
  }

  QPixmap pixmap = QPixmap::fromImage(image);
  ui->graphicsView->scene()->clear();
  ui->graphicsView->scene()->addPixmap(pixmap);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  ui->lineEdit->setText("");
  int canv_x = event->pos().x() - ui->graphicsView->x();
  int canv_y = event->pos().y() - ui->graphicsView->y();

  int abs_x, abs_y;
  to_abs_coordinates(canv_x, canv_y, &abs_x, &abs_y);

  if (canv_x >= 0 && canv_y >= 0 && canv_x <= ui->graphicsView->width() &&
      canv_y <= ui->graphicsView->height() + ui->menubar->height()) {
    if (polygons.size() == 0) {
      std::vector<QPoint> c;
      polygons.push_back(c);
    }
    polygons[polygons.size() - 1].push_back(QPoint(abs_x, abs_y));
    lastevents.push_back(DRAW_POINT_EVEN);
    scan_to_table();
    drawPoints();
  }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  drawPoints();
}

size_t count_all_points(std::vector<std::vector<QPoint>> polygons) {
  size_t k = 0;
  for (size_t i = 0; i < polygons.size(); i++)
    for (size_t j = 0; j < polygons[i].size(); j++)
      k++;
  return k;
}

void MainWindow::on_backbutton_clicked() {
  if (lastevents.empty()) {
    ui->lineEdit->setText("Нечего отменять!");
    return;
  }
  if (lastevents.size() != 0) {
    if (lastevents.back() == DRAW_POINT_EVEN) {
      if (polygons.size())
        polygons[polygons.size() - 1].pop_back();
      size_t cnt_all_points = count_all_points(polygons);
      // ui->table->setItem(cnt_all_points, 0, nullptr);
      // ui->table->setItem(cnt_all_points, 1, nullptr);
      // ui->table->setItem(cnt_all_points, 2, nullptr);
      ui->table->removeRow(cnt_all_points);
      ui->table->clearSelection();
      drawPoints();
    }
    if (lastevents.back() == DRAW_POLYGON_EVEN) {
      if (polygons.size())
        polygons.pop_back();
      drawPoints();
    }
    if (lastevents.back() == CLEAN_EVEN) {
      polygons = dup_polygons;
      drawPoints();
      scan_to_table();
    }
    if (lastevents.back() == DEL_POINT_EVEN) {
      addPoint(del_points.back().x(), del_points.back().y(), false);
      del_points.pop_back();
      drawPoints();
    }
    lastevents.pop_back();
  }
}

int MainWindow::scan_from_table() {
  size_t cnt = count_all_points(polygons);
  size_t cnt_vec = polygons.size();
  // std::vector<int> size_polygons;

  //  for (size_t i = 0; i < cnt_vec; i++) {
  //    size_polygons.push_back(polygons[i].size());
  //  }

  for (size_t i = 0; i < cnt_vec; i++) {
    size_t cnt_point = polygons[i].size();
    for (size_t j = 0; j < cnt_point; j++)
      polygons[i].pop_back();
    polygons.pop_back();
  }

  double x, y;
  int num;
  bool rc;
  // int k = 0, k_k = 0;
  for (size_t i = 0; i < cnt_vec; i++) {
    std::vector<QPoint> c;
    polygons.push_back(c);
  }
  for (size_t i = 0; i < cnt; i++) {
    QTableWidgetItem *x_item = ui->table->item(i, 0);
    QTableWidgetItem *y_item = ui->table->item(i, 1);
    QTableWidgetItem *num_item = ui->table->item(i, 2);
    if (x_item == nullptr && y_item == nullptr && num_item == nullptr)
      continue;
    if (x_item == nullptr || y_item == nullptr) {
      ui->lineEdit->setText("Ошибка! Нет значения у координаты.");
      return EXIT_FAILURE;
    }
    if (num_item == nullptr) {
      ui->lineEdit->setText("Ошибка! Нет значения у №.");
      return EXIT_FAILURE;
    }
    QString x_str = x_item->text();
    QString y_str = y_item->text();
    QString num_str = num_item->text();
    if (x_str.isEmpty() && y_str.isEmpty() && num_str.isEmpty())
      continue;
    if (x_str.isEmpty() || y_str.isEmpty()) {
      ui->lineEdit->setText("Ошибка! Нет значения у координаты.");
      return EXIT_FAILURE;
    }
    if (num_str.isEmpty()) {
      ui->lineEdit->setText("Ошибка! Нет значения у №.");
      return EXIT_FAILURE;
    }
    x = x_str.toDouble(&rc);
    if (!rc) {
      ui->lineEdit->setText("Ошибка! Некорректное значение X.");
      return EXIT_FAILURE;
    }
    y = y_str.toDouble(&rc);
    if (!rc) {
      ui->lineEdit->setText("Ошибка! Некорректное значение Y.");
      return EXIT_FAILURE;
    }
    num = num_str.toInt(&rc);
    if (!rc) {
      ui->lineEdit->setText("Ошибка! Некорректное значение у №.");
      return EXIT_FAILURE;
    }
    if (num > (int)cnt_vec || num < 1) {
      ui->lineEdit->setText("Ошибка! Нет такого № у многоугольников.");
      return EXIT_FAILURE;
    }

    polygons[num - 1].push_back(QPoint(int(x), int(y)));
    //    k_k++;
    //    if (k_k == size_polygons[k]) {
    //      k_k = 0;
    //      k++;
    //    }

    if (x > max_x)
      max_x = x;
    else if (x < min_x)
      min_x = x;

    if (y > max_y)
      max_y = y;
    else if (y < min_y)
      min_y = y;
  }
  ui->lineEdit->setText("");
  return EXIT_SUCCESS;
}

void MainWindow::scan_to_table() {
  bool oldState = ui->table->blockSignals(true);
  int cnt_all_points = count_all_points(polygons);
  int k = 0;
  for (size_t i = 0; i < polygons.size(); i++)
    for (size_t j = 0; j < polygons[i].size(); j++) {
      QTableWidgetItem *x = new QTableWidgetItem;
      QTableWidgetItem *y = new QTableWidgetItem;
      QTableWidgetItem *num = new QTableWidgetItem;
      ui->table->setRowCount(cnt_all_points);
      x->setText(QString::number((double)polygons[i][j].x()));
      y->setText(QString::number((double)(polygons[i][j].y())));
      num->setText(QString::number((int)(i + 1)));
      ui->table->setItem(k, 0, x);
      ui->table->setItem(k, 1, y);
      ui->table->setItem(k, 2, num);
      k++;
    }
  ui->table->blockSignals(oldState);
}

void MainWindow::on_table_itemChanged() {
  scan_from_table();
  drawPoints();
}

void MainWindow::on_table_cellClicked(int row, int column) {
  drawPoints();
  bool rc;
  double x, y, num;

  QTableWidgetItem *x_item = ui->table->item(row, 0);
  QTableWidgetItem *y_item = ui->table->item(row, 1);
  QTableWidgetItem *num_item = ui->table->item(row, 2);
  if (x_item == nullptr || y_item == nullptr || num_item == nullptr)
    return;
  QString x_str = x_item->text();
  QString y_str = y_item->text();
  QString num_str = num_item->text();
  if (x_str.isEmpty() || y_str.isEmpty() || num_str.isEmpty())
    return;
  x = x_str.toDouble(&rc);
  if (!rc)
    return;
  y = y_str.toDouble(&rc);
  if (!rc)
    return;
  num = num_str.toInt(&rc);
  if (!rc)
    return;

  int real_x, real_y;
  from_abs_coordinates(x, y, &real_x, &real_y);
  ui->graphicsView->scene()->addEllipse(
      real_x - rad, real_y - rad - ui->menubar->height(), R_POINT + 2,
      R_POINT + 2, QPen(Qt::yellow, 1, Qt::SolidLine), QBrush(Qt::yellow));

  highlighted = row;
  highlight_num_vec = num - 1;
}

void MainWindow::to_abs_coordinates(int x, int y, int *res_x, int *res_y) {
  int field_x = (max_x - min_x) * FIELD;
  int field_y = (max_y - min_y) * FIELD;
  int nmax_x = max_x + field_x;
  int nmin_x = min_x - field_x;
  int nmax_y = max_y + field_y;
  int nmin_y = min_y - field_y;

  double x_k = (double)(nmax_x - nmin_x) / ui->graphicsView->width();
  double x_b = nmin_x;
  double y_k = (double)(nmax_y - nmin_y) / ui->graphicsView->height();
  double y_b = nmin_y;

  *res_x = x * x_k + x_b;
  *res_y = y * y_k + y_b;
}

void MainWindow::from_abs_coordinates(int x, int y, int *res_x, int *res_y) {
  int field_x = (max_x - min_x) * FIELD;
  int field_y = (max_y - min_y) * FIELD;
  int nmax_x = max_x + field_x;
  int nmin_x = min_x - field_x;
  int nmax_y = max_y + field_y;
  int nmin_y = min_y - field_y;

  double x_k = (double)ui->graphicsView->width() / (nmax_x - nmin_x);
  double x_b = -x_k * nmin_x;
  double y_k = (double)ui->graphicsView->height() / (nmax_y - nmin_y);
  double y_b = -y_k * nmin_y;

  *res_x = x * x_k + x_b;
  *res_y = y * y_k + y_b;
}

void MainWindow::drawAxis(QPainter *p) {
  p->setBrush(QColor(0, 0, 255));
  p->setPen(QColor(0, 0, 255));

  int real_x, real_y;
  from_abs_coordinates(0, 0, &real_x, &real_y);

  int height = ui->graphicsView->height() - 2;
  int width = ui->graphicsView->width() - 2;

  p->drawLine(real_x, 0, real_x, height);
  p->drawLine(real_x, height, real_x - ARROW, height - ARROW);
  p->drawLine(real_x, height, real_x + ARROW, height - ARROW);

  real_y -= ui->menubar->height();
  p->drawLine(0, real_y, width, real_y);
  p->drawLine(width, real_y, width - ARROW, real_y - ARROW);
  p->drawLine(width, real_y, width - ARROW, real_y + ARROW);
}

void MainWindow::on_cleanbutton_clicked() {
  lastevents.push_back(CLEAN_EVEN);
  dup_polygons = polygons;
  ui->lineEdit->setText("");
  ui->table->clear();
  int cnt_points = count_all_points(polygons);
  for (int i = cnt_points; i >= 0; i--) {
    ui->table->removeRow(i);
    ui->table->clearSelection();
  }
  QTableWidgetItem *x = new QTableWidgetItem;
  QTableWidgetItem *y = new QTableWidgetItem;
  QTableWidgetItem *num = new QTableWidgetItem;
  x->setText("x");
  y->setText("y");
  num->setText("№ N-уг-ка");
  ui->table->setHorizontalHeaderItem(0, x);
  ui->table->setHorizontalHeaderItem(1, y);
  ui->table->setHorizontalHeaderItem(2, num);
  int cnt = polygons.size();
  for (int i = 0; i < cnt; i++) {
    int k = polygons[i].size();
    for (int j = 0; j < k; j++)
      polygons[i].pop_back();
    polygons.pop_back();
  }
  min_x = 0;
  min_y = 0;
  max_x = 100;
  max_y = 100;
  drawPoints();
}

void MainWindow::on_drawbutton_clicked() {
  if (polygons.empty() || polygons[polygons.size() - 1].size() <= 2) {
    ui->lineEdit->setText("Ошибка! Недостаточно точек для данного N-уголника.");
    return;
  }
  is_draw_polygon = 1;
  drawPoints();
}

int is_similarity_matr(std::vector<std::vector<double>> a,
                       std::vector<std::vector<double>> b) {
  double k = 0;
  for (size_t i = 0; i < a.size(); i++)
    for (size_t j = 0; j < a.size(); j++) {
      if (a[i][j] > EPS || a[i][j] < -EPS) {
        k = b[i][j] / a[i][j];
        break;
      }
    }

  if (abs(k) < EPS)
    return 0;
  for (size_t i = 0; i < a.size(); i++)
    for (size_t j = 0; j < a.size(); j++)
      if (abs(a[i][j] * k - b[i][j]) > EPS)
        return 0;
  return 1;
}

void swap_cols(std::vector<std::vector<double>> &x, int k, int t) {
  for (size_t i = 0; i < x.size(); i++) {
    double buf = x[i][k];
    x[i][k] = x[i][t];
    x[i][t] = buf;
  }
}

void swap_rows(std::vector<std::vector<double>> &x, int k, int t) {
  swap(x[k], x[t]);
}

void swap_els(int *a, int i, int j, std::vector<std::vector<double>> &m2) {
  int s = a[i];
  a[i] = a[j];
  a[j] = s;
  swap_rows(m2, i, j);
  swap_cols(m2, i, j);
}

bool NextSet(int *a, int n, std::vector<std::vector<double>> &m2) {
  if (n < 2)
    return false;
  int j = n - 2;
  if (j < 0)
    return false;
  while (j != -1 && a[j] >= a[j + 1])
    j--;
  if (j == -1)
    return false; // больше перестановок нет
  int k = n - 1;

  while (a[j] >= a[k])
    k--;
  swap_els(a, j, k, m2);
  int l = j + 1, r = n - 1; // сортируем оставшуюся часть последовательности
  while (l < r)
    swap_els(a, l++, r--, m2);
  return true;
}

int is_similarity(std::vector<std::vector<double>> a,
                  std::vector<std::vector<double>> b) {
  std::vector<std::vector<double>> m1 = a; //???
  std::vector<std::vector<double>> m2 = b;
  if (m1.size() != m2.size())
    return 0;
  if (is_similarity_matr(m1, m2))
    return 1;
  int cnt_el = m1.size();
  int *arr = new int[cnt_el];
  for (int i = 0; i < cnt_el; i++)
    arr[i] = i;
  while (NextSet(arr, cnt_el, m2))
    if (cnt_el && is_similarity_matr(m1, m2)) {
      delete[] arr;
      return 1;
    }
  delete[] arr;
  return 0;
}

void MainWindow::on_findbutton_clicked() {
  if (lastevents.back() == DRAW_POINT_EVEN) {
    ui->lineEdit->setText("Недорисован последний многоуголник.");
    return;
  }
  if (polygons.size() < 3) {
    ui->lineEdit->setText("Недостаточно многоуголников.");
    return;
  }
  size_t cnt_points = polygons[polygons.size() - 1].size();
  for (size_t i = 0; i < cnt_points; i++) {
    polygons[polygons.size() - 1].pop_back();
  }
  polygons.pop_back();

  double x1, x2, y1, y2;

  for (size_t i = 0; i < polygons.size(); i++) {
    std::vector<std::vector<double>> matr;
    for (size_t j = 0; j < polygons[i].size(); j++) {
      x1 = polygons[i][j].x();
      y1 = polygons[i][j].y();
      std::vector<double> d;
      for (size_t k = 0; k < polygons[i].size(); k++) {
        x2 = polygons[i][k].x();
        y2 = polygons[i][k].y();
        double distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        d.push_back(distance);
      }
      matr.push_back(d);
    }
    matrixes.push_back(matr);
  }

  size_t cnt_n = 0;
  for (size_t i = 0; i + 1 < matrixes.size(); i++) {
    for (size_t j = i + 1; j < matrixes.size(); j++) {
      if (matrixes[i].size() == matrixes[j].size() &&
          is_similarity(matrixes[i], matrixes[j]) &&
          matrixes[i].size() > cnt_n) {
        is_find_polygon = 1;
        cnt_n = matrixes[i].size();
        ind1 = i;
        ind2 = j;
      }
    }
  }
  std::vector<QPoint> c;
  polygons.push_back(c);

  int k_matr = matrixes.size();
  for (int i = k_matr - 1; i >= 0; i--) {
    size_t k_cols = matrixes[i].size();
    for (size_t j = 0; j < k_cols; j++)
      matrixes[i].pop_back();
    matrixes.pop_back();
  }
  if (cnt_n == 0) {
    is_find_polygon = 0;
    ui->lineEdit->setText("Нет подобных многоугольников.");
    return;
  } else
    ui->lineEdit->setText(
        "Подобные многоугольники выделены красной жирной линией.");
  drawPoints();
}

void MainWindow::addPoint(int x, int y, bool is_blocked) {
  bool oldState;
  if (is_blocked)
    oldState = ui->table->blockSignals(true);
  int row = count_all_points(polygons);
  highlighted = -1;
  ui->table->insertRow(row);
  ui->table->setItem(row, 0, new QTableWidgetItem(QString::number(x)));
  ui->table->setItem(row, 1, new QTableWidgetItem(QString::number(y)));

  QTableWidgetItem *num = new QTableWidgetItem;
  num->setText(QString::number((int)(polygons.size())));

  ui->table->setItem(row, 2, num);
  if (x > max_x)
    max_x = x;
  else if (x < min_x)
    min_x = x;

  if (y > max_y)
    max_y = y;
  else if (y < min_y)
    min_y = y;
  if (polygons.size() == 0) {
    std::vector<QPoint> c;
    polygons.push_back(c);
  }
  polygons[polygons.size() - 1].push_back(QPoint(x, y));
  drawPoints();
  lastevents.push_back(DRAW_POINT_EVEN);
  if (is_blocked)
    ui->table->blockSignals(oldState);
}

void MainWindow::on_addButton_clicked() {
  drawPoints();
  QString x_str = ui->lineX->text();
  QString y_str = ui->lineY->text();

  bool x_flag, y_flag;
  int x = x_str.toInt(&x_flag);
  int y = y_str.toInt(&y_flag);
  if (!x_flag || !y_flag) {
    ui->lineEdit->setText("Ошибка! Введено некорректное число!");
    ui->lineX->clear();
    ui->lineY->clear();
    return;
  }

  addPoint(x, y, true);
  ui->lineY->clear();
  ui->lineX->clear();
}

void MainWindow::on_deletebutton_clicked() {
  if (polygons.empty() || polygons[0].empty()) {
    ui->lineEdit->setText("Ошибка! Нет точек для удаления!");
    return;
  }
  drawPoints();
  if (highlighted == -1) {
    ui->lineEdit->setText("Ошибка! Не выделена точка!");
    return;
  }

  int sum = 0;
  for (int i = 0; i < highlight_num_vec; i++) {
    sum += polygons[i].size();
  }
  int num_del_point = highlighted - sum;

  del_points.push_back(polygons[highlight_num_vec][num_del_point]);

  polygons[highlight_num_vec].erase(polygons[highlight_num_vec].begin() +
                                    num_del_point);
  if (highlight_num_vec + 1 < (int)polygons.size() &&
      polygons[highlight_num_vec].empty()) {
    polygons.erase(polygons.begin() + highlight_num_vec);
  }

  ui->table->removeRow(highlighted);
  ui->table->clearSelection();
  highlighted = -1;

  drawPoints();
  lastevents.push_back(DEL_POINT_EVEN);
}
