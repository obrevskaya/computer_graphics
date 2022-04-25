#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "color_view.h"
#include "drawer.h"
#include "states.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QTableWidgetItem>
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
  void keyPressEvent(QKeyEvent *event);

private slots:
  void print_message(QString message, QColor color);

  void error_messagebox(QString message);

  void progInfo_messagebox();

  void authorInfo_messagebox();

  void exit_messagebox();

  void on_lineColorButton_clicked();

  void on_brushColorButton_clicked();

  void on_modeSelect_currentIndexChanged(int index);

  void on_addRowButton_clicked();

  void on_clearButton_clicked();

  void on_undoButton_clicked();

  void on_closePolygonButton_clicked();

  void on_fillButton_clicked();

  void on_addHoleButton_clicked();

  void add_point_to_pointsTable(const point_t &point);

  void on_pointsTable_cellClicked(int row, int column);

  void on_removePointButton_clicked();

  void on_pointsTable_currentCellChanged(int currentRow, int currentColumn,
                                         int previousRow, int previousColumn);

  void on_seedButton_clicked();

  void on_seedX_textChanged(const QString &arg1);

  void on_seedY_textChanged(const QString &arg1);

protected:
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);

private:
  QGraphicsScene *scene;
  Ui::MainWindow *ui;
  drawer_t *drawer;
  color_view *fg;
  color_view *bg;
  std::stack<state_t> states;
  std::vector<std::vector<QString>> prev_table;
  std::vector<std::string> prev;
  bool is_seed_inputing;
  point_t seed;
};
#endif // MAINWINDOW_H
