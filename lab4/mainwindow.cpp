#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QMouseEvent>
#include <fstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  view.setView(ui->graphicsView);
  ui->graphicsView->viewport()->installEventFilter(this);
  ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
  tick();
  setBackgroundColor(model.getBgColor());
  setLineColor(model.getLineColor());
}

void MainWindow::tick(bool x) {
  view.draw(x, model.getCircles(), model.getEllipses(), model.getBgColor());
}

bool MainWindow::get_var(double &var, const QLineEdit *lineEdit,
                         const QString &err_msg) {
  bool ok;
  var = lineEdit->text().toDouble(&ok);
  if (!ok)
    error_messagebox(err_msg);
  return ok;
}

bool MainWindow::get_positive_var(double &var, const QLineEdit *lineEdit,
                                  const QString &err_msg) {
  bool ok;
  ok = get_var(var, lineEdit, err_msg);
  if (ok)
    ok = var > 0.0;
  if (!ok)
    error_messagebox(err_msg);
  return ok;
}

void MainWindow::setLineColor(const QColor &color) {
  QString qss = QString("background-color: %1").arg(color.name());
  ui->colorButton->setStyleSheet(qss);
}

void MainWindow::setBackgroundColor(const QColor &color) {
  QString qss = QString("background-color: %1").arg(color.name());
  ui->backgroundColorButton->setStyleSheet(qss);
}

void MainWindow::on_colorButton_clicked() {
  QColor currentColor = Qt::black;
  currentColor = QColorDialog::getColor(currentColor, this, "Choose color: ");

  model.setColor(currentColor);
  setLineColor(currentColor);
}

void MainWindow::on_backgroundColorButton_clicked() {
  QColor currentColor = Qt::black;
  currentColor = QColorDialog::getColor(currentColor, this, "Choose color: ");

  setBackgroundColor(currentColor);
  model.setBgColor(currentColor);

  tick();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {

  if (event->type() == QEvent::Wheel &&
      object == ui->graphicsView->viewport()) {
#if QT_VERSION >= 0x060000
    QWheelEvent *wheel_event = static_cast<QWheelEvent *>(event);
    wheelEvent(wheel_event);
#endif
    return true;
  }
  return false;
}

void MainWindow::wheelEvent(QWheelEvent *event) {
  ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  double scale_factor = 1.15;
  if (event->angleDelta().y() > 0)
    ui->graphicsView->scale(scale_factor, scale_factor);
  else
    ui->graphicsView->scale(1 / scale_factor, 1 / scale_factor);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton &&
      ui->graphicsView->geometry().contains(event->pos()))
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
}

void MainWindow::error_messagebox(QString text) {

  QMessageBox::critical(this, "Ошибка", text);
}

void MainWindow::on_comboBox_activated(int index) {
  if (index == 0)
    model.setMethod(CANONICAL);
  else if (index == 1)
    model.setMethod(PARAMETRIC);
  else if (index == 2)
    model.setMethod(STANDART);
  else if (index == 3)
    model.setMethod(BRESENHAM);
  else if (index == 4)
    model.setMethod(MIDDLE_DOT);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_drawCircleButton_clicked() {
  double x, y, r;
  if (!get_var(x, ui->CenterX, "Некорректное значение x") ||
      !get_var(y, ui->CenterY, "Некорректное значение y") ||
      !get_positive_var(r, ui->circleREdit, "Некорректное значение r"))
    return;

  model.addCircle(QPointF(x, y), r);

  tick(0);
}

void MainWindow::on_drawEllipseButton_clicked() {
  double x, y, a, b;
  if (!get_var(x, ui->CenterX, "Некорректное значение x") ||
      !get_var(y, ui->CenterY, "Некорректное значение y") ||
      !get_positive_var(a, ui->ellipseAEdit, "Некорректное значение a") ||
      !get_positive_var(b, ui->ellipseBEdit, "Некорректное значение b"))
    return;

  model.addEllipse(QPointF(x, y), a, b);

  tick();
}

void MainWindow::on_ellipseSpectr_clicked() {
  double x, y, a, b, da, db, n;
  if (!get_var(x, ui->CenterX, "Некорректное значение x") ||
      !get_var(y, ui->CenterY, "Некорректное значение y") ||
      !get_positive_var(a, ui->ellipseAEdit, "Некорректное значение a") ||
      !get_positive_var(b, ui->ellipseBEdit, "Некорректное значение b") ||
      !get_positive_var(da, ui->ellipseDaEdit, "Некорректное значение db") ||
      !get_positive_var(db, ui->ellipseDbEdit, "Некорректное значение da") ||
      !get_positive_var(n, ui->spectrN, "Некорректное значение n"))
    return;

  model.addEllipses(QPointF(x, y), a, b, da, db, n);

  tick();
}

void MainWindow::on_circleSpectr_clicked() {
  double x, y, r, dr, n;
  if (!get_var(x, ui->CenterX, "Некорректное значение x") ||
      !get_var(y, ui->CenterY, "Некорректное значение y") ||
      !get_positive_var(r, ui->circleREdit, "Некорректное значение r") ||
      !get_positive_var(n, ui->spectrN, "Некорректное значение n") ||
      !get_positive_var(dr, ui->circleDrEdit, "Некорректное значение n"))
    return;

  model.addCircles(QPointF(x, y), r, dr, n);

  tick(0);
}

double MainWindow::drawingTime(Model &testModel) {
  QElapsedTimer t;
  t.start();

  view.draw(0, testModel.getCircles(), testModel.getEllipses(),
            testModel.getBgColor(), 0);

  return t.elapsed();
}

void MainWindow::on_backButton_clicked() {
  model.backState();
  tick();
}

void MainWindow::on_clearButton_clicked() {
  model.clear();
  tick();
}
void MainWindow::on_progInfo_triggered() {
  QMessageBox::about(
      this, "ПРОГРАММА",
      "Реализует и исследует алгоритмы построения окружностей и эллипсов");
}

void MainWindow::on_authorInfo_triggered() {
  QMessageBox::about(this, "АВТОР", "Обревская Вероника ИУ7-42Б");
}

void MainWindow::on_exit_triggered() { QApplication::quit(); }

void MainWindow::on_speedEllipseButton_clicked() {
  Model testModel;

  int n_t = 500;
  int a_t = 100;
  int b_t = 50;
  int da_t = 10;
  int db_t = 10;

  testModel.setMethod(CANONICAL);
  testModel.addEllipses(QPointF(0, 0), a_t, b_t, da_t, db_t, n_t);
  double canonicalTime = drawingTime(testModel);
  testModel.clear();

  testModel.setMethod(PARAMETRIC);
  testModel.addEllipses(QPointF(0, 0), a_t, b_t, da_t, db_t, n_t);
  double parametricTime = drawingTime(testModel);
  testModel.clear();

  testModel.setMethod(BRESENHAM);
  testModel.addEllipses(QPointF(0, 0), a_t, b_t, da_t, db_t, n_t);
  double bresenhamTime = drawingTime(testModel);
  testModel.clear();

  testModel.setMethod(MIDDLE_DOT);
  testModel.addEllipses(QPointF(0, 0), a_t, b_t, da_t, db_t, n_t);
  double middleDotTime = drawingTime(testModel);
  testModel.clear();

  std::ofstream out;
  out.open("../lab4/time_res_el.txt");
  if (out.is_open()) {
    out << canonicalTime << std::endl;
    out << parametricTime << std::endl;
    out << bresenhamTime << std::endl;
    out << middleDotTime << std::endl;
  }
  out.close();
  system("python ../lab4/test_time_el.py");
}

void MainWindow::on_stepEllipseButton_clicked() {
  Model testModel;
  double x, y, a, b, da, db, n;

  if (!get_var(x, ui->CenterX, "Некорректное значение x") ||
      !get_var(y, ui->CenterY, "Некорректное значение y") ||
      !get_positive_var(a, ui->ellipseAEdit, "Некорректное значение a") ||
      !get_positive_var(b, ui->ellipseBEdit, "Некорректное значение b") ||
      !get_positive_var(n, ui->spectrN, "Некорректное значение n") ||
      !get_positive_var(db, ui->ellipseDbEdit, "Некорректное значение db") ||
      !get_positive_var(da, ui->ellipseDaEdit, "Некорректное значение da"))
    return;

  double a_t = a;
  double b_t = b;
  std::vector<double> canonicalArr;
  for (int i = 0; i < n; i++) {
    testModel.setMethod(CANONICAL);
    testModel.addEllipse(QPointF(x, y), a_t, b_t);
    canonicalArr.push_back(drawingTime(testModel));
    testModel.clear();
    a_t += da;
    b_t += db;
  }

  a_t = a;
  b_t = b;
  std::vector<double> bresenhamArr;
  for (int i = 0; i < n; i++) {
    testModel.setMethod(BRESENHAM);
    testModel.addEllipse(QPointF(x, y), a_t, b_t);
    bresenhamArr.push_back(drawingTime(testModel));
    testModel.clear();
    a_t += da;
    b_t += db;
  }

  a_t = a;
  b_t = b;
  std::vector<double> middleDotArr;
  for (int i = 0; i < n; i++) {
    testModel.setMethod(MIDDLE_DOT);
    testModel.addEllipse(QPointF(x, y), a_t, b_t);
    middleDotArr.push_back(drawingTime(testModel));
    testModel.clear();
    a_t += da;
    b_t += db;
  }

  a_t = a;
  b_t = b;
  std::vector<double> parametricArr;
  for (int i = 0; i < n; i++) {
    testModel.setMethod(PARAMETRIC);
    testModel.addEllipse(QPointF(x, y), a_t, b_t);
    parametricArr.push_back(drawingTime(testModel));
    testModel.clear();
    a_t += da;
    b_t += db;
  }

  std::ofstream out;
  out.open("../lab4/time_r_res_el.txt");
  if (out.is_open()) {
    out << a << std::endl;
    out << b << std::endl;
    out << da << std::endl;
    out << db << std::endl;
    out << n << std::endl;
    for (int i = 0; i < n; i++) {
      out << canonicalArr[i] << std::endl;
    }
    for (int i = 0; i < n; i++) {
      out << parametricArr[i] << std::endl;
    }
    for (int i = 0; i < n; i++) {
      out << bresenhamArr[i] << std::endl;
    }
    for (int i = 0; i < n; i++) {
      out << middleDotArr[i] << std::endl;
    }
  }

  out.close();
  system("python ../lab4/test_r_el.py");
}

void MainWindow::on_speedTestButton_clicked() {
  Model testModel;

  int n_t = 500;
  int r_t = 100;

  testModel.setMethod(CANONICAL);
  testModel.addCircles(QPointF(0, 0), r_t, 10, n_t);
  double canonicalTime = drawingTime(testModel);
  testModel.clear();

  testModel.setMethod(BRESENHAM);
  testModel.addCircles(QPointF(0, 0), r_t, 10, n_t);
  double bresenhamTime = drawingTime(testModel);
  testModel.clear();

  testModel.setMethod(MIDDLE_DOT);
  testModel.addCircles(QPointF(0, 0), r_t, 10, n_t);
  double middleDotTime = drawingTime(testModel);
  testModel.clear();

  testModel.setMethod(PARAMETRIC);
  testModel.addCircles(QPointF(0, 0), r_t, 10, n_t);
  double parametricTime = drawingTime(testModel);
  testModel.clear();

  std::ofstream out;
  out.open("../lab4/time_res.txt");
  if (out.is_open()) {
    out << canonicalTime << std::endl;
    out << parametricTime << std::endl;
    out << bresenhamTime << std::endl;
    out << middleDotTime << std::endl;
  }
  out.close();
  system("python ../lab4/test_time.py");
}

void MainWindow::on_stepTestButton_clicked() {
  Model testModel;
  double x, y, r, dr, n;

  if (!get_var(x, ui->CenterX, "Некорректное значение x") ||
      !get_var(y, ui->CenterY, "Некорректное значение y") ||
      !get_positive_var(r, ui->circleREdit, "Некорректное значение r") ||
      !get_positive_var(n, ui->spectrN, "Некорректное значение n") ||
      !get_positive_var(dr, ui->circleDrEdit, "Некорректное значение dR"))
    return;

  double r_t = r;
  std::vector<double> canonicalArr;
  for (int i = 0; i < n; i++) {
    testModel.setMethod(CANONICAL);
    testModel.addCircle(QPointF(x, y), r_t);
    canonicalArr.push_back(drawingTime(testModel));
    testModel.clear();
    r_t += dr;
  }

  r_t = r;
  std::vector<double> bresenhamArr;
  for (int i = 0; i < n; i++) {
    testModel.setMethod(BRESENHAM);
    testModel.addCircle(QPointF(x, y), r_t);
    bresenhamArr.push_back(drawingTime(testModel));
    testModel.clear();
    r_t += dr;
  }

  r_t = r;
  std::vector<double> middleDotArr;
  for (int i = 0; i < n; i++) {
    testModel.setMethod(MIDDLE_DOT);
    testModel.addCircle(QPointF(x, y), r_t);
    middleDotArr.push_back(drawingTime(testModel));
    testModel.clear();
    r_t += dr;
  }

  r_t = r;
  std::vector<double> parametricArr;
  for (int i = 0; i < n; i++) {
    testModel.setMethod(PARAMETRIC);
    testModel.addCircle(QPointF(x, y), r_t);
    parametricArr.push_back(drawingTime(testModel));
    testModel.clear();
    r_t += dr;
  }

  std::ofstream out;
  out.open("../lab4/time_r_res.txt");
  if (out.is_open()) {
    out << r << std::endl;
    out << dr << std::endl;
    out << n << std::endl;
    for (int i = 0; i < n; i++) {
      out << canonicalArr[i] << std::endl;
    }
    for (int i = 0; i < n; i++) {
      out << parametricArr[i] << std::endl;
    }
    for (int i = 0; i < n; i++) {
      out << bresenhamArr[i] << std::endl;
    }
    for (int i = 0; i < n; i++) {
      out << middleDotArr[i] << std::endl;
    }
  }

  out.close();
  system("python ../lab4/test_r.py");
}
