#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("CPU_Scheduler");
    w.setGeometry(0,0,1800,1000);
    w.setStyleSheet("background-color:rgb(238,238,238); ");
    w.show();

 return a.exec();
}
