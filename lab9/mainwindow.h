#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow* ui;
    QColor seg_color = QColor(0, 0, 0);
    QColor vis_color = QColor(255, 0, 0);
    QColor cut_color = QColor(0, 255, 0);

private slots:
    void progInfo_messagebox();
    void authorInfo_messagebox();
    void error_messagebox(QString text);
    void exit_messagebox();
    void info_messagebox(QString text);
    void on_seg_col_Button_clicked();
    void on_vis_part_col_Button_clicked();
    void on_cut_col_Button_clicked();
    void on_cutButton_clicked();
    void on_undoButton_clicked();
    void on_clearButton_clicked();
    void on_clipperRadioButton_clicked();
    void on_segRadioButton_clicked();
    void on_addPointButton_clicked();
    void on_closeButton_clicked();
};

#endif // MAINWINDOW_H
