#include "mainwindow.h"
#include "QColorDialog"
#include "QMessageBox"
#include "color_view.h"
#include "drawer.h"
#include "request.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

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
  bg->set_color(QColor(0, 150, 150));
  bg->setGeometry(ui->brushColorLabel->geometry());

  connect(ui->progInfo, SIGNAL(triggered()), this, SLOT(progInfo_messagebox()));
  connect(ui->authorInfo, SIGNAL(triggered()), this,
          SLOT(authorInfo_messagebox()));
  connect(ui->exit, SIGNAL(triggered()), this, SLOT(exit_messagebox()));

  ui->undoButton->setEnabled(false);

  ui->Delay->setEnabled(false);
  ui->Delaylabel->setText("");
  ui->seedButton->setStyleSheet(
      "QPushButton{ background-color: rgb(32,181,245); color:rgb(240,247,250) "
      "}\n");
  for (int i = 0; i < 10; i++)
    ui->pointsTable->insertRow(ui->pointsTable->rowCount());
  prev = table_create(ui->pointsTable);

  is_seed_inputing = false;
  seed.x = 0;
  seed.y = 0;
}

MainWindow::~MainWindow() {
  delete ui;
  delete scene;
}

void MainWindow::print_message(QString message, QColor color) {
  QPalette palette = ui->timeEdit->palette();
  palette.setColor(ui->timeEdit->foregroundRole(), color);
  ui->timeEdit->setPalette(palette);
  ui->timeEdit->setText(message);
}

void MainWindow::error_messagebox(QString text) {

  QMessageBox::critical(this, "Ошибка", text);
}

void MainWindow::progInfo_messagebox() {
  QMessageBox::about(this, "ПРОГРАММА",
                     "Реализует алгоритм построчного затравочного заполнения "
                     "сплошных областей");
}

void MainWindow::authorInfo_messagebox() {
  QMessageBox::about(this, "АВТОР", "Обревская Вероника ИУ7-42Б");
}

void MainWindow::exit_messagebox() { QApplication::quit(); }

void MainWindow::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QRect fgRect = ui->lineColorLabel->geometry();
  fgRect.setRect(fgRect.x(), fgRect.y() + 22, fgRect.width(), fgRect.height());
  fg->setGeometry(fgRect);
  QRect bgRect = ui->brushColorLabel->geometry();
  bgRect.setRect(bgRect.x(), bgRect.y() + 22, bgRect.width(), bgRect.height());
  bg->setGeometry(bgRect);
  auto rcontent = ui->graphicsView->contentsRect();
  ui->graphicsView->scene()->setSceneRect(0, 0, rcontent.width(),
                                          rcontent.height());
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  QPointF p = ui->graphicsView->mapToScene(event->pos());
  p.setX(p.x() - ui->graphicsView->geometry().x());
  p.setY(p.y() - ui->graphicsView->geometry().y() - 20);
  std::vector<double> time;
  if (p.x() < 0 || p.x() > ui->graphicsView->width() || p.y() < 0 ||
      p.y() > ui->graphicsView->height())
    return;
  if (is_seed_inputing) {

    auto params = params_t{.command = CLOSE_POLYGON, .time = time};
    request(*drawer, params).handle();

    ui->seedX->setValue(p.x());
    ui->seedY->setValue(p.y());
    seed.x = p.x();
    seed.y = p.y();
    is_seed_inputing = false;
    return;
  }

  ui->undoButton->setEnabled(true);
  states_push(
      states,
      state_create(
          request(*drawer, params_t{.command = -1, .time = time}).handle(),
          ui->pointsTable));
  QColor qcolor = fg->get_color();
  color_t color;
  qcolor.getRgb(&color.r, &color.g, &color.b);

  point_t point = {p.x(), p.y(), color};
  auto params = params_t{.command = ADD_POINT, .time = time, .point = point};
  request(*drawer, params).handle();
  prev = table_create(ui->pointsTable);
  add_point_to_pointsTable(point);
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

void MainWindow::on_brushColorButton_clicked() {
  QColorDialog dialog;
  dialog.setCurrentColor(bg->get_color());
  dialog.show();
  dialog.exec();
  QColor color = dialog.selectedColor();
  if (!color.isValid())
    return;
  bg->set_color(color);
}

void MainWindow::on_modeSelect_currentIndexChanged(int index) {
  ui->Delay->setEnabled(index != 0);
  if (index != 0)
    ui->Delaylabel->setText("Задержка (в мс.)");
  else
    ui->Delaylabel->setText("");
}

void MainWindow::on_addRowButton_clicked() {
  ui->pointsTable->insertRow(ui->pointsTable->rowCount());
}

void MainWindow::on_clearButton_clicked() {
  std::vector<double> time;
  ui->undoButton->setEnabled(true);
  print_message("", Qt::black);
  states_push(
      states,
      state_create(
          request(*drawer, params_t{.command = -1, .time = time}).handle(),
          ui->pointsTable));
  request(*drawer, {.command = CLEAR_SCREEN, .time = time}).handle();
  ui->pointsTable->clear();
  QTableWidgetItem *x = new QTableWidgetItem;
  QTableWidgetItem *y = new QTableWidgetItem;
  x->setText("X");
  y->setText("Y");
  ui->pointsTable->setHorizontalHeaderItem(0, x);
  ui->pointsTable->setHorizontalHeaderItem(1, y);
  prev = table_create(ui->pointsTable);
}

void MainWindow::on_undoButton_clicked() {
  std::vector<double> time;
  polygones_t polygones;
  state_set(polygones, states_pop(states), *drawer, ui->pointsTable);
  auto params =
      params_t{.command = SET_POLYGONES, .time = time, .polygones = polygones};
  request(*drawer, params).handle();
  if (states.empty())
    ui->undoButton->setEnabled(false);
}

void MainWindow::on_closePolygonButton_clicked() {
  ui->addHoleButton->setStyleSheet("");
  QColor qcolor = fg->get_color();
  color_t color;
  qcolor.getRgb(&color.r, &color.g, &color.b);
  std::vector<double> time;
  polygones_t polygones =
      request(*drawer, params_t{.command = -1, .time = time}).handle();
  if (!polygones.size()) {
    error_messagebox("Нечего замыкать");
    return;
  }
  states_push(states, state_create(polygones, ui->pointsTable));
  auto params = params_t{.command = CLOSE_POLYGON, .time = time};
  request(*drawer, params).handle();
  prev = table_create(ui->pointsTable);
}

void MainWindow::on_fillButton_clicked() {
  std::vector<double> time; // TODO
  polygones_t polygones =
      request(*drawer, params_t{.command = -1, .time = time}).handle();
  if (!polygones_is_all_closed(polygones)) {
    error_messagebox("Замкните все многоугольники, чтобы залить их");
    return;
  }
  if (!polygones.size()) {
    error_messagebox("Нечего заливать");
    return;
  }
  QColor qcolor = bg->get_color();
  color_t color;
  qcolor.getRgb(&color.r, &color.g, &color.b);
  sleep_data_t sleep_data = {.is_sleep = ui->modeSelect->currentIndex() == 1,
                             .sleep_time = ui->Delay->value()};
  auto params = params_t{.command = FILL,
                         .color = color,
                         .time = time,
                         .point = seed,
                         .sleep_data = sleep_data};
  request(*drawer, params).handle();

  polygones = request(*drawer, params_t{.command = -1, .time = time}).handle();
  if (!polygones[polygones.size() - 2].filled) {
    QMessageBox::critical(this, "Ошибка",
                          "Растравая точка вне многоугольника!");
  }

  std::string str;
  str.append("Время заливки многоугольников (в мкс.): ");
  const double eps = 1e-8;
  for (auto &i : time) {
    if (fabs(i) > eps) {
      str.append(std::to_string(i));
      str.append(" ");
    }
  }
  print_message(QString(str.c_str()), Qt::black);
}

void MainWindow::on_addHoleButton_clicked() {
  ui->addHoleButton->setStyleSheet(
      "QPushButton{ background-color: rgb(255,86,86); color:rgb(255,251,175) "
      "}\n");
  std::vector<double> time;
  polygones_t polygones =
      request(*drawer, params_t{.command = -1, .time = time}).handle();
  if (!polygones.size()) {
    error_messagebox("Нет многоугольников");
    ui->addHoleButton->setStyleSheet("");
    return;
  }
  if (!polygones_is_all_closed(polygones)) {
    error_messagebox("Замкните все многоугольники, чтобы добавлять отверстие");
    ui->addHoleButton->setStyleSheet("");
    return;
  }
  if (polygones[polygones.size() - 2].filled == 1) {
    error_messagebox("Не нарисован новый многоугольник");
    ui->addHoleButton->setStyleSheet("");
    return;
  }

  states_push(states, state_create(polygones, ui->pointsTable));
  auto params = params_t{.command = ADD_HOLE, .time = time};
  request(*drawer, params).handle();
  prev = table_create(ui->pointsTable);
}

void MainWindow::add_point_to_pointsTable(const point_t &point) {
  for (int i = 0; i < ui->pointsTable->rowCount(); i++) {
    if (i == ui->pointsTable->rowCount() - 1)
      ui->pointsTable->insertRow(ui->pointsTable->rowCount());
    QTableWidgetItem *x_item = ui->pointsTable->item(i, 0);
    QTableWidgetItem *y_item = ui->pointsTable->item(i, 1);
    if ((x_item == nullptr && y_item == nullptr) ||
        (x_item->text().isEmpty() && y_item->text().isEmpty())) {
      QTableWidgetItem *x = new QTableWidgetItem;
      QTableWidgetItem *y = new QTableWidgetItem;
      x->setText(QString::number(point.x));
      y->setText(QString::number(point.y));
      ui->pointsTable->setItem(i, 0, x);
      ui->pointsTable->setItem(i, 1, y);
      return;
    }
  }
}

void MainWindow::on_pointsTable_cellClicked(int row, int column) {
  ui->removePointButton->setEnabled(true);
  bool rc;

  color_t yellow_color = {255, 255, 0};
  color_t black_color = {0, 0, 0};
  for (int i = 0; i < ui->pointsTable->height(); i++) {
    double x, y;
    QTableWidgetItem *x_item = ui->pointsTable->item(i, 0);
    QTableWidgetItem *y_item = ui->pointsTable->item(i, 1);
    if (x_item == nullptr || y_item == nullptr)
      break;
    QString x_str = x_item->text();
    QString y_str = y_item->text();
    if (x_str.isEmpty() || y_str.isEmpty())
      break;
    x = x_str.toDouble(&rc);
    if (!rc)
      break;
    y = y_str.toDouble(&rc);
    if (!rc)
      break;
    if (i == row)
      drawer->draw_point(x, y, yellow_color);
    else
      drawer->draw_point(x, y, black_color);
  }
}

void MainWindow::on_removePointButton_clicked() {
  std::vector<double> time;
  polygones_t polygones =
      request(*drawer, params_t{.command = -1, .time = time}).handle();
  if (!polygones.size()) {
    error_messagebox("Нет точек для удаления");
    return;
  }

  if (!polygones_is_all_closed(polygones)) {
    error_messagebox("Замкните все многоугольники, чтобы удалить точки");
    return;
  }
  ui->undoButton->setEnabled(true);
  states_push(states, state_create(polygones, ui->pointsTable));
  size_t cur_row = ui->pointsTable->currentRow();
  ui->pointsTable->removeRow(cur_row);
  auto params =
      params_t{.command = REMOVE_POINT, .time = time, .index = cur_row};
  request(*drawer, params).handle();
  print_message("", Qt::black);
  prev = table_create(ui->pointsTable);
}

void MainWindow::on_pointsTable_currentCellChanged(int currentRow,
                                                   int currentColumn,
                                                   int previousRow,
                                                   int previousColumn) {
  std::vector<double> time;
  ui->undoButton->setEnabled(true);
  state_t state = state_create(
      request(*drawer, params_t{.command = -1, .time = time}).handle(),
      ui->pointsTable, prev);
  states_push(states, state);
  QTableWidgetItem *x_item = ui->pointsTable->item(previousRow, 0);
  QTableWidgetItem *y_item = ui->pointsTable->item(previousRow, 1);
  if (x_item == nullptr && y_item == nullptr) {
    states_pop(states);
    return;
  }
  if (x_item == nullptr || y_item == nullptr) {
    error_messagebox(
        QString("Ошибка при вводе точки: поле x или y %1 точки пустое!")
            .arg(QString::number(previousRow + 1)));
    states_pop(states);
    return;
  }
  QString x_str = x_item->text();
  QString y_str = y_item->text();
  if (x_str.isEmpty() && y_str.isEmpty()) {
    states_pop(states);
    return;
  }
  if (x_str.isEmpty() || y_str.isEmpty()) {
    error_messagebox(
        QString("Ошибка при вводе точки: поле x или y %1 точки пустое!")
            .arg(QString::number(previousRow + 1)));
    states_pop(states);
    return;
  }
  double x, y;
  bool rc;
  x = x_str.toDouble(&rc);
  if (!rc) {
    error_messagebox(QString("Ошибка при вводе точки %1: x - некорректный!")
                         .arg(QString::number(previousRow + 1)));
    states_pop(states);
    return;
  }
  y = y_str.toDouble(&rc);
  if (!rc) {
    error_messagebox(QString("Ошибка при вводе точки %1: y - некорректный!")
                         .arg(QString::number(previousRow + 1)));
    states_pop(states);
    return;
  }

  QColor qcolor = fg->get_color();
  color_t color;
  qcolor.getRgb(&color.r, &color.g, &color.b);
  polygones_t prev_polygones =
      request(*drawer, params_t{.command = -1, .time = time}).handle();
  auto params = params_t{.command = CHANGE_POINT,
                         .time = time,
                         .point = {x, y, color},
                         .index = (size_t)previousRow};
  request(*drawer, params).handle();
  polygones_t new_polygones =
      request(*drawer, params_t{.command = -1, .time = time}).handle();
  if (polygones_are_equal(prev_polygones, new_polygones))
    states_pop(states);
  print_message("", Qt::black);
  prev = table_create(ui->pointsTable);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Return) {
    on_closePolygonButton_clicked();
  }
  if (event->key() == Qt::Key_Shift) {
    on_fillButton_clicked();
  }
  if (event->key() == Qt::Key_Space) {
    on_addHoleButton_clicked();
  }
}

void MainWindow::on_seedButton_clicked() { is_seed_inputing = true; }

void MainWindow::on_seedX_textChanged(const QString &arg1) {
  std::vector<double> time;
  polygones_t polygones =
      request(*drawer, params_t{.command = -1, .time = time}).handle();
  drawer->redraw(polygones);
  drawer->draw_point(ui->seedX->value(), ui->seedY->value(),
                     color_t{255, 223, 0});
  seed.x = ui->seedX->value();
  seed.y = ui->seedY->value();
}

void MainWindow::on_seedY_textChanged(const QString &arg1) {
  std::vector<double> time;

  polygones_t polygones =
      request(*drawer, params_t{.command = -1, .time = time}).handle();
  drawer->redraw(polygones);
  drawer->draw_point(ui->seedX->value(), ui->seedY->value(),
                     color_t{255, 223, 0});
  seed.x = ui->seedX->value();
  seed.y = ui->seedY->value();
}
