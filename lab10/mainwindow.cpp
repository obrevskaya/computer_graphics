#include <QColorDialog>
#include <QMessageBox>

#include "funcs.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->bg_view->set_color(bg);
  ui->fg_view->set_color(fg);

  ui->field->set_bg(ui->bg_view->get_color());
  ui->field->set_fg(ui->fg_view->get_color());

  connect(ui->progInfo, SIGNAL(triggered()), this, SLOT(progInfo_messagebox()));
  connect(ui->authorInfo, SIGNAL(triggered()), this,
          SLOT(authorInfo_messagebox()));
  connect(ui->exit, SIGNAL(triggered()), this, SLOT(exit_messagebox()));

  is_draw = false;
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::progInfo_messagebox() {
  QMessageBox::information(this, "О программе",
                           "Программа, реализующая алгоритм волнового фронта");
}

void MainWindow::authorInfo_messagebox() {
  QMessageBox::information(this, "Об авторе", "Обревская Вероника ИУ7-42Б");
}

void MainWindow::exit_messagebox() { QApplication::quit(); }

void MainWindow::on_bg_clicked() {
  QColorDialog dialog;
  dialog.show();
  dialog.exec();
  ui->bg_view->set_color(dialog.selectedColor());

  ui->field->set_bg(dialog.selectedColor());
}

void MainWindow::on_fg_clicked() {
  QColorDialog dialog;
  dialog.show();
  dialog.exec();
  ui->fg_view->set_color(dialog.selectedColor());

  ui->field->set_fg(dialog.selectedColor());
}

void MainWindow::on_drawButton_clicked() {
  is_draw = true;
  double (*f)(double x, double z);
  if (ui->selectionFunc->currentIndex() == 0)
    f = func3;
  else if (ui->selectionFunc->currentIndex() == 1)
    f = func4;
  else if (ui->selectionFunc->currentIndex() == 2)
    f = func2;
  else
    f = func1;
  ui->field->draw(f, ui->x_min->text().toDouble(), ui->x_max->text().toDouble(),
                  ui->x_step->text().toDouble(), ui->z_min->text().toDouble(),
                  ui->z_max->text().toDouble(), ui->z_step->text().toDouble());
}

void MainWindow::on_clearButton_clicked() {
  ui->field->clear();
  is_draw = false;
}

void MainWindow::on_scaleButton_clicked() {
  if (not is_draw)
    return;
  ui->field->set_scale(ui->scale->text().toDouble());
  double (*f)(double x, double z);
  if (ui->selectionFunc->currentIndex() == 0)
    f = func3;
  else if (ui->selectionFunc->currentIndex() == 1)
    f = func4;
  else if (ui->selectionFunc->currentIndex() == 2)
    f = func2;
  else
    f = func1;
  ui->field->draw(f, ui->x_min->text().toDouble(), ui->x_max->text().toDouble(),
                  ui->x_step->text().toDouble(), ui->z_min->text().toDouble(),
                  ui->z_max->text().toDouble(), ui->z_step->text().toDouble());
}

void MainWindow::on_rotate_x_clicked() {
  if (not is_draw)
    return;
  ui->field->rotate_x(ui->angle_x->text().toDouble());
  double (*f)(double x, double z);
  if (ui->selectionFunc->currentIndex() == 0)
    f = func3;
  else if (ui->selectionFunc->currentIndex() == 1)
    f = func4;
  else if (ui->selectionFunc->currentIndex() == 2)
    f = func2;
  else
    f = func1;
  ui->field->draw(f, ui->x_min->text().toDouble(), ui->x_max->text().toDouble(),
                  ui->x_step->text().toDouble(), ui->z_min->text().toDouble(),
                  ui->z_max->text().toDouble(), ui->z_step->text().toDouble());
}

void MainWindow::on_rotate_y_clicked() {
  if (not is_draw)
    return;
  ui->field->rotate_y(ui->angle_y->text().toDouble());
  double (*f)(double x, double z);
  if (ui->selectionFunc->currentIndex() == 0)
    f = func3;
  else if (ui->selectionFunc->currentIndex() == 1)
    f = func4;
  else if (ui->selectionFunc->currentIndex() == 2)
    f = func2;
  else
    f = func1;
  ui->field->draw(f, ui->x_min->text().toDouble(), ui->x_max->text().toDouble(),
                  ui->x_step->text().toDouble(), ui->z_min->text().toDouble(),
                  ui->z_max->text().toDouble(), ui->z_step->text().toDouble());
}

void MainWindow::on_rotate_z_clicked() {
  if (not is_draw)
    return;
  ui->field->rotate_z(ui->angle_z->text().toDouble());
  double (*f)(double x, double z);
  if (ui->selectionFunc->currentIndex() == 0)
    f = func3;
  else if (ui->selectionFunc->currentIndex() == 1)
    f = func4;
  else if (ui->selectionFunc->currentIndex() == 2)
    f = func2;
  else
    f = func1;
  ui->field->draw(f, ui->x_min->text().toDouble(), ui->x_max->text().toDouble(),
                  ui->x_step->text().toDouble(), ui->z_min->text().toDouble(),
                  ui->z_max->text().toDouble(), ui->z_step->text().toDouble());
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  if (is_draw)
    on_drawButton_clicked();
}
