#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "canvas.h"
#include <QColor>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void progInfo_messagebox();

    void authorInfo_messagebox();

    void exit_messagebox();

    void on_bg_clicked();

    void on_fg_clicked();

    void on_drawButton_clicked();

    void on_clearButton_clicked();

    void on_scaleButton_clicked();

    void on_rotate_x_clicked();

    void on_rotate_y_clicked();

    void on_rotate_z_clicked();

    void resizeEvent(QResizeEvent* event);

private:
    Ui::MainWindow* ui;
    bool is_draw;
    QColor bg = QColor(255, 255, 255);
    QColor fg = QColor(0, 0, 0);
};
#endif // MAINWINDOW_HPP
