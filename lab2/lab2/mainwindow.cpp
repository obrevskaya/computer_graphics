#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QtMath>
#include <iostream>
#include <math.h>
#include <vector>

#define CNT_POINTS_BODY 7
#define TO_RAD (4 * atan(1) / 180.0)
#define ARROW 5
#define MIN -600
#define MAX 600
#define FIELD ((double)5 / 100)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QGraphicsScene *scene = new QGraphicsScene();
  ui->graphicsView->setScene(scene);
  ui->graphicsView->scene()->clear();
  min_x = min_y = MIN;
  max_x = max_y = MAX;
  createCat();
  drawPic();
}

MainWindow::~MainWindow() {
  delete ui->graphicsView->scene();
  delete ui;
}

void MainWindow::showEvent(QShowEvent *ev) {
  QMainWindow::showEvent(ev);
  drawPic();
}

void MainWindow::on_action_triggered() {
  QMessageBox::about(this, "ПРОГРАММА",
                     "Рисует исходный рисунок кота, затем его перемещает, "
                     "масштабирует, поворачивает.");
}

void MainWindow::on_action_2_triggered() {
  QMessageBox::about(this, "АВТОР", "Обревская Вероника ИУ7-42Б");
}

void MainWindow::on_action_4_triggered() { QApplication::quit(); }

void MainWindow::initPoints() {
  cat.points.push_back(QPoint(200, -450));
  cat.points.push_back(QPoint(300, 415));
  cat.points.push_back(QPoint(-300, 415));
  cat.points.push_back(QPoint(-200, -450));
  cat.points.push_back(QPoint(-80, -260));
  cat.points.push_back(QPoint(0, -265));
  cat.points.push_back(QPoint(80, -260));

  cat.points.push_back(QPoint(0, 93));
  cat.points.push_back(QPoint(0, -20));

  cat.points.push_back(QPoint(20, -40));
  cat.points.push_back(QPoint(290, -20));
  cat.points.push_back(QPoint(290, 0));
  cat.points.push_back(QPoint(290, 20));

  cat.points.push_back(QPoint(-20, -40));
  cat.points.push_back(QPoint(-290, -20));
  cat.points.push_back(QPoint(-290, 0));
  cat.points.push_back(QPoint(-290, 20));

  cat.points.push_back(QPoint(380, 390));
  cat.points.push_back(QPoint(350, 330));

  cat.points.push_back(QPoint(420, 390));
  cat.points.push_back(QPoint(390, 330));

  cat.points.push_back(QPoint(-380, 390));
  cat.points.push_back(QPoint(-350, 330));

  cat.points.push_back(QPoint(-420, 390));
  cat.points.push_back(QPoint(-390, 330));

  cat.points.push_back(QPoint(90, 400));
  cat.points.push_back(QPoint(85, 370));

  cat.points.push_back(QPoint(120, 400));
  cat.points.push_back(QPoint(115, 370));

  cat.points.push_back(QPoint(-90, 400));
  cat.points.push_back(QPoint(-85, 370));

  cat.points.push_back(QPoint(-120, 400));
  cat.points.push_back(QPoint(-115, 370));
  // НОГИ Большие
  cat.points.push_back(QPoint(300, 395));
  cat.points.push_back(QPoint(485, 395));
  cat.points.push_back(QPoint(455, 335));
  cat.points.push_back(QPoint(420, 310));
  cat.points.push_back(QPoint(400, 300));
  cat.points.push_back(QPoint(380, 295));
  cat.points.push_back(QPoint(350, 290));
  cat.points.push_back(QPoint(320, 290));
  cat.points.push_back(QPoint(295, 295));

  cat.points.push_back(QPoint(-300, 395));
  cat.points.push_back(QPoint(-485, 395));
  cat.points.push_back(QPoint(-455, 335));
  cat.points.push_back(QPoint(-420, 310));
  cat.points.push_back(QPoint(-400, 300));
  cat.points.push_back(QPoint(-380, 295));
  cat.points.push_back(QPoint(-350, 290));
  cat.points.push_back(QPoint(-320, 290));
  cat.points.push_back(QPoint(-295, 295));
  // НОГИ Мал
  cat.points.push_back(QPoint(20, 405));
  cat.points.push_back(QPoint(180, 405));
  cat.points.push_back(QPoint(160, 375));
  cat.points.push_back(QPoint(127, 350));
  cat.points.push_back(QPoint(100, 343));
  cat.points.push_back(QPoint(77, 350));
  cat.points.push_back(QPoint(45, 375));
  cat.points.push_back(QPoint(20, 405));

  cat.points.push_back(QPoint(-20, 405));
  cat.points.push_back(QPoint(-180, 405));
  cat.points.push_back(QPoint(-160, 375));
  cat.points.push_back(QPoint(-127, 350));
  cat.points.push_back(QPoint(-100, 343));
  cat.points.push_back(QPoint(-77, 350));
  cat.points.push_back(QPoint(-45, 375));
  cat.points.push_back(QPoint(-20, 405));

  //РОТ
  cat.points.push_back(QPoint(0, 93));
  cat.points.push_back(QPoint(100, 75));
  cat.points.push_back(QPoint(170, 45));

  cat.points.push_back(QPoint(-100, 75));
  cat.points.push_back(QPoint(-170, 45));
}

void MainWindow::createCat() {
  initPoints();

  cat.nose = {.center = QPoint(0, -40), .r1 = 10, .r2 = 10};
  cat.r_eye = {.center = QPoint(75, -110), .r1 = 40, .r2 = 40};
  cat.l_eye = {.center = QPoint(-75, -110), .r1 = 40, .r2 = 40};
  cat.r_pupil = {.center = QPoint(45, -110), .r1 = 7, .r2 = 7};
  cat.l_pupil = {.center = QPoint(-45, -110), .r1 = 7, .r2 = 7};

  cat.lines = {
      {1, 2},   {2, 3},   {3, 4},   {4, 5},   {5, 6},   {6, 7},   {7, 1},
      {8, 9},   {10, 11}, {10, 12}, {10, 13}, {14, 15}, {14, 16}, {14, 17},
      {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 31},
      {32, 33}, {34, 35}, {35, 36}, {36, 37}, {37, 38}, {38, 39}, {39, 40},
      {40, 41}, {41, 42}, {43, 44}, {44, 45}, {45, 46}, {46, 47}, {47, 48},
      {48, 49}, {49, 50}, {50, 51}, {52, 53}, {53, 54}, {54, 55}, {55, 56},
      {56, 57}, {57, 58}, {58, 59}, {60, 61}, {61, 62}, {62, 63}, {63, 64},
      {64, 65}, {65, 66}, {66, 67}, {68, 69}, {69, 70}, {68, 71}, {71, 72}};
}

void MainWindow::draw_circle(QPainter *p, circle_t circle) {
  int real_x, real_y;
  from_abs_coordinates(circle.center.x(), circle.center.y(), &real_x, &real_y);
  ui->graphicsView->scene()->addEllipse(real_x - circle.r1, real_y - circle.r2,
                                        2 * circle.r1, 2 * circle.r2,
                                        QPen(Qt::black, 5, Qt::SolidLine));
  p->drawEllipse(real_x - circle.r1, real_y - circle.r2, 2 * circle.r1,
                 2 * circle.r2);
}

void MainWindow::drawPic() {
  QImage image =
      QImage(ui->graphicsView->geometry().width(),
             ui->graphicsView->geometry().height(), QImage::Format_RGB32);
  QPainter p(&image);
  image.fill(QColor(255, 255, 255));

  int x, y;
  from_abs_coordinates(abs_x, abs_y, &x, &y);

  if (is_center_scale) {
    p.setPen(QColor(QColor(0, 250, 0)));
    p.setBrush(QColor(0, 250, 0));
    p.drawEllipse(x, y, r, r);
  }

  if (is_center_rotate) {
    p.setPen(QColor(QColor(250, 0, 0)));
    p.setBrush(QColor(250, 0, 0));
    p.drawEllipse(x, y, r, r);
  }

  p.setPen(QPen(QColor(0, 0, 0), 5));
  p.setBrush(QColor(0, 0, 0));

  draw_circle(&p, cat.r_pupil);
  draw_circle(&p, cat.l_pupil);

  p.setBrush(QColor(0, 0, 0, 0));
  draw_circle(&p, cat.nose);
  draw_circle(&p, cat.r_eye);
  draw_circle(&p, cat.l_eye);

  drawAxis(&p);

  p.setPen(QPen(QColor(0, 0, 0), 5));
  for (int i = 0; i < cat.lines.size(); i++) {
    int x1 = cat.points[cat.lines[i].p1 - 1].x();
    int y1 = cat.points[cat.lines[i].p1 - 1].y();

    int x2 = cat.points[cat.lines[i].p2 - 1].x();
    int y2 = cat.points[cat.lines[i].p2 - 1].y();

    int real_x1, real_y1;
    from_abs_coordinates(x1, y1, &real_x1, &real_y1);

    int real_x2, real_y2;
    from_abs_coordinates(x2, y2, &real_x2, &real_y2);

    p.drawLine(real_x1, real_y1, real_x2, real_y2);
  }

  QPixmap pixmap = QPixmap::fromImage(image);
  ui->graphicsView->scene()->clear();
  ui->graphicsView->scene()->addPixmap(pixmap);
}

void MainWindow::drawAxis(QPainter *p) {
  p->setBrush(QColor(0, 0, 250));
  p->setPen(QColor(0, 0, 250));

  int real_x, real_y;
  from_abs_coordinates(0, 0, &real_x, &real_y);

  int height = ui->graphicsView->height() - 2;
  int width = ui->graphicsView->width() - 2;

  // Oy
  p->drawLine(real_x, 0, real_x, height);
  p->drawLine(real_x, height, real_x - ARROW, height - ARROW);
  p->drawLine(real_x, height, real_x + ARROW, height - ARROW);

  // Ox
  p->drawLine(0, real_y, width, real_y);
  p->drawLine(width, real_y, width - ARROW, real_y - ARROW);
  p->drawLine(width, real_y, width - ARROW, real_y + ARROW);
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

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  drawPic();
}

void MainWindow::on_movebutton_clicked() {
  ui->lineEdit->setText(" ");
  drawPic();
  QString x_str = ui->move_dx->text();
  QString y_str = ui->move_dy->text();

  bool x_flag, y_flag;
  int dx = x_str.toInt(&x_flag);
  int dy = y_str.toInt(&y_flag);
  if (!x_flag || !y_flag) {
    ui->lineEdit->setText(
        "ОШИБКА!!! Введено некорректное число для перемещения!");
    ui->move_dx->clear();
    ui->move_dx->clear();
    return;
  }

  event_t new_event = {.event = EVENT_MOVE};
  new_event.parameters.move.dx = dx;
  new_event.parameters.move.dy = dy;
  lastevents.push_back(new_event);

  for (int i = 0; i < cat.points.size(); i++) {
    cat.points[i].setX(cat.points[i].x() + dx);
    cat.points[i].setY(cat.points[i].y() + dy);
  }

  cat.nose.center.setX(cat.nose.center.x() + dx);
  cat.nose.center.setY(cat.nose.center.y() + dy);

  cat.l_eye.center.setX(cat.l_eye.center.x() + dx);
  cat.l_eye.center.setY(cat.l_eye.center.y() + dy);

  cat.l_pupil.center.setX(cat.l_pupil.center.x() + dx);
  cat.l_pupil.center.setY(cat.l_pupil.center.y() + dy);

  cat.r_eye.center.setX(cat.r_eye.center.x() + dx);
  cat.r_eye.center.setY(cat.r_eye.center.y() + dy);

  cat.r_pupil.center.setX(cat.r_pupil.center.x() + dx);
  cat.r_pupil.center.setY(cat.r_pupil.center.y() + dy);

  drawPic();

  ui->move_dx->clear();
  ui->move_dy->clear();
  ui->lineEdit->setText("Перемещение выполнено!");
}

void MainWindow::scale(int cx, int cy, double kx, double ky) {
  for (int i = 0; i < cat.points.size(); i++) {
    int x = cat.points[i].x();
    int y = cat.points[i].y();
    double x1 = kx * x + (1 - kx) * cx;
    double y1 = ky * y + (1 - ky) * cy;
    cat.points[i].setX(x1);
    cat.points[i].setY(y1);
  }

  cat.nose.center.setX(kx * cat.nose.center.x() + (1 - kx) * cx);
  cat.nose.center.setY(ky * cat.nose.center.y() + (1 - ky) * cy);
  cat.nose.r1 *= kx;
  cat.nose.r2 *= ky;

  cat.l_eye.center.setX(kx * cat.l_eye.center.x() + (1 - kx) * cx);
  cat.l_eye.center.setY(ky * cat.l_eye.center.y() + (1 - ky) * cy);
  cat.l_eye.r1 *= kx;
  cat.l_eye.r2 *= ky;

  cat.l_pupil.center.setX(kx * cat.l_pupil.center.x() + (1 - kx) * cx);
  cat.l_pupil.center.setY(ky * cat.l_pupil.center.y() + (1 - ky) * cy);
  cat.l_pupil.r1 *= kx;
  cat.l_pupil.r2 *= ky;

  cat.r_eye.center.setX(kx * cat.r_eye.center.x() + (1 - kx) * cx);
  cat.r_eye.center.setY(ky * cat.r_eye.center.y() + (1 - ky) * cy);
  cat.r_eye.r1 *= kx;
  cat.r_eye.r2 *= ky;

  cat.r_pupil.center.setX(kx * cat.r_pupil.center.x() + (1 - kx) * cx);
  cat.r_pupil.center.setY(ky * cat.r_pupil.center.y() + (1 - ky) * cy);
  cat.r_pupil.r1 *= kx;
  cat.r_pupil.r2 *= ky;

  drawPic();
}

void MainWindow::on_scaleButton_clicked() {
  ui->lineEdit->setText(" ");
  drawPic();
  QString cx_str = ui->scale_x->text();
  QString cy_str = ui->scale_y->text();
  QString kx_str = ui->scale_kx->text();
  QString ky_str = ui->scale_ky->text();

  bool x_flag, y_flag, kx_flag, ky_flag;
  int cx = cx_str.toInt(&x_flag);
  int cy = cy_str.toInt(&y_flag);
  double kx = kx_str.toDouble(&kx_flag);
  double ky = ky_str.toDouble(&ky_flag);
  if (!x_flag || !y_flag || !kx_flag || !ky_flag || kx == 0 || ky == 0) {
    ui->lineEdit->setText(
        "ОШИБКА!!! Введено некорректное число для масштабирования!");
    ui->scale_x->clear();
    ui->scale_y->clear();
    ui->scale_kx->clear();
    ui->scale_ky->clear();
    return;
  }

  event_t new_event = {.event = EVENT_SCALE};
  new_event.parameters.scale.cx = cx;
  new_event.parameters.scale.cy = cy;
  new_event.parameters.scale.kx = kx;
  new_event.parameters.scale.ky = ky;
  lastevents.push_back(new_event);

  scale(cx, cy, kx, ky);
  ui->scale_x->clear();
  ui->scale_y->clear();
  ui->scale_kx->clear();
  ui->scale_ky->clear();
  ui->lineEdit->setText("Масштабирование выполнено!");
}

void MainWindow::rotation_for_circle(circle_t *circle, int cx, int cy,
                                     double rad) {
  int x = circle->center.x();
  int y = circle->center.y();
  circle->center = QPoint(cx + (x - cx) * cos(rad) + (y - cy) * sin(rad),
                          cy - (x - cx) * sin(rad) + (y - cy) * cos(rad));
}

void MainWindow::rotation(int cx, int cy, int angle) {
  double rad = (double)angle * TO_RAD;
  for (int i = 0; i < cat.points.size(); i++) {
    int x = cat.points[i].x();
    int y = cat.points[i].y();
    double x1 = cx + (double)(x - cx) * cos(rad) + (y - cy) * sin(rad);
    double y1 = cy - (double)(x - cx) * sin(rad) + (y - cy) * cos(rad);
    cat.points[i].setX(x1);
    cat.points[i].setY(y1);
  }

  rotation_for_circle(&(cat.nose), cx, cy, rad);
  rotation_for_circle(&(cat.l_eye), cx, cy, rad);
  rotation_for_circle(&(cat.l_pupil), cx, cy, rad);
  rotation_for_circle(&(cat.r_eye), cx, cy, rad);
  rotation_for_circle(&(cat.r_pupil), cx, cy, rad);

  drawPic();
}

void MainWindow::on_rotationButton_clicked() {
  drawPic();
  QString cx_str = ui->rotation_x->text();
  QString cy_str = ui->rotation_y->text();
  QString angle_str = ui->rotation_angle->text();

  bool x_flag, y_flag, angle_flag;
  int cx = cx_str.toInt(&x_flag);
  int cy = cy_str.toInt(&y_flag);
  int angle = angle_str.toInt(&angle_flag);
  if (!x_flag || !y_flag || !angle_flag) {
    ui->rotation_x->clear();
    ui->rotation_y->clear();
    ui->rotation_angle->clear();
    ui->lineEdit->setText("ОШИБКА!!! Введено некорректное число для поворота!");
    return;
  }
  if (angle % 360 == 0) {
    ui->rotation_x->clear();
    ui->rotation_y->clear();
    ui->rotation_angle->clear();
    ui->lineEdit->setText("Поворот выполнен!");
    return;
  }

  event_t new_event = {.event = EVENT_ROTATION};
  new_event.parameters.rotate.cx = cx;
  new_event.parameters.rotate.cy = cy;
  new_event.parameters.rotate.angle = angle;
  lastevents.push_back(new_event);

  rotation(cx, cy, angle);
  ui->rotation_x->clear();
  ui->rotation_y->clear();
  ui->rotation_angle->clear();
  ui->lineEdit->setText("Поворот выполнен!");
}

void MainWindow::on_cleanButton_clicked() {
  ui->lineEdit->setText(" ");
  for (int i = 0; i < cat.points.size(); i++)
    deleted.push_back(cat.points[i]);
  cat.points.clear();
  initPoints();
  cat.nose = {.center = QPoint(0, -40), .r1 = 10, .r2 = 10};
  cat.r_eye = {.center = QPoint(75, -110), .r1 = 40, .r2 = 40};
  cat.l_eye = {.center = QPoint(-75, -110), .r1 = 40, .r2 = 40};
  cat.r_pupil = {.center = QPoint(45, -110), .r1 = 7, .r2 = 7};
  cat.l_pupil = {.center = QPoint(-45, -110), .r1 = 7, .r2 = 7};

  drawPic();

  event_t new_event = {.event = EVENT_RESET};
  lastevents.push_back(new_event);
}

void MainWindow::on_backButton_clicked() {
  if (lastevents.empty()) {
    ui->lineEdit->setText("Нечего отменять.");
    return;
  }

  event_t lastevent = lastevents.back();

  if (lastevent.event == EVENT_MOVE) {
    int dx = -lastevent.parameters.move.dx;
    int dy = -lastevent.parameters.move.dy;
    move(dx, dy);
  } else if (lastevent.event == EVENT_SCALE) {
    int cx = lastevent.parameters.scale.cx;
    int cy = lastevent.parameters.scale.cy;
    double kx = 1 / lastevent.parameters.scale.kx;
    double ky = 1 / lastevent.parameters.scale.ky;
    scale(cx, cy, kx, ky);
  } else if (lastevent.event == EVENT_ROTATION) {
    int cx = lastevent.parameters.rotate.cx;
    int cy = lastevent.parameters.rotate.cy;
    int angle = -lastevent.parameters.rotate.angle;
    rotation(cx, cy, angle);
  } else if (lastevent.event == EVENT_RESET) {
    if (deleted.empty())
      return;

    int i = lastevents.size() - 2;
    while (i >= 0 && lastevents[i].event != EVENT_RESET)
      i--;
    for (int j = i + 1;
         j < lastevents.size() && lastevents[j].event != EVENT_RESET; j++) {
      lastevent = lastevents[j];
      if (lastevent.event == EVENT_MOVE) {
        int dx = lastevent.parameters.move.dx;
        int dy = lastevent.parameters.move.dy;
        move(dx, dy);
      } else if (lastevent.event == EVENT_SCALE) {
        int cx = lastevent.parameters.scale.cx;
        int cy = lastevent.parameters.scale.cy;
        double kx = lastevent.parameters.scale.kx;
        double ky = lastevent.parameters.scale.ky;
        scale(cx, cy, kx, ky);
      } else if (lastevent.event == EVENT_ROTATION) {
        int cx = lastevent.parameters.rotate.cx;
        int cy = lastevent.parameters.rotate.cy;
        int angle = lastevent.parameters.rotate.angle;
        rotation(cx, cy, angle);
      }
    }
  }
  drawPic();

  if (lastevents.size() != 0)
    lastevents.pop_back();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  if (!is_center_rotate && !is_center_scale)
    return;

  int canv_x = event->pos().x() - ui->graphicsView->x();
  int canv_y = event->pos().y() - ui->graphicsView->y() -
               ui->menubar->geometry().height();

  int rad = 10;
  to_abs_coordinates(canv_x, canv_y, &abs_x, &abs_y);

  if (canv_x < 0 || canv_y < 0 || canv_x > ui->graphicsView->width() ||
      canv_y > ui->graphicsView->height()) {
    is_center_rotate = false;
    is_center_scale = false;
    return;
  }

  if (is_center_rotate) {
    ui->rotation_x->setText(QString::number(abs_x));
    ui->rotation_y->setText(QString::number(abs_y));
  }

  if (is_center_scale) {
    ui->scale_x->setText(QString::number(abs_x));
    ui->scale_y->setText(QString::number(abs_y));
  }
  drawPic();
  is_center_rotate = false;
  is_center_scale = false;
}

void MainWindow::on_choose_center_rotate_clicked() { is_center_rotate = true; }

void MainWindow::on_choose_center_scale_clicked() { is_center_scale = true; }
