#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

typedef struct {
  QPoint center;
  int r1;
  int r2;
} circle_t;

typedef struct {
  int p1;
  int p2;
} line_t;

typedef std::vector<line_t> lines_t;

typedef struct {
  std::vector<QPoint> points;
  lines_t lines;
  circle_t nose;
  circle_t r_eye;
  circle_t l_eye;
  circle_t r_pupil;
  circle_t l_pupil;
} cat_t;

enum Events {
  EVENT_MOVE,
  EVENT_SCALE,
  EVENT_ROTATION,
  EVENT_RESET,
};

typedef struct {
  Events event;
  union {
    struct {
      int dx;
      int dy;
    } move;
    struct {
      int cx;
      int cy;
      double kx;
      double ky;
    } scale;
    struct {
      int cx;
      int cy;
      int angle;
    } rotate;
  } parameters;
} event_t;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);

  ~MainWindow();

  void resizeEvent(QResizeEvent *event);

  void showEvent(QShowEvent *ev);

private slots:

  void on_action_triggered();

  void on_action_2_triggered();

  void on_action_4_triggered();

  virtual void mousePressEvent(QMouseEvent *event);

  void initPoints();

  void createCat();

  void drawPic();

  void to_abs_coordinates(int x, int y, int *res_x, int *res_y);

  void from_abs_coordinates(int x, int y, int *res_x, int *res_y);

  void drawAxis(QPainter *p);

  void draw_circle(QPainter *p, circle_t circle);

  void on_movebutton_clicked();

  void scale(int cx, int cy, double kx, double ky);

  void on_scaleButton_clicked();

  void rotation_for_circle(circle_t *circle, int cx, int cy, double rad);

  void rotation(int cx, int cy, int angle);

  void on_rotationButton_clicked();

  void on_cleanButton_clicked();

  void on_backButton_clicked();

  void on_choose_center_rotate_clicked();

  void on_choose_center_scale_clicked();

private:
  Ui::MainWindow *ui;
  cat_t cat;
  std::vector<event_t> lastevents;
  std::vector<QPoint> deleted;
  int min_x, max_x;
  int min_y, max_y;
  int abs_x = 0, abs_y = 0;
  int r = 10;
  bool is_center_rotate = false, is_center_scale = false;
};
#endif
