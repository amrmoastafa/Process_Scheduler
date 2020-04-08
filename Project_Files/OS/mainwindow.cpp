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

   Algorithm_dropdown = new QComboBox(this);
   Algorithm_dropdown->setStyleSheet("background-color:white;");
   Algorithm_dropdown->setGeometry(150,30,120,30);
   Algorithm_dropdown->addItem("FCFS");
   Algorithm_dropdown->addItem("SJF");
   Algorithm_dropdown->addItem("Round Robin");
   Algorithm_dropdown->addItem("Priority");


  /******number of process input*****/
  N_process= new QLabel (this);
  N_process->setText("Number of process");
  N_process->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
  N_process->setGeometry(10,90,130,30);

  num_process_line_edit= new   QLineEdit(this);
  num_process_line_edit->setGeometry(150,90,120,30);
  num_process_line_edit->setStyleSheet("background-color:white;");
  QString p= num_process_line_edit->text();


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
 * choose layout for each algorithm
 */
void MainWindow:: get_param(){
    this->num_process_chosen= this->num_process_line_edit->text(); //getting the number of process from the line edit
    this->Alg_chosen = this->Algorithm_dropdown->currentText(); //getting the chosen algorithm from dropdown

  //height is a variable used to adjust the simulate button after the qline-edits are drawn
    int height=0;

    if (this->Alg_chosen == "SJF") height=SJF_layout();
    else if (this->Alg_chosen == "FCFS") height=FCFS_layout();
    else if (this->Alg_chosen == "Round Robin") height=RR_layout();
    else if (this->Alg_chosen == "Priority") height=PRIORITY_layout();

    Simulate = new QPushButton("Simulate");
    Simulate->setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius: 4px;} "
                            "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");
    Simulate->setGeometry(40,height+270,210,40);
    this->layout()->addWidget(Simulate);

    connect(Simulate,SIGNAL(clicked()),this,SLOT(Get_Text()));
}


/*Function to get the values stored in the burst and arrival time vectors*/ /***debugging function***/
void MainWindow::Get_Text()
{QTextStream output(stdout);
    output <<burst_time.size()<<endl;
    for(int j = 0; j<burst_time.size(); j++)
    {
        QString text = burst_time.at(j)->text(); //getting the text written (burst time of process j) from line edit of j
        output << text<<endl;
    }

    output <<"*********"<<endl;

    for(int j = 0; j<arrival_time.size(); j++)
    {
        QString text = arrival_time.at(j)->text();
        output << text<<endl;
    }
}


/*Layout for each algorithm
 * each one return the height of the simulate button
 */
int MainWindow::SJF_layout(){
    /*setting the Arrival an burst time label*/
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
    for (int i = 0; i<this->num_process_chosen.split(" ")[0].toInt(); i++)//convert from QString to int
    {
        /***creating a line edit and pushing the created line edit to a vector***/
         arrival_input = new QLineEdit();
         burst_input = new QLineEdit();

         burst_time.push_back(burst_input);
         arrival_time.push_back(arrival_input);

         ID_Process = new QLabel();
         ID_Process->setText(tr("P %1").arg(i));
         ID_Process->setGeometry(20,250+height,30,30);
         ID_Process->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
         arrival_input->setGeometry(60,250+height,80,30);
         burst_input->setGeometry(170,250+height,80,30);

         this->layout()->addWidget(arrival_input);
         this->layout()->addWidget(burst_input);
         this->layout()->addWidget(ID_Process);
         height+=50;

      }

     return height;
}

int MainWindow::FCFS_layout(){
    int height=0;
    qDebug("FCFS");
    return height;
}

int MainWindow::RR_layout(){
    int height=0;
    qDebug("RR");
    return height;
}

int MainWindow::PRIORITY_layout(){
    int height=0;
    qDebug("FCFS");
    return height;
}
