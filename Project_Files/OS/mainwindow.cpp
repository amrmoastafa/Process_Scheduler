#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
setFixedSize(1900, 1100);
//m_button = new QPushButton("Hello World", this);
 //m_button->setGeometry(10, 10, 80, 30);
    //num = new QLineEdit(this);
    //num->show();
    num2 = new QLineEdit(this);
    num2->setGeometry(1100,400,80,80);
    //num2->show();



    toolbar = new QToolBar(this);
    toolbar->setStyleSheet("QToolBar{ background-color:rgb(78,204,163);  color:black; }");
    toolbar->setGeometry(0,0,300,1000);


   /****type of scheduler*****/
   type= new QLabel(this);
   type->setText("Scheduler Type");
   type->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
   type->setGeometry(10,30,120,30);


   Algorithm = new QComboBox(this);
   Algorithm->setStyleSheet("background-color:white;");
   Algorithm->setGeometry(150,30,120,30);
   Algorithm->addItem("FCFS");
   Algorithm->addItem("SJF");
   Algorithm->addItem("Round Robin");
   Algorithm->addItem("Priority");


  /******number of process input*****/
  N_process= new QLabel (this);
  N_process->setText("Number of process");
  N_process->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
  N_process->setGeometry(10,90,130,30);

  number= new   QLineEdit(this);
  number->setGeometry(150,90,120,30);
  number->setStyleSheet("background-color:white;");
  QString p= number->text();


  Ok = new QPushButton("OK",this);
  Ok->setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius: 4px;} "
                    "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");
 Ok->setGeometry(40,150,210,40);

 //connect(Ok,SIGNAL(clicked()),this,SLOT(get_param())) ;

   // ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow:: get_param(){
    this->t1= this->number->text();
    this->t2 = this->Algorithm->currentText();
    this->number->setText(this->t2);
    this->num2->setText(this->t1);



   // for (int i = 0; i<10;i++){

    //}

    qDebug("test");
}
