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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("CPU_Scheduler");
    w.setGeometry(0,0,1900,1100);
    w.setStyleSheet("background-color:rgb(238,238,238); ");

    QToolBar toolbar(&w);
    toolbar.setStyleSheet("QToolBar{ background-color:rgb(78,204,163);  color:black; }");
    toolbar.setGeometry(0,0,300,1000);


    /****type of scheduler*****/
    QLabel type(&w);
    type.setText("Scheduler Type");
    type.setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    type.setGeometry(10,30,120,30);

    QComboBox Algorithms(&w);
    Algorithms.setStyleSheet("background-color:white;");
    Algorithms.setGeometry(150,30,120,30);
    Algorithms.addItem("FCFS");
    Algorithms.addItem("SJF");
    Algorithms.addItem("Round Robin");
    Algorithms.addItem("Priority");

    QString alg= Algorithms.currentText();
    if(alg=="FCFS"){
        qDebug("FCFS");
    }

    /******number of process input*****/
    QLabel N_process(&w);
    N_process.setText("Number of process");
    N_process.setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    N_process.setGeometry(10,90,130,30);

    QLineEdit number(&w);
    number.setGeometry(150,90,120,30);
    number.setStyleSheet("background-color:white;");
    QString p= number.text();
    if (p=="4"){
        qDebug("p");
    }


    /****OK push button***/
    QPushButton Ok("OK",&w);
    Ok.setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius: 4px;} "
                     "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");
    Ok.setGeometry(40,150,210,40);


    w.show();

    return a.exec();
}
