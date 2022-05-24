#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.setWindowTitle("Алгоритм Сазерленда-Ходжмена");
  w.show();
  return a.exec();
}
