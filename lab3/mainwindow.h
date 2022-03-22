#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "drawing.h"
#include "tasks.h"
#include "states.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QWheelEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void progInfo_messagebox();
  void authorInfo_messagebox();
  void error_messagebox(QString text);
  void exit_messagebox();

  void on_lineColorButton_clicked();

  void on_bgColorButton_clicked();

  void on_drawLineButton_clicked();

  void on_drawSpectrumButton_clicked();

  void on_compare_timeButton_clicked();

  void measure_steps(method_t method, std::vector<size_t> &steps, double r,
                     double step);

  void on_compare_stepsButton_clicked();

  void on_clearButton_clicked();

  void on_undoButton_clicked();

protected:
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  Ui::MainWindow *ui;
  QGraphicsScene *scene;
  drawer_t *drawer;
  color_view *fg;
  color_view *bg;
  std::stack<std::vector<data_t>> states;
  std::vector<data_t> dataset;
};
#endif // MAINWINDOW_H
