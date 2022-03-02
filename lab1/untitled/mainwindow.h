#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPointF>
#include <QPushButton>
#include <QResizeEvent>
#include <QTableWidget>
#include <stack>

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
  void mousePressEvent(QMouseEvent *event);
  void resizeEvent(QResizeEvent *event);

private slots:

  void on_action_9_triggered();

  void on_action_6_triggered();

  void on_action_8_triggered();

  void drawPoint(QPainter *p, QColor col, QPoint point);

  void drawPoints();

  void on_backbutton_clicked();

  void on_table_itemChanged();

  void on_table_cellClicked(int row, int column);

  int scan_from_table();
  void scan_to_table();

  void on_cleanbutton_clicked();

  void to_abs_coordinates(int x, int y, int *res_x, int *res_y);

  void from_abs_coordinates(int x, int y, int *res_x, int *res_y);

  void drawAxis(QPainter *p);

  void on_drawbutton_clicked();

  void on_findbutton_clicked();

  void addPoint(int x, int y, bool is_blocked);

  void on_addButton_clicked();

  void on_deletebutton_clicked();

  void on_table_itemChanged(QTableWidgetItem *item);

private:
  Ui::MainWindow *ui;
  std::vector<QPoint> points;
  std::vector<QPoint> del_points;
  std::vector<QPolygon> polygons;
  std::vector<int> lastevents;
  std::vector<std::vector<QPoint>> vec;
  std::vector<std::vector<std::vector<double>>> matrixes;
  int size_x = 100;
  int size_y = 100;
  int min_x, max_x;
  int min_y, max_y;
  int is_draw_polygon = 0;
  int is_find_polygon = 0;
  int ind1 = 0, ind2 = 0;
  size_t cnt_polygons = 1;
  int highlighted = -1;
};
#endif // MAINWINDOW_H
