#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "model.h"
#include "view.h"

#include <QLineEdit>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum class InputType {
  CIRCLE,
  ELLIPSE,
};

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_colorButton_clicked();
  void on_backgroundColorButton_clicked();
  void error_messagebox(QString text);
  void on_comboBox_activated(int index);

  void on_drawCircleButton_clicked();

  void on_drawEllipseButton_clicked();

  void on_ellipseSpectr_clicked();

  void on_circleSpectr_clicked();

  void on_backButton_clicked();

  void on_clearButton_clicked();

  void on_speedTestButton_clicked();

  void on_stepTestButton_clicked();

  void on_progInfo_triggered();

  void on_authorInfo_triggered();

  void on_exit_triggered();

  void on_speedEllipseButton_clicked();

  void on_stepEllipseButton_clicked();

protected:
  virtual void mousePressEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  bool eventFilter(QObject *object, QEvent *event);

private:
  static const int STATUS_BAR_TIMEOUT = 5000;
  Ui::MainWindow *ui;
  View view;
  Model model;
  void setLineColor(const QColor &color);
  void setBackgroundColor(const QColor &color);
  bool get_var(double &var, const QLineEdit *lineEdit, const QString &err_msg);
  bool get_positive_var(double &var, const QLineEdit *lineEdit,
                        const QString &err_msg);
  void tick(bool x = true);
  void setInputType(InputType inType);
  double drawingTime(Model &model);
};
#endif // MAINWINDOW_H
