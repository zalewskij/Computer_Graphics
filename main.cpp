#include "mainwindow.h"
#include "function_filters.h"
#include "convolution_filters.h"
#include <QApplication>
#include<QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
