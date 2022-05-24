#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->cut_col->set_color(cut_color);
  ui->seg_col->set_color(seg_color);
  ui->vis_part_col->set_color(vis_color);

  ui->canvas->set_cut_col(ui->cut_col->get_color());
  ui->canvas->set_vis_col(ui->vis_part_col->get_color());
  ui->canvas->set_seg_col(ui->seg_col->get_color());

  connect(ui->progInfo, SIGNAL(triggered()), this, SLOT(progInfo_messagebox()));
  connect(ui->authorInfo, SIGNAL(triggered()), this,
          SLOT(authorInfo_messagebox()));
  connect(ui->exit, SIGNAL(triggered()), this, SLOT(exit_messagebox()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::progInfo_messagebox() {
  QMessageBox::information(this, "ПРОГРАММА",
                           "Реализует простой алгоритм отсечения отрезка "
                           "регулярным отсекателем.");
}

void MainWindow::authorInfo_messagebox() {
  QMessageBox::information(this, "АВТОР", "Обревская Вероника ИУ7-42Б");
}

void MainWindow::exit_messagebox() { QApplication::quit(); }

void MainWindow::info_messagebox(QString text) {
  QMessageBox::information(this, "Сообщение", text);
}

void MainWindow::error_messagebox(QString text) {
  QMessageBox::critical(this, "Ошибка", text);
}

void MainWindow::on_seg_col_Button_clicked() {
  QColorDialog dialog;
  dialog.show();
  dialog.exec();
  ui->seg_col->set_color(dialog.selectedColor());
  ui->canvas->set_seg_col(dialog.selectedColor());
}

void MainWindow::on_vis_part_col_Button_clicked() {
  QColorDialog dialog;
  dialog.show();
  dialog.exec();
  ui->vis_part_col->set_color(dialog.selectedColor());
  ui->canvas->set_vis_col(dialog.selectedColor());
}

void MainWindow::on_cut_col_Button_clicked() {
  QColorDialog dialog;
  dialog.show();
  dialog.exec();
  ui->cut_col->set_color(dialog.selectedColor());
  ui->canvas->set_cut_col(dialog.selectedColor());
}

void MainWindow::on_cutButton_clicked() { ui->canvas->cut(); }

void MainWindow::on_undoButton_clicked() {
  bool is_not_empty = ui->canvas->undo();
  if (not is_not_empty) {
    info_messagebox("Вы в начальном состоянии");
    ui->clipperRadioButton->setChecked(true);
  }
}

void MainWindow::on_clearButton_clicked() {
  ui->undoButton->setEnabled(true);
  ui->canvas->clear();
  ui->clipperRadioButton->setChecked(true);
}

void MainWindow::on_clipperRadioButton_clicked() { ui->canvas->new_clip(); }

void MainWindow::on_segRadioButton_clicked() { ui->canvas->new_seg(); }

void MainWindow::on_addPointButton_clicked() {
  ui->undoButton->setEnabled(true);
  ui->canvas->add_point(ui->pointX->value(), ui->pointY->value());
}
