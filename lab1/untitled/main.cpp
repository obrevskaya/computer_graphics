#include "mainwindow.h"

#include <QtGui>
#include <QPushButton>
#include <QApplication>
#include <QVBoxLayout>
#include <QTextEdit>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
