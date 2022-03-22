#include "mainwindow.h"
#include "QColorDialog"
#include "QMessageBox"
#include "drawing.h"
#include "tasks.h"
#include "states.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <cstdlib>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  scene = new QGraphicsScene(this);
  ui->graphicsView->setScene(scene);
  ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  ui->graphicsView->setRenderHint(QPainter::Antialiasing);

  drawer = new drawer_t(scene);

  fg = new color_view(this);
  QRect fgRect = ui->lineColorLabel->geometry();
  fgRect.setY(fgRect.y() + 10);
  fg->setGeometry(fgRect);
  fg->set_color(QColor(0, 0, 0));
  bg = new color_view(this);
  bg->setGeometry(ui->bgColorLabel->geometry());

  connect(ui->progInfo, SIGNAL(triggered()), this, SLOT(progInfo_messagebox()));
  connect(ui->authorInfo, SIGNAL(triggered()), this,
          SLOT(authorInfo_messagebox()));
  connect(ui->exit, SIGNAL(triggered()), this, SLOT(exit_messagebox()));
}

MainWindow::~MainWindow() {
  delete ui;
  delete scene;
  delete fg;
  delete bg;
}

void MainWindow::progInfo_messagebox() {
  QMessageBox::about(this, "ПРОГРАММА",
                     "Реализует и исследует алгоритмы построения отрезков");
}

void MainWindow::authorInfo_messagebox() {
  QMessageBox::about(this, "АВТОР", "Обревская Вероника ИУ7-42Б");
}

void MainWindow::exit_messagebox() { QApplication::quit(); }

void MainWindow::error_messagebox(QString text) {

  QMessageBox::critical(this, "Ошибка", text);
}

void MainWindow::on_lineColorButton_clicked() {
  QColorDialog dialog;
  dialog.setCurrentColor(fg->get_color());
  dialog.show();
  dialog.exec();
  QColor color = dialog.selectedColor();
  if (!color.isValid())
    return;
  fg->set_color(color);
}

void MainWindow::on_bgColorButton_clicked() {
  QColorDialog dialog;
  dialog.setCurrentColor(bg->get_color());
  dialog.show();
  dialog.exec();
  QColor color = dialog.selectedColor();
  if (!color.isValid())
    return;
  bg->set_color(color);
}

void MainWindow::on_drawLineButton_clicked() {
  point_t p1 = point_t{ui->xBegin->value(), ui->yBegin->value()};
  point_t p2 = point_t{ui->xEnd->value(), ui->yEnd->value()};

  if (p1.x == p2.x && p1.y == p2.y) {
    error_messagebox("Начальные и конечные координаты совпадают.");
    return;
  }

  ui->undoButton->setEnabled(true);
  states_push(states, dataset);

  line_t line = line_t{.p1 = p1, .p2 = p2};

  int method_i = ui->methodSelect->currentIndex();

  params_t params =
      params_t{.command = DRAW_LINE, .method = DDA + method_i, .line = line};

  QColor qcolor = fg->get_color();
  color_t color;
  qcolor.getRgb(&color.r, &color.g, &color.b);

  QBrush bg_brush(bg->get_color());
  scene->setBackgroundBrush(bg_brush);
  request(*drawer, params).handle(color);
  data_t data = {
      .is_line = true, .line = line, .method = DDA + method_i, .color = color};
  dataset.push_back(data);
}

void MainWindow::on_drawSpectrumButton_clicked() {
  if (ui->spectrumR->value() <= 0) {
    error_messagebox("Радиус спектра меньше или равен нулю.");
    return;
  }
  if (ui->spectrumStep->value() <= 0 || ui->spectrumStep->value() >= 360) {
    error_messagebox("Неверно задан угол.");
    return;
  }
  ui->undoButton->setEnabled(true);
  states_push(states, dataset);

  spectrum_t spectrum =
      spectrum_t{.center = {ui->spectrumX->value(), ui->spectrumY->value()},
                 .r = ui->spectrumR->value(),
                 .step = ui->spectrumStep->value()};

  int method_i = ui->methodSelect->currentIndex();

  params_t params = params_t{
      .command = DRAW_SPECTRUM, .method = DDA + method_i, .spectrum = spectrum};

  QColor qcolor = fg->get_color();
  color_t color;
  qcolor.getRgb(&color.r, &color.g, &color.b);
  QBrush bg_brush(bg->get_color());
  scene->setBackgroundBrush(bg_brush);
  request(*drawer, params).handle(color);
  data_t data = {.is_line = false,
                 .spectrum = spectrum,
                 .method = DDA + method_i,
                 .color = color};
  dataset.push_back(data);
}

void MainWindow::on_clearButton_clicked() {
  ui->undoButton->setEnabled(true);
  states_push(states, dataset);
  request(*drawer, {.command = CLEAR_SCREEN}).handle({});
  dataset.clear();
}

void MainWindow::on_compare_timeButton_clicked() {
  double r = ui->spectrumR->value();
  if (r <= 0) {
    error_messagebox("Радиус спектра меньше или равен нулю.");
    return;
  }
  if (ui->spectrumStep->value() <= 0 || ui->spectrumStep->value() >= 360) {
    error_messagebox("Неверно задан угол.");
    return;
  }

  auto time = std::vector<double>();

  spectrum_t spectrum =
      spectrum_t{.center = {}, .r = r, .step = ui->spectrumStep->value()};
  research_spectrum_t r_spectrum{.spectrum = spectrum, .time = &time};
  params_t params = params_t{.command = MEASURE_TIME, .r_spectrum = r_spectrum};

  request(*drawer, params).handle({}, false);

  std::ofstream out;
  out.open("../lab3/time_res.txt");
  if (out.is_open()) {
    out << r << "\n";
    for (auto &i : time)
      out << i << "\n";
  }
  out.close();
  system("python ../lab3/time.py");
}

inline double to_radians(double angle) { return angle * M_PI / 180; }

void MainWindow::measure_steps(method_t method, std::vector<size_t> &steps,
                               double r, double step) {

  params_t params =
      params_t{.command = DRAW_LINE_WITH_COUNT_STEPS, .method = method};

  point_t center = point_t{ui->graphicsView->width() / 2.0,
                           ui->graphicsView->height() / 2.0};
  size_t measure_steps;

  for (double teta = 0.0; teta <= 360.0; teta += step) {
    line_t line = line_t{.p1 = center,
                         .p2 = {center.x + r * cos(to_radians(teta)),
                                center.y + r * sin(to_radians(teta))}};
    params.line = line;
    measure_steps = request(*drawer, params).handle({}, false);
    steps.push_back(measure_steps);
  }
}

void MainWindow::on_compare_stepsButton_clicked() {
  double r = ui->spectrumR->value();
  if (r <= 0) {
    error_messagebox("Радиус спектра меньше или равен нулю.");
    return;
  }
  if (ui->spectrumStep->value() <= 0 || ui->spectrumStep->value() >= 360) {
    error_messagebox("Неверно задан угол.");
    return;
  }

  auto steps = std::vector<size_t>();

  for (int i = DDA; i < LIBRARY; i++)
    measure_steps((method_t)i, steps, ui->spectrumR->value(),
                  ui->spectrumStep->value());

  double max_angle =
      ui->spectrumStep->value() * std::ceil(360.0 / ui->spectrumStep->value());

  std::ofstream out;
  out.open("../lab3/steps_res.txt");
  if (out.is_open()) {
    out << r << "\n";
    out << ui->spectrumStep->value() << "\n";
    out << max_angle << "\n";
    for (auto &i : steps)
      out << i << "\n";
  }
  out.close();
  system("python ../lab3/steps.py");
}

void MainWindow::on_undoButton_clicked() {
  std::vector<data_t> state = states_pop(states);
  dataset_set(state, *drawer);
  dataset = state;
  if (states.empty())
    ui->undoButton->setEnabled(false);
}

void MainWindow::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QRect fgRect = ui->lineColorLabel->geometry();
  fgRect.setRect(fgRect.x(), fgRect.y() + 22, fgRect.width(), fgRect.height());
  fg->setGeometry(fgRect);
  QRect bgRect = ui->bgColorLabel->geometry();
  bgRect.setRect(bgRect.x(), bgRect.y() + 22, bgRect.width(), bgRect.height());
  bg->setGeometry(bgRect);
  auto rcontent = ui->graphicsView->contentsRect();
  ui->graphicsView->scene()->setSceneRect(0, 0, rcontent.width(),
                                          rcontent.height());
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
}

void MainWindow::wheelEvent(QWheelEvent *event) {
  ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  double scale_factor = 1.15;

  if (event->angleDelta().y() > 0)
    ui->graphicsView->scale(scale_factor, scale_factor);
  else
    ui->graphicsView->scale(1 / scale_factor, 1 / scale_factor);
}
