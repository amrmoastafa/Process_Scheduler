#include "mainwindow.h"
#include <QApplication>
#include <QComboBox>
#include <QToolBar>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout> //for the process
#include <QObject>
#include <QtGlobal>
#include <QTextStream>



//void MainWindow:: test(){
//qDebug("5 sa7");
   /* this->tp= num->text();
    QString ts = Algorithm->currentText();
    num->setText(ts);
    //num2->setText(tp);
    qDebug("done");
    if (tp == "5"){
        qDebug("5 sa7");
    }*/

//}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    w.setWindowTitle("CPU_Scheduler");
      //  w.setGeometry(0,0,1900,1100);
        w.setStyleSheet("background-color:rgb(238,238,238); ");
        //while(w.t2 == "SJF");
    QObject ::connect(w.Ok,SIGNAL(clicked()),&w,SLOT(get_param())) ;
        if (w.t1 == "8"){
                       qDebug("8 sa7");
                   }
        QTextStream output(stdout);
        w.t2="yarab";
        QString str = w.t2;

        output << str;
   w.show();

    return a.exec();
}
