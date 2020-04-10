#include "mainwindow.h"
//#include "ui_mainwindow.h"

#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  // ,ui(new Ui::MainWindow)
{
    //Drawing The Scene Where Processes will be drawn
    Scene = new QGraphicsScene();
    Scene->setBackgroundBrush(Qt::darkGray);
    view = new  QGraphicsView(Scene,this);
    view->setGeometry(300,0,1500,1000);

    //Adding Tool bar
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

    connect(Simulate,SIGNAL(clicked()),this,SLOT(Get_Text()));
}


/*Function to get the values stored in the burst and arrival time vectors*/
/*also to execute the chose algorithm*/
void MainWindow::Get_Text()
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
   /****choose the Algorithm to be executed****/
    if(Alg_chosen == "SJF" && Preemptive_Checkbox->isChecked() ) SJF_P_Alg();
    else if (Alg_chosen == "SJF" && !Preemptive_Checkbox->isChecked()) SJF_NONP_Alg();
    else if (Alg_chosen == "FCFS") FCFS_Alg();
    else if (Alg_chosen == "Round Robin")
    {
        /*Initializing quantum time*/
        for(int l=0; l<Processes_Queue.size();l++)
        {
            Processes_Queue[l]->quantum_time=time_quantum_input->text().toInt();
        }
        RR_Alg();
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
        Process *p = new Process;
        //Creating a pointer to a process to carry any random address initially given by compiler
        Processes_Queue.append(p);
        // Appending that address to the pointer vector
        qDebug()<< "TRIAL : " << burst_input->text();
        burst_time.push_back(burst_input);
        arrival_time.push_back(arrival_input);

        ID_Process = new QLabel();
        ID_Process->setText(tr("P%1").arg(i));
        //adding the name of the process attribute
        Processes_Queue[i]->Process_name = ID_Process->text();
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
        ID_Process->setText(tr("P%1").arg(i));
        //adding the name of the process attribute
        Processes_Queue[i]->Process_name = ID_Process->text();
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
    /*Time Quantum*/
    time_quantum_label = new QLabel();
    time_quantum_label->setGeometry(50,200,100,30);

    time_quantum_label->setText("Time Quantum");
    time_quantum_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");

    time_quantum_input = new QLineEdit();
    time_quantum_input->setGeometry(175,200,80,30);

    this->layout()->addWidget(time_quantum_label);
    this->layout()->addWidget(time_quantum_input);

    /*setting the Arrival an burst time label*/
    arrival_label = new QLabel("Arrival Time");
    burst_label = new QLabel("Burst Time");

    arrival_label->setGeometry(70,240,80,30);
    burst_label->setGeometry(170,240,80,30);

    arrival_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    burst_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");

    this->layout()->addWidget(burst_label);
    this->layout()->addWidget(arrival_label);

    /*Creating a new process and pushing time quantum to it*/



    int height=40;

    for (int i = 0; i<this->num_process_chosen.split(" ")[0].toInt(); i++)
    {
        arrival_input = new QLineEdit();
        burst_input = new QLineEdit();

        Process *p = new Process;
        Processes_Queue.append(p);

        //p->quantum_time=this->time_quantum_input->text().toInt();

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
        //adding the name of the process attribute
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
    /*sorting according to arrival time*/
        for(int i=0; i<Processes_Queue.size(); i++){
            for(int j=0; j<Processes_Queue.size(); j++){
                if(Processes_Queue[j]->Arrival_Time >  Processes_Queue[i]->Arrival_Time){
                    Process *temp=Processes_Queue[j];
                    Processes_Queue[j]=Processes_Queue[i];
                    Processes_Queue[i]= temp;
                }
            }
        }

        int time =Processes_Queue[0]->Arrival_Time;
        int width_Prev=0;


        while(Processes_Queue.size() !=0){
            QVector<Process *> ready_processes;
            for(int i=0; i<Processes_Queue.size();i++){
                /**check the ready processes**/
                if(Processes_Queue[i]->Arrival_Time<=time){

                    ready_processes.push_back(Processes_Queue[i]);
                }
             }

                // If only one process is ready draw it
                if(ready_processes.size() == 1){
                    //draw
                    qDebug()<<ready_processes[0]->ID;
                    draw_process = new QLabel();
                    draw_process->setText(tr("P %1").arg(ready_processes[0]->ID));
                    draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
                    draw_process->setGeometry(300+width_Prev,700,ready_processes[0]->Burst_Time*80,50);
                    this->layout()->addWidget(draw_process);
                    //                            THIS IS THE PART OF THE DRAWRING SCENE
                    //                            Scene->addWidget(draw_process);
                    width_Prev=ready_processes[0]->Burst_Time*80;
                    time= time + ready_processes[0]->Burst_Time;
                    for(int x=0;x<Processes_Queue.size();x++){
                        if(Processes_Queue[x]->ID == ready_processes[0]->ID){
                           Processes_Queue.erase(Processes_Queue.begin()+x);
                           break;
                        }
                     }

                }
                else if (ready_processes.size()==0)  continue;

                /**if more than 1 process is ready compare their burst time**/
                else{
                    int min_burst= ready_processes[0]->Burst_Time;
                    for(int j=0; j<ready_processes.size();j++){
                       if(ready_processes[j]->Burst_Time < min_burst) min_burst=ready_processes[j]->Burst_Time;
                    }

                    for(int j=0; j<ready_processes.size();j++){
                        //el min burst ersmha we ems7ha mn el Queue
                        if(ready_processes[j]->Burst_Time == min_burst){
                            qDebug()<<ready_processes[j]->ID;

                            //Draw
                            draw_process = new QLabel();
                            draw_process->setText(tr("P %1").arg(ready_processes[j]->ID));
                            draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
                            draw_process->setGeometry(300+width_Prev +500,700,ready_processes[j]->Burst_Time*80,50);
                            this->layout()->addWidget(draw_process);

//                            THIS IS THE PART OF THE DRAWRING SCENE
//                            Scene->addWidget(draw_process);

                            width_Prev=ready_processes[j]->Burst_Time*80;

                            time = time + ready_processes[j]->Burst_Time;
                            for(int x=0;x<Processes_Queue.size();x++){
                                if(Processes_Queue[x]->ID == ready_processes[j]->ID){
                                   Processes_Queue.erase(Processes_Queue.begin()+x);
                                   break;
                                }
                             }
                            break;
                        }

                    }
                }
        }
}

void MainWindow::SJF_P_Alg(){
    qDebug()<<"Preemptive";

    //intializing remaining time = burst time
    for(int i=0; i<Processes_Queue.size(); i++){
        Processes_Queue[i]->Remaining_Time=Processes_Queue[i]->Burst_Time;
    }


    /*sorting according to arrival time*/
        for(int i=0; i<Processes_Queue.size(); i++){
            for(int j=0; j<Processes_Queue.size(); j++){
                if(Processes_Queue[j]->Arrival_Time >  Processes_Queue[i]->Arrival_Time){
                    Process *temp=Processes_Queue[j];
                    Processes_Queue[j]=Processes_Queue[i];
                    Processes_Queue[i]= temp;
                }
            }
        }

       // int time =Processes_Queue[0]->Arrival_Time;
        int width_Prev=0;


        for(int time =Processes_Queue[0]->Arrival_Time; Processes_Queue.size()!=0; time++ ){
            qDebug()<<"HI";
            QVector<Process *> ready_processes;
            for(int i=0; i<Processes_Queue.size();i++){
                /**check the ready processes**/
                if(Processes_Queue[i]->Arrival_Time<=time){

                    ready_processes.push_back(Processes_Queue[i]);
                }
             }

                // If only one process is ready draw it
                if(ready_processes.size() == 1){
                    //draw
                    qDebug()<<ready_processes[0]->ID;
                    draw_process = new QLabel();
                    draw_process->setText(tr("P %1").arg(ready_processes[0]->ID));
                    draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
                    draw_process->setGeometry(300+width_Prev,700,100,50);
                    this->layout()->addWidget(draw_process);
                    width_Prev=width_Prev+100;

                    for(int x=0;x<Processes_Queue.size();x++){
                        if(Processes_Queue[x]->ID == ready_processes[0]->ID){
                           Processes_Queue[x]->Remaining_Time --;
                           if(Processes_Queue[x]->Remaining_Time == 0){
                               Processes_Queue.erase(Processes_Queue.begin()+x);
                           }
                           break;
                        }
                     }


                }
                else if (ready_processes.size()==0)  continue;

                /**if more than 1 process is ready compare their remaining time**/
                else{
                    int min_remaining_time= ready_processes[0]->Burst_Time;
                    for(int j=0; j<ready_processes.size();j++){
                       if(ready_processes[j]->Remaining_Time < min_remaining_time) min_remaining_time=ready_processes[j]->Remaining_Time;
                    }

                    for(int j=0; j<ready_processes.size();j++){
                        //el min burst ersmha we ems7ha mn el Queue
                        if(ready_processes[j]->Remaining_Time == min_remaining_time){
                            qDebug()<<ready_processes[j]->ID;

                            //Draw
                            draw_process = new QLabel();
                            draw_process->setText(tr("P %1").arg(ready_processes[j]->ID));
                            draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
                            draw_process->setGeometry(300+width_Prev +500,700,100,50);
                            this->layout()->addWidget(draw_process);
                            width_Prev=width_Prev+100;


                            for(int x=0;x<Processes_Queue.size();x++){
                                if(Processes_Queue[x]->ID == ready_processes[j]->ID){
                                    Processes_Queue[x]->Remaining_Time --;
                                    if(Processes_Queue[x]->Remaining_Time == 0) Processes_Queue.erase(Processes_Queue.begin()+x);
                                   break;
                                }
                             }
                            break;
                        }

                    }
                }
        }

}

void MainWindow::FCFS_Alg(){
    /*sorting according to arrival time*/
        for(int i=0; i<Processes_Queue.size(); i++){
            for(int j=0; j<Processes_Queue.size(); j++){
                if(Processes_Queue[j]->Arrival_Time >  Processes_Queue[i]->Arrival_Time){
                    Process *temp=Processes_Queue[j];
                    Processes_Queue[j]=Processes_Queue[i];
                    Processes_Queue[i]= temp;
                }
            }
        }

        for(int i = 0; i<Processes_Queue.size(); i++) qDebug()<< Processes_Queue[i]->Process_name << " Arrived :" <<Processes_Queue[i]->Arrival_Time;
//        int time = 0;
//        for(int i = 0; i < Processes_Queue.size();i++)
//        {
//            if( (Processes_Queue[i]->Arrival_Time) > time)
//            {
//                Temp->p_next = "Gap";
//                Temp->p_width =(Processes_Queue[i]->Arrival_Time - time );
//                DrawingQueueFCFS.push_back(Temp);
//                time = Processes_Queue[i]->Arrival_Time;
//                qDebug()<<"Gap";
//            }else
//            {
//                Temp->p_next = Processes_Queue[i]->Process_name;
//                Temp->p_width = (Processes_Queue[i]->Burst_Time);
//                DrawingQueueFCFS.push_back(Temp);
//                time = time + Processes_Queue[i]->Burst_Time;
//                qDebug()<< Processes_Queue[i]->Process_name;
//            }
//        }
//        int time =Processes_Queue[0]->Arrival_Time;
//        int width_Prev=0;


//        while(Processes_Queue.size() !=0){
//            QVector<Process *> ready_processes;
//            for(int i=0; i<Processes_Queue.size();i++){
//                /**check the ready processes**/
//                if(Processes_Queue[i]->Arrival_Time<=time){

//                    ready_processes.push_back(Processes_Queue[i]);
//                }
//             }

//                // If only one process is ready draw it
//                if(ready_processes.size() == 1){
//                    //draw
//                    qDebug()<<ready_processes[0]->ID;
//                    draw_process = new QLabel();
//                    draw_process->setText(tr("P %1").arg(ready_processes[0]->ID));
//                    draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
//                    draw_process->setGeometry(300+width_Prev,700,ready_processes[0]->Burst_Time*80,50);
//                    this->layout()->addWidget(draw_process);
//                    //                            THIS IS THE PART OF THE DRAWRING SCENE
//                    //                            Scene->addWidget(draw_process);
//                    width_Prev=ready_processes[0]->Burst_Time*80;
//                    time= time + ready_processes[0]->Burst_Time;
//                    for(int x=0;x<Processes_Queue.size();x++){
//                        if(Processes_Queue[x]->ID == ready_processes[0]->ID){
//                           Processes_Queue.erase(Processes_Queue.begin()+x);
//                           break;
//                        }
//                     }

//                }
//                else if (ready_processes.size()==0)  continue;

//                /**if more than 1 process is ready compare their burst time**/
//                else{
//                    int min_burst= ready_processes[0]->Burst_Time;
//                    for(int j=0; j<ready_processes.size();j++){
//                       if(ready_processes[j]->Burst_Time < min_burst) min_burst=ready_processes[j]->Burst_Time;
//                    }

//                    for(int j=0; j<ready_processes.size();j++){
//                        //el min burst ersmha we ems7ha mn el Queue
//                        if(ready_processes[j]->Burst_Time == min_burst){
//                            qDebug()<<ready_processes[j]->ID;

//                            //Draw
//                            draw_process = new QLabel();
//                            draw_process->setText(tr("P %1").arg(ready_processes[j]->ID));
//                            draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
//                            draw_process->setGeometry(300+width_Prev +500,700,ready_processes[j]->Burst_Time*80,50);
//                            this->layout()->addWidget(draw_process);

////                            THIS IS THE PART OF THE DRAWRING SCENE
////                            Scene->addWidget(draw_process);

//                            width_Prev=ready_processes[j]->Burst_Time*80;

//                            time = time + ready_processes[j]->Burst_Time;
//                            for(int x=0;x<Processes_Queue.size();x++){
//                                if(Processes_Queue[x]->ID == ready_processes[j]->ID){
//                                   Processes_Queue.erase(Processes_Queue.begin()+x);
//                                   break;
//                                }
//                             }
//                            break;
//                        }

//                    }
//                }
//        }
}

void MainWindow::RR_Alg()
{
    /*sorting according to arrival time*/
    for(int i=0; i<Processes_Queue.size(); i++)
    {
        for(int j=0; j<Processes_Queue.size(); j++)
        {
            if(Processes_Queue[j]->Arrival_Time >  Processes_Queue[i]->Arrival_Time)
            {
                Process *temp=Processes_Queue[j];
                Processes_Queue[j]=Processes_Queue[i];
                Processes_Queue[i]= temp;
            }
        }
    }

    //initializing remaining time = burst time
    for(int s=0; s<Processes_Queue.size(); s++)
    {
        Processes_Queue[s]->Remaining_Time=Processes_Queue[s]->Burst_Time;
    }
    qDebug()<< "HI " ;

    int time = 0;
    int width_Prev=0;
    while(1)
    {
        bool vector_done = true;
        qDebug()<< "HI 2";
        //looping through all processes
        for(int k = 0; k < Processes_Queue.size(); k++)
        {
            qDebug()<< "process no:" << k;
            if( Processes_Queue[k]->Remaining_Time> 0)
            {
                vector_done = false;
                int l =Processes_Queue[k]->Remaining_Time;
                int p=Processes_Queue[k]->quantum_time;
                if(Processes_Queue[k]->Remaining_Time > Processes_Queue[k]->quantum_time)
                {
                    //increasing time by a quantum and drawing that time
                    time+=Processes_Queue[k]->quantum_time;

                    draw_process = new QLabel();
                    draw_process->setText(tr("P %1").arg(Processes_Queue[k]->ID));
                    draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
                    draw_process->setGeometry(300+width_Prev,700,Processes_Queue[k]->quantum_time*80,50);
                    this->layout()->addWidget(draw_process);
                    width_Prev=Processes_Queue[k]->quantum_time*80;

                    //decreasing remaining time by quantum
                    Processes_Queue[k]->Remaining_Time -= Processes_Queue[k]->quantum_time;
                }

                else
                {
                    time += Processes_Queue[k]->Remaining_Time;

                    //drawing the remaining time
                    draw_process = new QLabel();
                    draw_process->setText(tr("P %1").arg(Processes_Queue[k]->ID));
                    draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
                    draw_process->setGeometry(300+width_Prev,700,Processes_Queue[k]->Remaining_Time*80,50);
                    this->layout()->addWidget(draw_process);
                    width_Prev=Processes_Queue[k]->quantum_time*80;

                    //waiting time will be burst time subtracted from current time
                    Processes_Queue[k]->Waiting_Time = time - Processes_Queue[k]->Burst_Time;


                    //making remaining time = 0 to indicate process is done
                    Processes_Queue[k]->Remaining_Time=0;
                }
            }
        }

        if(vector_done == true)
            break;
    }

}
