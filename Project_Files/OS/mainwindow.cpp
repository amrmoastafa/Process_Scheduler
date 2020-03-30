#include "mainwindow.h"
//#include "ui_mainwindow.h"

#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
   // ,ui(new Ui::MainWindow)
{

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

  connect(Ok,SIGNAL(clicked()),this,SLOT(get_param())) ;

    //ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    //delete ui;
}


/*to get the no of process and the alg
 * create qlineEdit for each process' burst and arrival time
 */
void MainWindow:: get_param(){
    this->num_process= this->number->text();
    this->alg = this->Algorithm->currentText();

    arrival_label = new QLabel();
    burst_label = new QLabel();
    arrival_label->setGeometry(70,200,80,30);
    burst_label->setGeometry(180,200,80,30);
    arrival_label->setText("Arrival Time");
    burst_label->setText("Burst Time");
    arrival_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    burst_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    this->layout()->addWidget(burst_label);
    this->layout()->addWidget(arrival_label);


    int height=0;
    for (int i = 0; i<this->num_process.split(" ")[0].toInt(); i++)//convert from QString to int
    {
        arrival_input = new QLineEdit();
        burst_input = new QLineEdit();

        burst_time.push_back(burst_input);
        arrival_time.push_back(arrival_input);

        ID = new QLabel();
        ID->setText(tr("P %1").arg(i));
        ID->setGeometry(20,250+height,30,30);
        ID->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
        arrival_input->setGeometry(60,250+height,80,30);
        burst_input->setGeometry(170,250+height,80,30);

        this->layout()->addWidget(arrival_input);
        this->layout()->addWidget(burst_input);
        this->layout()->addWidget(ID);
        height+=50;

    }

    Simulate = new QPushButton("Simulate");
    Simulate->setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius: 4px;} "
                          "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");
    Simulate->setGeometry(40,height+270,210,40);
    this->layout()->addWidget(Simulate);

    connect(Simulate,SIGNAL(clicked()),this,SLOT(Get_Text())) ;


}

void MainWindow::Get_Text()
{QTextStream output(stdout);
    output <<burst_time.size()<<endl;
    for(int j = 0; j<burst_time.size(); j++)
    {
        QString text = burst_time.at(j)->text();
        output << text<<endl;

    }

    output <<"*********"<<endl;

    for(int j = 0; j<arrival_time.size(); j++)
    {
        QString text = arrival_time.at(j)->text();
        output << text<<endl;

    }
}

