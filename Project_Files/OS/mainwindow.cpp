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
    Algorithm_dropdown->addItems({"FCFS","SJF","Round Robin","Priority"});

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

    connect(Simulate,SIGNAL(clicked()),this,SLOT(Get_Data()));
}


/*Function to get the values stored in the burst and arrival time vectors*/ /***debugging function***/
void MainWindow::Get_Data()
{
    //We can use qDebug() Function to view variables as follows:
    //qDebug()<<"This is the value of first element of burst time vector : " <<burst_time[0]->text();
    //End

    for(int j = 0; j<burst_time.size(); j++)
    {
        Processes_Queue[j]->Arrival_Time = (arrival_time[j]->text().toInt());

        qDebug() <<"Process : " <<Processes_Queue[j]->Process_name<<" Arrival Time is : "<<Processes_Queue[j]->Arrival_Time;

        Processes_Queue[j]->Burst_Time = (burst_time[j]->text().toInt());//getting the text written (burst time of process j) from line edit of j
        Processes_Queue[j]->ID =j;

        qDebug() <<"Process : " <<Processes_Queue[j]->Process_name<<" Burst Time is : "<<Processes_Queue[j]->Burst_Time;
        if(Algorithm_dropdown->currentText() == "Priority")
        {
            Processes_Queue[j]->Priority = ((priority_vect[j]->currentIndex())+1);

            qDebug() <<"Process : " <<Processes_Queue[j]->Process_name<<" Priority is : "<<Processes_Queue[j]->Priority;

        }

    }

    if(Alg_chosen == "SJF" && Preemptive_Checkbox->isChecked() ) SJF_P_Alg();
    else if (Alg_chosen == "SJF" && !Preemptive_Checkbox->isChecked()) SJF_NONP_Alg();
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
        Process *p = new Process;
        //Creating a pointer to a process to carry any random address initially given by compiler
        Processes_Queue.append(p);
        // Appending that address to the pointer vector
        qDebug()<< "TRIAL : " << burst_input->text();
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
    Preemptive_Checkbox = new QCheckBox();
    Preemptive_Checkbox->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    Preemptive_Checkbox->setGeometry(80,height+270,18,18);
    Preemptive_label = new QLabel("Pre-emptive");
    Preemptive_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    Preemptive_label->setGeometry(110,height+270,80,18);
    this->layout()->addWidget(Preemptive_Checkbox);
    this->layout()->addWidget(Preemptive_label);
    height += 50;

    return height;
}

int MainWindow::FCFS_layout(){
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
        Process *p = new Process;
        //Creating a pointer to a process to carry any random address initially given by compiler
        Processes_Queue.append(p);
        // Appending that address to the pointer vector
        qDebug()<< "TRIAL : " << burst_input->text();
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

int MainWindow::RR_layout(){
    int height=0;
    qDebug("RR");
    return height;
}

int MainWindow::PRIORITY_layout(){
    /*setting the Arrival an burst time label*/
    arrival_label = new QLabel("Arrival Time");
    burst_label = new QLabel("Burst Time");
    arrival_label->setGeometry(40,200,80,30);
    burst_label->setGeometry(130,200,80,30);

    priortiy_label = new QLabel("Priority");
    priortiy_label->setGeometry(220,200,50,30);
    priortiy_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    this->layout()->addWidget(priortiy_label);




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

        priority_input = new QComboBox();
        priority_vect.push_back(priority_input);
        priority_input->addItems({"1","2","3","4","5"});
        priority_input->setGeometry(220,250+height,40,30);

        this->layout()->addWidget(priority_input);

        Process *p = new Process;

        //Creating a pointer to a process to carry any random address initially given by compiler
        Processes_Queue.append(p);
        // Appending that address to the pointer vector
        burst_time.push_back(burst_input);
        arrival_time.push_back(arrival_input);

        ID_Process = new QLabel();

        ID_Process->setText(tr("P%1").arg(i));
        Processes_Queue[i]->Process_name = ID_Process->text();

        ID_Process->setGeometry(20,250+height,30,30);
        ID_Process->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
        arrival_input->setGeometry(60,250+height,40,30);
        burst_input->setGeometry(140,250+height,40,30);

        this->layout()->addWidget(priority_input);
        this->layout()->addWidget(arrival_input);
        this->layout()->addWidget(burst_input);

        this->layout()->addWidget(ID_Process);
        height+=50;

    }
    Preemptive_Checkbox = new QCheckBox();
    Preemptive_Checkbox->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    Preemptive_Checkbox->setGeometry(80,height+270,18,18);
    Preemptive_label = new QLabel("Pre-emptive");
    Preemptive_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    Preemptive_label->setGeometry(110,height+270,80,18);
    this->layout()->addWidget(Preemptive_Checkbox);
    this->layout()->addWidget(Preemptive_label);
    height += 50;
    return height;
}

void MainWindow::SJF_NONP_Alg(){
    /*sorting according to burst time*/
    for(int i=0; i<Processes_Queue.size(); i++){
        for(int j=0; j<Processes_Queue.size(); j++){
            if(Processes_Queue[j]->Burst_Time >  Processes_Queue[i]->Burst_Time){
                Process *temp=Processes_Queue[j];
                Processes_Queue[j]=Processes_Queue[i];
                Processes_Queue[i]= temp;
            }
        }
    }
    for(int i=0; i<Processes_Queue.size(); i++){
        draw_process = new QLabel();
        draw_process->setText(tr("P %1").arg(Processes_Queue[i]->ID));
        draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
        draw_process->setGeometry(250+(Processes_Queue[i]->Burst_Time*100),700,Processes_Queue[i]->Burst_Time*80,50);
        this->layout()->addWidget(draw_process);
    }
}
void MainWindow::SJF_P_Alg(){
    qDebug()<<"Preemptive";
}
