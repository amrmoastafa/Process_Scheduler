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
    view->setAlignment(Qt::AlignLeft);

    scene_toolbar= new QGraphicsScene();
    view_toolbar= new QGraphicsView(scene_toolbar,this);
    view_toolbar->setGeometry(0,0,300,1000);
    view_toolbar->setStyleSheet("QGraphicsView{ background-color:rgb(78,204,163);  color:black; }");
    //Adding Tool bar

    toolbar = new QToolBar();
    toolbar->setStyleSheet("QToolBar{ background-color:rgb(78,204,163);  color:black; }");
    toolbar->setGeometry(0,0,300,1000);
    this->scene_toolbar->addWidget(toolbar);

    /****type of scheduler*****/
    type= new QLabel();
    type->setText("Scheduler Type");
    type->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    type->setGeometry(10,30,120,30);
    this->scene_toolbar->addWidget(type);

    Algorithm_dropdown = new QComboBox();
    Algorithm_dropdown->setStyleSheet("background-color:white;");
    Algorithm_dropdown->setGeometry(150,30,120,30);
    Algorithm_dropdown->addItems({"FCFS","SJF","Round Robin","Priority"});
    this->scene_toolbar->addWidget(Algorithm_dropdown);

    /******number of process input*****/
    N_process= new QLabel ();
    N_process->setText("Number of process");
    N_process->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    N_process->setGeometry(10,130,130,30);
    this->scene_toolbar->addWidget(N_process);

    num_process_line_edit= new QLineEdit();
    num_process_line_edit->setGeometry(150,130,120,30);
    num_process_line_edit->setStyleSheet("background-color:white;");
    QString p= num_process_line_edit->text();
    this->scene_toolbar->addWidget(num_process_line_edit);


    Ok = new QPushButton("OK");
    Ok->setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius:50%; border-width:0.2px;border-style: solid; border-color:white;} "
                      "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");
    Ok->setGeometry(60,180,210,40);
    this->scene_toolbar->addWidget(Ok);

    connect(Ok,SIGNAL(clicked()),this,SLOT(get_param())) ;

    restart = new QPushButton("♺");
    restart->setStyleSheet(" QPushButton{ background-color:rgb(208,47,47); color:white; font-size: 17px; font-family: Arial;border-radius: 50%;} "
                      "QPushButton:hover { background-color: white; border-radius:50%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");
    restart->setGeometry(20,180,30,40);
    this->scene_toolbar->addWidget(restart);
    connect(restart,SIGNAL(clicked()),this,SLOT(again())) ;

    Avg_label= new QLabel();
    Avg_label->setStyleSheet("color:rgb(78,204,163); background-color:rgb(128,128,128);font-size: 40px;");
    Avg_label->setGeometry(500,50,700,70);

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

    if(num_process_chosen.split(" ")[0].toInt() == 0){
        msg_box = new QMessageBox();
        msg_box->setText("Please Enter Number Of Processes :( ");
        msg_box->exec();
    }
    else{
        //height is a variable used to adjust the simulate button after the qline-edits are drawn
        int height=0;


        if (this->Alg_chosen == "SJF") height=SJF_layout();
        else if (this->Alg_chosen == "FCFS") height=FCFS_layout();
        else if (this->Alg_chosen == "Round Robin") height=RR_layout();
        else if (this->Alg_chosen == "Priority") height=PRIORITY_layout();


        Simulate = new QPushButton("Simulate");

        Simulate->setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius: 4px;} "
                                "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");

        Simulate->setGeometry(40,height+320,210,40);

        this->scene_toolbar->addWidget(Simulate);

        connect(Simulate,SIGNAL(clicked()),this,SLOT(Get_Text()));
    }

}


/*Function to get the values stored in the burst and arrival time vectors*/
/*also to execute the chose algorithm*/
void MainWindow::Get_Text()
{
    //We can use qDebug() Function to view variables as follows:
    //qDebug()<<"This is the value of first element of burst time vector : " <<burst_time[0]->text();
    //End
    int check_flag=1;
    for(int j = 0; j<burst_time.size(); j++)
    {
        if((arrival_time[j]->text().at(0) >=97 && arrival_time[j]->text().at(0)<=122)  || (burst_time[j]->text().at(0)>=97 && burst_time[j]->text().at(0) <=122)){
            msg_box = new QMessageBox();
            msg_box->setText("This is not a valid number!");
            msg_box->exec();
            check_flag=0;
            break;
        }
        else{
            Processes_Queue[j]->Arrival_Time = (arrival_time[j]->text().toFloat());

            qDebug() <<"Process : " <<Processes_Queue[j]->Process_name<<" Arrival Time is : "<<Processes_Queue[j]->Arrival_Time;

            Processes_Queue[j]->Burst_Time = (burst_time[j]->text().toFloat());//getting the text written (burst time of process j) from line edit of j
            Processes_Queue[j]->ID =j;

            qDebug() <<"Process : " <<Processes_Queue[j]->Process_name<<" Burst Time is : "<<Processes_Queue[j]->Burst_Time;
            if(Algorithm_dropdown->currentText() == "Priority")
            {
                //Processes_Queue[j]->Priority = ((priority_vect[j]->currentIndex())+1);
                Processes_Queue[j]->Priority=Priority_t[j]->text().toInt();

                qDebug() <<"Process : " <<Processes_Queue[j]->Process_name<<" Priority is : "<<Processes_Queue[j]->Priority;

            }
        }


    }

    if(check_flag){
        /****choose the Algorithm to be executed****/
         if(Alg_chosen == "SJF" && Preemptive_Checkbox->isChecked() ) SJF_P_Alg();
         else if (Alg_chosen == "SJF" && !Preemptive_Checkbox->isChecked()) SJF_NONP_Alg();
         else if (Alg_chosen == "FCFS") FCFS_Alg();
         else if(Alg_chosen == "Priority")
         {

             if(Preemptive_Checkbox->isChecked())
             {
                 Priority_AlgP();
             }else
             {
                 Priority_AlgNP();
             }
         }
         else if (Alg_chosen == "Round Robin")
         {
             /*Initializing quantum time*/
             for(int l=0; l<Processes_Queue.size();l++)
             {
                 Processes_Queue[l]->quantum_time=time_quantum_input->text().toFloat();
             }
             RR_Alg();
         }
    }

}


/*Layout for each algorithm
 * each one return the height of the simulate button
 */


int MainWindow::SJF_layout(){
    /*setting the Arrival an burst time label*/
    arrival_label = new QLabel();
    burst_label = new QLabel();
    arrival_label->setGeometry(70,250,80,30);
    burst_label->setGeometry(180,250,80,30);
    arrival_label->setText("Arrival Time");
    burst_label->setText("Burst Time");
    arrival_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    burst_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");

    this->scene_toolbar->addWidget(arrival_label);
    this->scene_toolbar->addWidget(burst_label);
    //this->layout()->addWidget(burst_label);
    //this->layout()->addWidget(arrival_label);
    //toolbar->addWidget(arrival_label);


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
        ID_Process->setGeometry(20,300+height,30,30);
        ID_Process->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
        arrival_input->setGeometry(60,300+height,80,30);
        burst_input->setGeometry(170,300+height,80,30);

        this->scene_toolbar->addWidget(arrival_input);
        this->scene_toolbar->addWidget(burst_input);
        this->scene_toolbar->addWidget(ID_Process);
        //this->layout()->addWidget(arrival_input);
        //this->layout()->addWidget(burst_input);
        //this->layout()->addWidget(ID_Process);
        height+=50;

    }
    Preemptive_Checkbox = new QCheckBox();
    Preemptive_Checkbox->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    Preemptive_Checkbox->setGeometry(80,height+320,18,18);
    Preemptive_label = new QLabel("Pre-emptive");
    Preemptive_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    Preemptive_label->setGeometry(110,height+320,80,18);
    this->scene_toolbar->addWidget(Preemptive_label);
    this->scene_toolbar->addWidget(Preemptive_Checkbox);
    //this->layout()->addWidget(Preemptive_Checkbox);
    //this->layout()->addWidget(Preemptive_label);
    height += 50;

    return height;
}

int MainWindow::FCFS_layout(){
    /*setting the Arrival an burst time label*/
    arrival_label = new QLabel();
    burst_label = new QLabel();
    arrival_label->setGeometry(70,250,80,30);
    burst_label->setGeometry(180,250,80,30);
    arrival_label->setText("Arrival Time");
    burst_label->setText("Burst Time");
    arrival_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    burst_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    this->scene_toolbar->addWidget(burst_label);
    this->scene_toolbar->addWidget(arrival_label);

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
        ID_Process->setGeometry(20,300+height,30,30);
        ID_Process->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
        arrival_input->setGeometry(60,300+height,80,30);
        burst_input->setGeometry(170,300+height,80,30);

        this->scene_toolbar->addWidget(arrival_input);
        this->scene_toolbar->addWidget(burst_input);
        this->scene_toolbar->addWidget(ID_Process);
        height+=50;

    }

    return height;
}

int MainWindow::RR_layout(){
    /*Time Quantum*/
    time_quantum_label = new QLabel();
    time_quantum_label->setGeometry(50,250,100,30);

    time_quantum_label->setText("Time Quantum");
    time_quantum_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");

    validator = new QDoubleValidator ();
    time_quantum_input = new QLineEdit();
    time_quantum_input->setValidator(validator);
    time_quantum_input->setGeometry(175,250,80,30);

    this->scene_toolbar->addWidget(time_quantum_label);
    this->scene_toolbar->addWidget(time_quantum_input);

    /*setting the Arrival an burst time label*/
    arrival_label = new QLabel("Arrival Time");
    burst_label = new QLabel("Burst Time");

    arrival_label->setGeometry(70,300,80,30);
    burst_label->setGeometry(180,300,80,30);

    arrival_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    burst_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");

    this->scene_toolbar->addWidget(burst_label);
    this->scene_toolbar->addWidget(arrival_label);

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
        ID_Process->setGeometry(20,320+height,30,30);
        ID_Process->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
        arrival_input->setGeometry(60,320+height,80,30);
        burst_input->setGeometry(170,320+height,80,30);

        this->scene_toolbar->addWidget(arrival_input);
        this->scene_toolbar->addWidget(burst_input);
        this->scene_toolbar->addWidget(ID_Process);
        height+=50;

    }

    qDebug("RR");
    return height;
}

int MainWindow::PRIORITY_layout(){
    /*setting the Arrival an burst time label*/
    arrival_label = new QLabel("Arrival Time");
    burst_label = new QLabel("Burst Time");
    arrival_label->setGeometry(40,250,80,30);
    burst_label->setGeometry(130,250,80,30);

    priortiy_label = new QLabel("Priority");
    priortiy_label->setGeometry(220,250,50,30);
    priortiy_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    this->scene_toolbar->addWidget(priortiy_label);




    arrival_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    burst_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");

    this->scene_toolbar->addWidget(burst_label);
    this->scene_toolbar->addWidget(arrival_label);

    int height=0;
    for (int i = 0; i<this->num_process_chosen.split(" ")[0].toInt(); i++)//convert from QString to int
    {
        /***creating a line edit and pushing the created line edit to a vector***/
        arrival_input = new QLineEdit();
        burst_input = new QLineEdit();
        validator = new QDoubleValidator();

        priority_input = new QLineEdit();
        //priority_vect.push_back(priority_input);
        //priority_input->addItems({"1","2","3","4","5"});
        priority_input->setGeometry(220,300+height,40,30);
        priority_input->setValidator(validator);

        this->layout()->addWidget(priority_input);

        Process *p = new Process;

        //Creating a pointer to a process to carry any random address initially given by compiler
        Processes_Queue.append(p);
        // Appending that address to the pointer vector
        burst_time.push_back(burst_input);
        arrival_time.push_back(arrival_input);
        Priority_t.push_back(priority_input);

        ID_Process = new QLabel();

        ID_Process->setText(tr("P%1").arg(i));
        //adding the name of the process attribute
        Processes_Queue[i]->Process_name = ID_Process->text();
        ID_Process->setGeometry(20,300+height,30,30);
        ID_Process->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
        arrival_input->setGeometry(60,300+height,60,30);
        burst_input->setGeometry(140,300+height,60,30);

        this->scene_toolbar->addWidget(priority_input);
        this->scene_toolbar->addWidget(arrival_input);
        this->scene_toolbar->addWidget(burst_input);
        this->scene_toolbar->addWidget(ID_Process);;
        height+=50;

    }
    Preemptive_Checkbox = new QCheckBox();
    Preemptive_Checkbox->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    Preemptive_Checkbox->setGeometry(80,height+320,18,18);
    Preemptive_label = new QLabel("Pre-emptive");
    Preemptive_label->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
    Preemptive_label->setGeometry(110,height+320,80,18);
    this->scene_toolbar->addWidget(Preemptive_Checkbox);
    this->scene_toolbar->addWidget(Preemptive_label);
    height += 50;
    return height;
}

void MainWindow::SJF_NONP_Alg(){
    view->setAlignment(Qt::AlignLeft);
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
        for(int i=0;i<Processes_Queue.size();i++){
            Processes_Queue[i]->Waiting_Time=0;
        }

        QVector <Process *> arrive;
        for(int i=0;i<Processes_Queue.size();i++){
            arrive.push_back(Processes_Queue[i]);
        }

        //int time =Processes_Queue[0]->Arrival_Time;
        float time=0;
        float width_Prev=0;

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
                    draw_process->setGeometry(width_Prev,700,ready_processes[0]->Burst_Time*60,70);
                    draw_time = new QLabel();
                    draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                    draw_time->setText(tr(" %1").arg(time));
                    draw_time->setGeometry(width_Prev,750,50,50);
                    this->Scene->addWidget(draw_time);
                    this->Scene->addWidget(draw_process);
                    //this->layout()->addWidget(draw_process);
                    //                            THIS IS THE PART OF THE DRAWRING SCENE
                     //       Scene->addWidget(draw_process);

                    width_Prev=width_Prev + ready_processes[0]->Burst_Time*60;
                    for(int s=0;s<arrive.size();s++){
                        if(ready_processes[0]->ID == arrive[s]->ID) arrive[s]->Waiting_Time=time-arrive[s]->Arrival_Time;
                    }
                    time= time + ready_processes[0]->Burst_Time;
                    for(int x=0;x<Processes_Queue.size();x++){
                        if(Processes_Queue[x]->ID == ready_processes[0]->ID){
                           Processes_Queue.erase(Processes_Queue.begin()+x);
                           break;
                        }
                     }

                }
                else if (ready_processes.size()==0) {
                    //draw gap
                    draw_process = new QLabel();
                    draw_process->setText("GAP");
                    draw_process->setStyleSheet("background-color:white;color:black; border-width: 2px; border-style: solid; border-color: gray;");
                    draw_process->setGeometry(width_Prev ,700,1*60,70);
                    //this->layout()->addWidget(draw_process);
                    draw_time = new QLabel();
                    draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                    draw_time->setText(tr(" %1").arg( time));
                    draw_time->setGeometry(width_Prev,750,60,50);
                    this->Scene->addWidget(draw_time);
                    this->Scene->addWidget(draw_process);

//                            THIS IS THE PART OF THE DRAWRING SCENE
//                            Scene->addWidget(draw_process);
                    width_Prev=width_Prev + 1*60;

                    time = time + 1;
                }

                /**if more than 1 process is ready compare their burst time**/
                else{
                    QVector<int>IDs;

                    float min_burst= ready_processes[0]->Burst_Time;
                    for(int j=0; j<ready_processes.size();j++){
                       if(ready_processes[j]->Burst_Time < min_burst) min_burst=ready_processes[j]->Burst_Time;
                    }

                    for(int j=0; j<ready_processes.size();j++){
                        //el min burst ersmha we ems7ha mn el Queue
                        if(ready_processes[j]->Burst_Time == min_burst){
                            qDebug()<<ready_processes[j]->ID;
                            for(int s=0;s<arrive.size();s++){
                                if(ready_processes[j]->ID == arrive[s]->ID) arrive[s]->Waiting_Time=time-arrive[s]->Arrival_Time;
                            }

                            //Draw
                            draw_process = new QLabel();
                            draw_process->setText(tr("P %1").arg(ready_processes[j]->ID));
                            draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
                            draw_process->setGeometry(width_Prev ,700,ready_processes[j]->Burst_Time*60,70);
                            //this->layout()->addWidget(draw_process);
                            draw_time = new QLabel();
                            draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                            draw_time->setText(tr(" %1").arg(time));
                            draw_time->setGeometry(width_Prev,750,60,50);
                            this->Scene->addWidget(draw_time);
                            this->Scene->addWidget(draw_process);

//                            THIS IS THE PART OF THE DRAWRING SCENE
//                            Scene->addWidget(draw_process);
                            width_Prev=width_Prev + ready_processes[j]->Burst_Time*60;

                            time = time + ready_processes[j]->Burst_Time;
                            for(int x=0;x<Processes_Queue.size();x++){
                                if(Processes_Queue[x]->ID == ready_processes[j]->ID){
                                   Processes_Queue.erase(Processes_Queue.begin()+x);
                                   break;
                                }
                             }
                            break; //hat3ml bug b3d kda
                        }
                        else{
                        IDs.push_back(ready_processes[j]->ID);

                        }
                    }

                    /*for(int q=0; q<arrive.size();q++){
                        for(int w=0;w<IDs.size();w++){
                            if(arrive[q]->ID==IDs[w]){
                                arrive[q]->Waiting_Time=arrive[q]->Waiting_Time+min_burst;
                            }
                        }
                    }*/
                }
        }
        draw_time = new QLabel();
        draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
        draw_time->setText(tr(" %1").arg(time));
        draw_time->setGeometry(width_Prev,750,60,50);
        this->Scene->addWidget(draw_time);
        float sum=0;
        for(int q=0;q<arrive.size();q++){
            qDebug()<<arrive[q]->ID << "waiting time is"<<arrive[q]->Waiting_Time;
            sum=sum+arrive[q]->Waiting_Time;
        }

        float avg=sum/arrive.size();
        qDebug()<<avg;

        Avg_label->setText(tr("Average Waiting Time= %1").arg(avg));

        this->Scene->addWidget(Avg_label);

}

void MainWindow::SJF_P_Alg(){
    view->setAlignment(Qt::AlignLeft);
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
            for(int i=0;i<Processes_Queue.size();i++){
                Processes_Queue[i]->Waiting_Time=0;
                Processes_Queue[i]->Termination_Time=0;
            }

            QVector <Process *> arrive;
            for(int i=0;i<Processes_Queue.size();i++){
                arrive.push_back(Processes_Queue[i]);
            }

            QVector <Process *> DrawingSJFP;
           // int time =Processes_Queue[0]->Arrival_Time;
            float width_Prev=0;
            float time=0;
            int arrive_index=1;
            int flag=-1;
            while(Processes_Queue.size()!=0){
                QVector<Process *> ready_processes;
                for(int i=0; i<Processes_Queue.size();i++){
                    /**check the ready processes**/
                    if(Processes_Queue[i]->Arrival_Time<=time){
                        ready_processes.push_back(Processes_Queue[i]);
                    }
                 }

                    // If only one process is ready draw it
                    if(ready_processes.size() == 1){
                       // qDebug()<<ready_processes[0]->ID;

                        if (flag !=ready_processes[0]->ID){
                            //get waiting time
                            for(int s=0;s<arrive.size();s++){
                                if(ready_processes[0]->ID == arrive[s]->ID) {
                                    if(arrive[s]->Remaining_Time != arrive[s]->Burst_Time){
                                        arrive[s]->Waiting_Time=arrive[s]->Waiting_Time + (time-arrive[s]->Termination_Time);
                                    }
                                    else {
                                        arrive[s]->Waiting_Time=arrive[s]->Waiting_Time + time-arrive[s]->Arrival_Time;
                                    }

                                }
                            }
                        }

                        float start_index=time;

                        //adjust time
                        if(arrive_index<arrive.size()){
                            if(time+ready_processes[0]->Remaining_Time < arrive[arrive_index]->Arrival_Time){
                                time = time + ready_processes[0]->Remaining_Time;
                            }
                            else{
                                time=arrive[arrive_index]->Arrival_Time;
                                flag=ready_processes[0]->ID;
                            }
                            arrive_index++;
                        }
                        else{
                            time = time + ready_processes[0]->Remaining_Time;
                        }


                        for(int s=0;s<arrive.size();s++){
                            if(ready_processes[0]->ID == arrive[s]->ID) {
                                arrive[s]->Termination_Time=time;
                            }
                        }

                        draw_process = new QLabel();
                        draw_process->setText(tr("P %1").arg(ready_processes[0]->ID));
                        draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
                        draw_process->setGeometry(width_Prev,700,(time-start_index)*60,70);
                        this->Scene->addWidget(draw_process);
                        draw_time = new QLabel();
                        draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                        draw_time->setText(tr(" %1").arg(start_index));
                        draw_time->setGeometry(width_Prev,780,60,30);
                        this->Scene->addWidget(draw_time);
                        width_Prev=width_Prev + (time-start_index)*60;

                       // DrawingSJFP.push_back(ready_processes[0]);
                        for(int x=0;x<Processes_Queue.size();x++){
                            if(Processes_Queue[x]->ID == ready_processes[0]->ID){
                               Processes_Queue[x]->Remaining_Time =Processes_Queue[x]->Remaining_Time- (time-start_index);
                               if(Processes_Queue[x]->Remaining_Time == 0){
                                   Processes_Queue.erase(Processes_Queue.begin()+x);
                               }
                               break;
                            }
                         }
                    }

                    else if (ready_processes.size()==0){

                        float start_index=time;
                        time=arrive[arrive_index-1]->Arrival_Time;
                         //arrive_index++;
                         draw_process = new QLabel();
                         draw_process->setText("GAP");
                         draw_process->setStyleSheet("background-color:white;color:black;");
                         draw_process->setGeometry(width_Prev ,700,(time-start_index)*60,70);
                         this->Scene->addWidget(draw_process);
                         draw_time = new QLabel();
                         draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                         draw_time->setText(tr(" %1").arg(start_index));
                         draw_time->setGeometry(width_Prev,780,60,30);
                         this->Scene->addWidget(draw_time);
                         width_Prev=width_Prev + (time-start_index)*60;
                         Process *gap = new Process();
                         gap->ID=-1;
                         DrawingSJFP.push_back(gap);
                    }

                    /**if more than 1 process is ready compare their remaining time**/
                    else{
                        float min_remaining_time= ready_processes[0]->Burst_Time;
                        for(int j=0; j<ready_processes.size();j++){
                           if(ready_processes[j]->Remaining_Time < min_remaining_time) min_remaining_time=ready_processes[j]->Remaining_Time;
                        }

                        for(int j=0; j<ready_processes.size();j++){
                            //el min burst ersmha we ems7ha mn el Queue
                            if(ready_processes[j]->Remaining_Time == min_remaining_time){
                                //qDebug()<<ready_processes[j]->ID;

                                //calc avg
                                if(flag!=ready_processes[j]->ID){
                                    for(int s=0;s<arrive.size();s++){
                                        if(ready_processes[j]->ID == arrive[s]->ID){
                                            if(arrive[s]->Remaining_Time != arrive[s]->Burst_Time){
                                                arrive[s]->Waiting_Time=arrive[s]->Waiting_Time + (time-arrive[s]->Termination_Time);
                                            }
                                            else {
                                                arrive[s]->Waiting_Time=arrive[s]->Waiting_Time + time-arrive[s]->Arrival_Time;
                                            }

                                        }

                                    }

                                }

                                //set waiting time

                               float start_index=time;
                                //adjust time
                                if(arrive_index < arrive.size()){
                                    if(time+ready_processes[j]->Remaining_Time < arrive[arrive_index]->Arrival_Time){

                                        time = time + ready_processes[j]->Remaining_Time;
                                    }
                                    else {
                                        time=arrive[arrive_index]->Arrival_Time;
                                        flag = ready_processes[j]->ID;
                                        arrive_index++;
                                    }

                                }
                                else{
                                    time = time + ready_processes[j]->Remaining_Time;
                                    flag = ready_processes[j]->ID;
                                }

                                for(int s=0;s<arrive.size();s++){
                                    if(ready_processes[j]->ID == arrive[s]->ID) {
                                        arrive[s]->Termination_Time=time;
                                    }
                                }
                                //Draw
                                draw_process = new QLabel();
                                draw_process->setText(tr("P %1").arg(ready_processes[j]->ID));
                                draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
                                draw_process->setGeometry(width_Prev,700,(time-start_index)*60,70);
                                this->Scene->addWidget(draw_process);
                                draw_time = new QLabel();
                                draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                                draw_time->setText(tr(" %1").arg(start_index));
                                draw_time->setGeometry(width_Prev,780,60,30);
                                this->Scene->addWidget(draw_time);
                                width_Prev=width_Prev + (time-start_index)*60;

                                qDebug()<<time;
                                //DrawingSJFP.push_back(ready_processes[j]);
                                for(int x=0;x<Processes_Queue.size();x++){
                                    if(Processes_Queue[x]->ID == ready_processes[j]->ID){
                                        Processes_Queue[x]->Remaining_Time =Processes_Queue[x]->Remaining_Time- (time-start_index);
                                        if(Processes_Queue[x]->Remaining_Time == 0) Processes_Queue.erase(Processes_Queue.begin()+x);
                                       break;
                                    }
                                 }
                                break;
                            }

                        }
                    }
            }

            draw_time = new QLabel();
            draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
            draw_time->setText(tr(" %1").arg(time));
            draw_time->setGeometry(width_Prev,780,60,30);
            this->Scene->addWidget(draw_time);

            float sum=0;
            for(int q=0;q<arrive.size();q++){
                sum=sum+(arrive[q]->Termination_Time-arrive[q]->Burst_Time-arrive[q]->Arrival_Time);
            }
            Avg_label= new QLabel();
            float avg=sum/arrive.size();
            qDebug()<<avg;
            Avg_label->setStyleSheet("color:rgb(78,204,163); background-color:rgb(128,128,128);font-size: 40px;");
            Avg_label->setText(tr("Average Waiting Time= %1").arg(avg));
            Avg_label->setGeometry(500,50,700,70);
            this->Scene->addWidget(Avg_label);
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
            if(Processes_Queue[j]->Arrival_Time ==  Processes_Queue[i]->Arrival_Time)
            {
                if(Processes_Queue[i]->Process_name < Processes_Queue[j]->Process_name)
                {
                    //Swap alphapetical
                    Process *temp=Processes_Queue[j];
                    Processes_Queue[j]=Processes_Queue[i];
                    Processes_Queue[i]= temp;
                }
            }
        }
    }
    Temp = new DrawingQueue;
    for(int i = 0; i<Processes_Queue.size(); i++) qDebug()<< Processes_Queue[i]->Process_name << " FCFS Arrived :" <<Processes_Queue[i]->Arrival_Time;
    float time = 0;
    for(int i = 0; i < Processes_Queue.size();i++)
    {

        if( (Processes_Queue[i]->Arrival_Time) > time)
        {
            qDebug()<<"Gap";
            Temp->p_next = "Gap";Temp->p_width = ((Processes_Queue[i]->Arrival_Time) - time );Temp->time_start = time;
            DrawingQueueFCFS.append(*Temp);
            time = time + Temp->p_width;
            qDebug()<<"Gap width : " <<Temp->p_width;
            Temp->p_next = Processes_Queue[i]->Process_name;
            Temp->p_width = Processes_Queue[i]->Burst_Time;
            Temp->time_start = time;
            DrawingQueueFCFS.append(*Temp);
            time = time + Temp->p_width;
            qDebug()<<Temp->p_next<<"Width:"<<Temp->p_width<<"Start"<<Temp->time_start;
        }else if((Processes_Queue[i]->Arrival_Time) == time)
        {
            qDebug()<<"Arrived on time";
            Temp->p_next = Processes_Queue[i]->Process_name;
            Temp->p_width = Processes_Queue[i]->Burst_Time;
            Temp->time_start = time;
            time = time + Temp->p_width;
            DrawingQueueFCFS.append(*Temp);
            qDebug()<<Temp->p_next<<"Width:"<<Temp->p_width<<"Start"<<Temp->time_start;
        }
        else if( (Processes_Queue[i]->Arrival_Time) < time)
        {
            qDebug()<<"Arrived at same time";
            Temp->p_next = Processes_Queue[i]->Process_name;
            Temp->p_width = Processes_Queue[i]->Burst_Time;
            Temp->time_start = time;
            time = time + Temp->p_width;DrawingQueueFCFS.append(*Temp);
            qDebug()<<Temp->p_next<<"Width:"<<Temp->p_width<<"Start"<<Temp->time_start;
        }
    }
    for(int i = 0; i<DrawingQueueFCFS.size(); i++) qDebug()<< DrawingQueueFCFS[i].p_next << "FCFS Arrived :" <<DrawingQueueFCFS[i].p_width;
    time = 0;
    qDebug()<<DrawingQueueFCFS.size();
    for(int i = 0 ; i < DrawingQueueFCFS.size();i++)
    {
        Process_drawn = new QPushButton();
        Process_drawn->setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius: 10%;} "
                                     "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");

        draw_time = new QLabel();
        draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");

        qDebug()<<DrawingQueueFCFS[i].p_next;
        if(DrawingQueueFCFS[i].p_next == "Gap")
        {
            qDebug() <<"Entered Gap";
            Process_drawn->setGeometry( time,0,(DrawingQueueFCFS[i].p_width*25),100);
            Process_drawn->setText(DrawingQueueFCFS[i].p_next);
            Process_drawn->setStyleSheet(" QPushButton{ background-color:white; color:black; font-size: 17px; font-family: Arial;border-radius: 10%;} "
                                         "QPushButton:hover { background-color: black; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:white;} ");
            Scene->addWidget(Process_drawn);

            draw_time->setText(QString::number(time/25));
            draw_time->setGeometry(time-5,100,(DrawingQueueFCFS[i].p_width*25),30);
            Scene->addWidget(draw_time);
            time += ((DrawingQueueFCFS[i].p_width*25));
        }else
        {
            Process_drawn->setGeometry(time,0,(DrawingQueueFCFS[i].p_width*25),100);
            Process_drawn->setText(DrawingQueueFCFS[i].p_next);
            Process_drawn->setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius: 10%;} "
                                         "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");
            Scene->addWidget(Process_drawn);
            draw_time->setText(QString::number(time/25));
            draw_time->setGeometry(time-5,100,(DrawingQueueFCFS[i].p_width*25),30);
            Scene->addWidget(draw_time);
            time += ((DrawingQueueFCFS[i].p_width*25));
        }
        if(i == DrawingQueueFCFS.size()-1)
        {
            draw_time = new QLabel();
            draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
            draw_time->setText(QString::number(time/25));
            draw_time->setGeometry(time-5,100,(DrawingQueueFCFS[i].p_width*25),30);
            Scene->addWidget(draw_time);
        }

    }
}

void MainWindow::Priority_AlgNP()
{
    /*sorting according to arrival time*/

    for(int k = 0 ; k < Processes_Queue.size();k++)
    {
        for(int i=0; i<Processes_Queue.size(); i++){


            for(int j=0; j<Processes_Queue.size(); j++){

                if(Processes_Queue[j]->Arrival_Time >  Processes_Queue[i]->Arrival_Time){
                    Process *temp=Processes_Queue[j];
                    Processes_Queue[j]=Processes_Queue[i];
                    Processes_Queue[i]= temp;

                }else if((Processes_Queue[j]->Arrival_Time == Processes_Queue[i]->Arrival_Time))
                {
                    if((Processes_Queue[i]->Priority < Processes_Queue[j]->Priority))
                    {
                        Process *temp=Processes_Queue[j];
                        Processes_Queue[j]=Processes_Queue[i];
                        Processes_Queue[i]= temp;
                    }

                }

            }
        }
    }
    float time;
            for(int initial = 0;initial <Processes_Queue.size();initial++)
    {
        time = Processes_Queue[initial]->Burst_Time + Processes_Queue[initial]->Arrival_Time;//Completion time of P0
        for(int i = 1; i < Processes_Queue.size();i++)
        {

            if(Processes_Queue[i]->Arrival_Time > time)
            {
                //The case where the next process is a gap so here the processes will get sorted
                /*Capturing all the processes that arrived during another process run time*/
                qDebug()<<"Process :"<<Processes_Queue[i]->Process_name<< "Arrived :"<<Processes_Queue[i]->Arrival_Time;
                qDebug()<<"Time"<<time;
                /* Now Sorting According To Priority */

                for(int k =initial+1; k<i;k++)
                {

                    for(int j = k+1 ; j <i ; j++)
                    {
                        if((Processes_Queue[k]->Priority > Processes_Queue[j]->Priority))
                        {
                            qDebug()<< "Process of [k] :"<<Processes_Queue[k]->Process_name << "Process of [j] :"<<Processes_Queue[j]->Process_name ;
                            Process *temp=Processes_Queue[j];
                            Processes_Queue[j]=Processes_Queue[k];
                            Processes_Queue[k]= temp;
                        }

                    }
                }
            }else if (i == (Processes_Queue.size() - 1))
            {
                /* in case we reached the end and didnt find a process with arrival time = burst on the one being executed*/
                qDebug()<< "reached the end without finding one greater";
                for(int k =initial+1; k<=i;k++)
                {

                    for(int j = k+1 ; j <=i ; j++)
                    {
                        if((Processes_Queue[k]->Priority > Processes_Queue[j]->Priority))
                        {
                            qDebug()<< "Process of [k] :"<<Processes_Queue[k]->Process_name << "Process of [j] :"<<Processes_Queue[j]->Process_name ;
                            Process *temp=Processes_Queue[j];
                            Processes_Queue[j]=Processes_Queue[k];
                            Processes_Queue[k]= temp;
                        }

                    }
                }
            }
        }}
//    for(int i=0; i<Processes_Queue.size(); i++){


//        for(int j=i+1; j<Processes_Queue.size(); j++)
//        {
//            if(Processes_Queue[i]->Priority > Processes_Queue[j]->Priority)
//            {
//                qDebug()<<"Trigerred";
//                Process *temp=Processes_Queue[j];
//                Processes_Queue[j]=Processes_Queue[i];
//                Processes_Queue[i]= temp;
//            }
//        }
//    }

    /* ARRANGING PROCESSES IN DRAWING QUEUE */
    Temp = new DrawingQueue;
    for(int i = 0; i<Processes_Queue.size(); i++) qDebug()<< Processes_Queue[i]->Process_name << " Priority Arrived :" <<Processes_Queue[i]->Arrival_Time << "Priority :"<<Processes_Queue[i]->Priority;
    time = 0;
    for(int i = 0; i < Processes_Queue.size();i++)
    {

        if( (Processes_Queue[i]->Arrival_Time) > time)
        {


            Temp->p_next = "Gap";Temp->p_width = ((Processes_Queue[i]->Arrival_Time) - time );Temp->time_start = time;
            DrawingQueueFCFS.append(*Temp);
            time = time + Temp->p_width;

            Temp->p_next = Processes_Queue[i]->Process_name;
            Temp->p_width = Processes_Queue[i]->Burst_Time;
            Temp->time_start = time;
            DrawingQueueFCFS.append(*Temp);
            time = time + Temp->p_width;


        }else if((Processes_Queue[i]->Arrival_Time) == time)
        {

            Temp->p_next = Processes_Queue[i]->Process_name;
            Temp->p_width = Processes_Queue[i]->Burst_Time;
            Temp->time_start = time;
            time = time + Temp->p_width;
            DrawingQueueFCFS.append(*Temp);

        }
        else if( (Processes_Queue[i]->Arrival_Time) < time)
        {
            Temp->p_next = Processes_Queue[i]->Process_name;
            Temp->p_width = Processes_Queue[i]->Burst_Time;
            Temp->time_start = time;
            time = time + Temp->p_width;
            DrawingQueueFCFS.append(*Temp);

        }
    }
    //        for(int i = 0; i<DrawingQueueFCFS.size(); i++) qDebug()<< DrawingQueueFCFS[i].p_next << " Arrived :" <<DrawingQueueFCFS[i].p_width;
    time = 0;

    for(int i = 0 ; i < DrawingQueueFCFS.size();i++)
    {
        Process_drawn = new QPushButton();
        Process_drawn->setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius: 10%;} "
                                     "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");

        draw_time = new QLabel();
        draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");

        if(DrawingQueueFCFS[i].p_next == "Gap")
        {

            Process_drawn->setGeometry( time,0,(DrawingQueueFCFS[i].p_width*25),100);
            Process_drawn->setText(DrawingQueueFCFS[i].p_next);
            Process_drawn->setStyleSheet(" QPushButton{ background-color:white; color:black; font-size: 17px; font-family: Arial;border-radius: 10%;} "
                                         "QPushButton:hover { background-color: black; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:white;} ");
            Scene->addWidget(Process_drawn);

            draw_time->setText(QString::number(time/25));
            draw_time->setGeometry(time-5,100,(DrawingQueueFCFS[i].p_width*25),30);
            Scene->addWidget(draw_time);

            time += ((DrawingQueueFCFS[i].p_width*25));
        }else
        {
            Process_drawn->setGeometry(time,0,(DrawingQueueFCFS[i].p_width*25),100);
            Process_drawn->setText(DrawingQueueFCFS[i].p_next);
            Process_drawn->setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius: 10%;} "
                                         "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");
            Scene->addWidget(Process_drawn);
            draw_time->setText(QString::number(time/25));
            draw_time->setGeometry(time-5,100,(DrawingQueueFCFS[i].p_width*25),30);
            Scene->addWidget(draw_time);
            time += ((DrawingQueueFCFS[i].p_width*25));
        }
        if(i == DrawingQueueFCFS.size()-1)
        {
            draw_time = new QLabel();
            draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
            draw_time->setText(QString::number(time/25));
            draw_time->setGeometry(time-5,100,(DrawingQueueFCFS[i].p_width*25),30);
            Scene->addWidget(draw_time);
        }
    }
}

void MainWindow::Priority_AlgP()
{
    view->setAlignment(Qt::AlignLeft);
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
            for(int i=0;i<Processes_Queue.size();i++){
                Processes_Queue[i]->Waiting_Time=0;
                Processes_Queue[i]->Termination_Time=0;
            }

            QVector <Process *> arrive;
            for(int i=0;i<Processes_Queue.size();i++){
                arrive.push_back(Processes_Queue[i]);
            }

            QVector <Process *> DrawingSJFP;
           // int time =Processes_Queue[0]->Arrival_Time;
            float width_Prev=0;
            float time=0;
            int arrive_index=1;
            int flag=-1;
            while(Processes_Queue.size()!=0){
                QVector<Process *> ready_processes;
                for(int i=0; i<Processes_Queue.size();i++){
                    /**check the ready processes**/
                    if(Processes_Queue[i]->Arrival_Time<=time){
                        ready_processes.push_back(Processes_Queue[i]);
                    }
                 }

                    // If only one process is ready draw it
                    if(ready_processes.size() == 1){
                       // qDebug()<<ready_processes[0]->ID;

                        float start_index=time;

                        //adjust time
                        if(arrive_index<arrive.size()){
                            if(time+ready_processes[0]->Remaining_Time < arrive[arrive_index]->Arrival_Time){
                                time = time + ready_processes[0]->Remaining_Time;
                            }
                            else{
                                time=arrive[arrive_index]->Arrival_Time;
                                flag=ready_processes[0]->ID;
                            }
                            arrive_index++;
                        }
                        else{
                            time = time + ready_processes[0]->Remaining_Time;
                        }


                        for(int s=0;s<arrive.size();s++){
                            if(ready_processes[0]->ID == arrive[s]->ID) {
                                arrive[s]->Termination_Time=time;
                            }
                        }

                        draw_process = new QLabel();
                        draw_process->setText(tr("P %1").arg(ready_processes[0]->ID));
                        draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
                        draw_process->setGeometry(width_Prev,700,(time-start_index)*60,70);
                        this->Scene->addWidget(draw_process);
                        draw_time = new QLabel();
                        draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                        draw_time->setText(tr(" %1").arg(start_index));
                        draw_time->setGeometry(width_Prev,780,60,30);
                        this->Scene->addWidget(draw_time);
                        width_Prev=width_Prev + (time-start_index)*60;

                       // DrawingSJFP.push_back(ready_processes[0]);
                        for(int x=0;x<Processes_Queue.size();x++){
                            if(Processes_Queue[x]->ID == ready_processes[0]->ID){
                               Processes_Queue[x]->Remaining_Time =Processes_Queue[x]->Remaining_Time- (time-start_index);
                               if(Processes_Queue[x]->Remaining_Time == 0){
                                   Processes_Queue.erase(Processes_Queue.begin()+x);
                               }
                               break;
                            }
                         }
                    }

                    else if (ready_processes.size()==0){

                        float start_index=time;
                        time=arrive[arrive_index-1]->Arrival_Time;
                         //arrive_index++;
                         draw_process = new QLabel();
                         draw_process->setText("GAP");
                         draw_process->setStyleSheet("background-color:white;color:black;");
                         draw_process->setGeometry(width_Prev ,700,(time-start_index)*60,70);
                         this->Scene->addWidget(draw_process);
                         draw_time = new QLabel();
                         draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                         draw_time->setText(tr(" %1").arg(start_index));
                         draw_time->setGeometry(width_Prev,780,60,30);
                         this->Scene->addWidget(draw_time);
                         width_Prev=width_Prev + (time-start_index)*60;
                         Process *gap = new Process();
                         gap->ID=-1;
                         DrawingSJFP.push_back(gap);
                    }

                    /**if more than 1 process is ready compare their remaining time**/
                    else{
                        float min_Priority= ready_processes[0]->Priority;
                        for(int j=0; j<ready_processes.size();j++){
                           if(ready_processes[j]->Priority < min_Priority) min_Priority=ready_processes[j]->Priority;
                        }

                        for(int j=0; j<ready_processes.size();j++){
                            //el min burst ersmha we ems7ha mn el Queue
                            if(ready_processes[j]->Priority == min_Priority){
                                //qDebug()<<ready_processes[j]->ID;

                                //set waiting time

                               float start_index=time;
                                //adjust time
                                if(arrive_index < arrive.size()){
                                    if(time+ready_processes[j]->Remaining_Time < arrive[arrive_index]->Arrival_Time){

                                        time = time + ready_processes[j]->Remaining_Time;
                                    }
                                    else {
                                        time=arrive[arrive_index]->Arrival_Time;
                                        flag = ready_processes[j]->ID;
                                        arrive_index++;
                                    }

                                }
                                else{
                                    time = time + ready_processes[j]->Remaining_Time;
                                    flag = ready_processes[j]->ID;
                                }

                                for(int s=0;s<arrive.size();s++){
                                    if(ready_processes[j]->ID == arrive[s]->ID) {
                                        arrive[s]->Termination_Time=time;
                                    }
                                }
                                //Draw
                                draw_process = new QLabel();
                                draw_process->setText(tr("P %1").arg(ready_processes[j]->ID));
                                draw_process->setStyleSheet("background-color:black;color:white; border-width: 2px; border-style: solid; border-color: gray;");
                                draw_process->setGeometry(width_Prev,700,(time-start_index)*60,70);
                                this->Scene->addWidget(draw_process);
                                draw_time = new QLabel();
                                draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                                draw_time->setText(tr(" %1").arg(start_index));
                                draw_time->setGeometry(width_Prev,780,60,30);
                                this->Scene->addWidget(draw_time);
                                width_Prev=width_Prev + (time-start_index)*60;

                                qDebug()<<time;
                                //DrawingSJFP.push_back(ready_processes[j]);
                                for(int x=0;x<Processes_Queue.size();x++){
                                    if(Processes_Queue[x]->ID == ready_processes[j]->ID){
                                        Processes_Queue[x]->Remaining_Time =Processes_Queue[x]->Remaining_Time- (time-start_index);
                                        if(Processes_Queue[x]->Remaining_Time == 0) Processes_Queue.erase(Processes_Queue.begin()+x);
                                       break;
                                    }
                                 }
                                break;
                            }

                        }
                    }
            }

            draw_time = new QLabel();
            draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
            draw_time->setText(tr(" %1").arg(time));
            draw_time->setGeometry(width_Prev,780,60,30);
            this->Scene->addWidget(draw_time);

            float sum=0;
            for(int q=0;q<arrive.size();q++){
                sum=sum+(arrive[q]->Termination_Time-arrive[q]->Burst_Time-arrive[q]->Arrival_Time);
            }
            Avg_label= new QLabel();
            float avg=sum/arrive.size();
            qDebug()<<avg;
            Avg_label->setStyleSheet("color:rgb(78,204,163); background-color:rgb(128,128,128);font-size: 40px;");
            Avg_label->setText(tr("Average Waiting Time= %1").arg(avg));
            Avg_label->setGeometry(500,50,700,70);
            this->Scene->addWidget(Avg_label);
}

//void MainWindow::Priority_AlgP()
//{

   // {
        /*Sorting according to arrival time*/
        /*Then Sorting According to priority*/

      //  for(int i=0; i<Processes_Queue.size(); i++){


         //   for(int j=i+1; j<Processes_Queue.size(); j++){

            //    if(Processes_Queue[j]->Arrival_Time >  Processes_Queue[i]->Arrival_Time){
               //     Process *temp=Processes_Queue[j];
                  //  Processes_Queue[j]=Processes_Queue[i];
                    //Processes_Queue[i]= temp;

                //}else if((Processes_Queue[j]->Arrival_Time == Processes_Queue[i]->Arrival_Time))
                //{
                   // if((Processes_Queue[i]->Priority < Processes_Queue[j]->Priority))
                    //{
                       // Processes_Queue[j]->Arrival_Time = Processes_Queue[i]->Arrival_Time + //Processes_Queue[i]->Burst_Time;
   //                     Process *temp=Processes_Queue[j];
      //                  Processes_Queue[j]=Processes_Queue[i];
         //               Processes_Queue[i]= temp;
            //        }

               // }

            //}
        //}

        /*Arranging the drawing queue , which simulates the heap*/
        //Temp = new DrawingQueue;
        //            for(int i = 0; i<Processes_Queue.size(); i++) qDebug()<< Processes_Queue[i]->Process_name << " Arrived :" <<Processes_Queue[i]->Arrival_Time << Processes_Queue[i]->Priority;
//        int time = 0;
   //     int SizeOfDrawingQueue=0;
      //  for(int i = 0; i < Processes_Queue.size();i++)
        //{

           // if( (Processes_Queue[i]->Arrival_Time) > time)
            //{


               // Temp->p_next = "Gap";Temp->p_width = ((Processes_Queue[i]->Arrival_Time) - //time );Temp->time_start = time;
   //             DrawingQueueFCFS.append(*Temp);
      //          SizeOfDrawingQueue++;
         //       time = time + Temp->p_width;

            //    Temp->p_next = Processes_Queue[i]->Process_name;
               // Temp->p_width = Processes_Queue[i]->Burst_Time;
                //Temp->time_start = time;
                //DrawingQueueFCFS.append(*Temp);
                //SizeOfDrawingQueue++;
                //time = time + Temp->p_width;


            //}else if((Processes_Queue[i]->Arrival_Time) == time)
            //{
               // qDebug()<<Processes_Queue[i]->Process_name<<" Arrived on time";
                //Temp->p_next = Processes_Queue[i]->Process_name;
                //Temp->p_width = Processes_Queue[i]->Burst_Time;
                //Temp->time_start = time;
                //time = time + Temp->p_width;
                //DrawingQueueFCFS.append(*Temp);
                //SizeOfDrawingQueue++;

            //}
            //else if( (Processes_Queue[i]->Arrival_Time) < time)
            //{
                //Process Arrived While another one was executing
                //Processes after this one in queue must either have
                //Lower priority of arrival time
               // if(Processes_Queue[i]->Priority < Processes_Queue[i-1]->Priority)
                //{
                   // qDebug()<<"Higher Priority Detected";
                    //Decrease width of the process before this one and keep the remaining time
                   // DrawingQueueFCFS[SizeOfDrawingQueue - 1].p_width = (Processes_Queue[i]->Arrival_Time - Processes_Queue[i-1]->Arrival_Time);
                    //Temp->p_next = Processes_Queue[i]->Process_name;
                    //Temp->p_width = Processes_Queue[i]->Burst_Time;
                    //Temp->time_start = Processes_Queue[i]->Arrival_Time;

                    //Add this process with its full width to the  queue
                    //DrawingQueueFCFS.append(*Temp);
                    //re add the part that you removed
                    //Temp->p_next = Processes_Queue[i-1]->Process_name;
                    //Temp->p_width = (Processes_Queue[i-1]->Burst_Time - (Processes_Queue[i]->Arrival_Time - Processes_Queue[i-1]->Arrival_Time));
                    //time += Processes_Queue[i]->Arrival_Time + Processes_Queue[i]->Burst_Time;
                    //Temp->time_start = Processes_Queue[i]->Arrival_Time + Processes_Queue[i]->Burst_Time;
                    //DrawingQueueFCFS.append(*Temp);



                //}else
                //{
                   // qDebug()<<Processes_Queue[i]->Process_name <<" ** "<<Processes_Queue[i-1]->Process_name;
                    //qDebug()<<"Arrived at same time";

                    //Temp->p_next = Processes_Queue[i]->Process_name;
                    //Temp->p_width = Processes_Queue[i]->Burst_Time;
                    //Temp->time_start = time;
                    //time = time + Temp->p_width;
                    //DrawingQueueFCFS.append(*Temp);
                    //SizeOfDrawingQueue++;}

            //}
        //}
        //        for(int i = 0; i<DrawingQueueFCFS.size(); i++) qDebug()<< DrawingQueueFCFS[i].p_next << " Arrived :" <<DrawingQueueFCFS[i].p_width;
        //time = 0;

        //for(int i = 0 ; i < DrawingQueueFCFS.size();i++)
        //{
           // Process_drawn = new QPushButton();
            //Process_drawn->setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius: 10%;} "
                                       //  "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");



            //if(DrawingQueueFCFS[i].p_next == "Gap")
            //{

               // Process_drawn->setGeometry( time,0,(DrawingQueueFCFS[i].p_width*25),100);
                //Process_drawn->setText(DrawingQueueFCFS[i].p_next);
                //Process_drawn->setStyleSheet(" QPushButton{ background-color:white; color:black; font-size: 17px; font-family: Arial;border-radius: 10%;} "
                                             //"QPushButton:hover { background-color: black; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:white;} ");
                //Scene->addWidget(Process_drawn);
                //time += ((DrawingQueueFCFS[i].p_width*25));
            //}else
            //{
               // Process_drawn->setGeometry(time,0,(DrawingQueueFCFS[i].p_width*25),100);
                //Process_drawn->setText(DrawingQueueFCFS[i].p_next);
                //Process_drawn->setStyleSheet(" QPushButton{ background-color:rgb(35,41,49); color:white; font-size: 17px; font-family: Arial;border-radius: 10%;} "
                                    //         "QPushButton:hover { background-color: white; border-radius:10%;border-width: 0.5px; border-style: solid; border-color: gray ;color:black;} ");
                //Scene->addWidget(Process_drawn);
                //time += ((DrawingQueueFCFS[i].p_width*25));
            //}
        //}
    //}
//}


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
    //and waiting time to be zero
    for(int s=0; s<Processes_Queue.size(); s++)
    {
        Processes_Queue[s]->Remaining_Time=Processes_Queue[s]->Burst_Time;
        Processes_Queue[s]->Waiting_Time=0;
    }
    qDebug()<< "HI " ;

    float time = 0;
    float gap=0;
    float start_index=0;
    float width_Prev=0;
    int first_time=1;
    float avg_waiting_time=0;
    float tot_avg_waiting_time=0;
    while(1)
    {
        bool vector_done = true;
        qDebug()<< "HI 2";
        //looping through all processes
        for(int k = 0; k < Processes_Queue.size(); k++)
        {
            qDebug()<< "process no:" << k;

            //here I'm calculating the total remaining time for the process where in
            float tot_rem_time=0.0;
            for(int i=0;i<k;i++)
            {
                tot_rem_time+=Processes_Queue[i]->Remaining_Time;
            }

            //checking for gap in the beginning
            if(Processes_Queue[0]->Arrival_Time>0 && k==0 && first_time)
            {
                 start_index=time;
                 gap=Processes_Queue[0]->Arrival_Time;
                 time+=gap;

                 draw_process = new QLabel();
                 draw_process->setText("GAP");
                 draw_process->setStyleSheet("background-color:white;color:black; border-width: 1px; border-style:solid; border-color: black;");
                 draw_process->setGeometry(width_Prev,700,gap*25,50) ;

                 //this->layout()->addWidget(draw_process);
                 qDebug()<<"GAP: "<<gap*25;
                 this->Scene->addWidget(draw_process);

                 draw_time = new QLabel();
                 draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                 draw_time->setText(tr(" %1").arg(start_index));
                 draw_time->setGeometry(width_Prev,780,60,30);
                 this->Scene->addWidget(draw_time);


                 width_Prev+=gap*25;

            }

            int temp=k;
            if( Processes_Queue[k]->Arrival_Time > time && k>0)
            {

                //vector_done = false;

                while(tot_rem_time!=0)
                {
                    for(int i=0;i<temp;i++)
                    {
                        if(Processes_Queue[i]->Remaining_Time>0)
                        {
                            if(Processes_Queue[i]->Remaining_Time>Processes_Queue[i]->quantum_time)
                            {
                                if(first_time)
                                {
                                    Processes_Queue[i]->Waiting_Time=time-Processes_Queue[i]->Arrival_Time;
                                }
                                else
                                {
                                    Processes_Queue[i]->Waiting_Time+=Processes_Queue[i]->Arrival_Time-Processes_Queue[i]->old_finish_time;
                                }
                                start_index=time;
                                time += Processes_Queue[i]->quantum_time;
                                Processes_Queue[i]->old_finish_time=time;

                                //drawing the quantum time
                                draw_process = new QLabel();
                                draw_process->setText(tr("P %1").arg(Processes_Queue[i]->ID));
                                draw_process->setStyleSheet("background-color:black;color:white; border-width: 1px; border-style:solid; border-color: white;");
                                draw_process->setGeometry(width_Prev,700,Processes_Queue[i]->quantum_time*25,50);
                                //this->layout()->addWidget(draw_process);
                                qDebug()<<Processes_Queue[i]->quantum_time*25;

                                this->Scene->addWidget(draw_process);

                                draw_time = new QLabel();
                                draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                                draw_time->setText(tr(" %1").arg(start_index));
                                draw_time->setGeometry(width_Prev-5,780,60,30);
                                this->Scene->addWidget(draw_time);

                                width_Prev+=Processes_Queue[i]->quantum_time*25;


                                //making remaining time = 0 to indicate process is done
                                Processes_Queue[i]->Remaining_Time-=Processes_Queue[i]->quantum_time;
                                tot_rem_time-=Processes_Queue[i]->quantum_time;
                            }

                            else
                            {
                                if(first_time)
                                {
                                    Processes_Queue[i]->Waiting_Time=time-Processes_Queue[i]->Arrival_Time;
                                }
                                else
                                {
                                    Processes_Queue[i]->Waiting_Time+=Processes_Queue[i]->Arrival_Time-Processes_Queue[i]->old_finish_time;
                                }

                                start_index=time;
                                time += Processes_Queue[i]->Remaining_Time;
                                Processes_Queue[i]->old_finish_time=time;

                                //drawing the quantum time
                                draw_process = new QLabel();
                                draw_process->setText(tr("P %1").arg(Processes_Queue[i]->ID));
                                draw_process->setStyleSheet("background-color:black;color:white; border-width: 1px; border-style:solid; border-color: white;");
                                draw_process->setGeometry(width_Prev,700,Processes_Queue[k]->Remaining_Time*25,50);
                                //this->layout()->addWidget(draw_process);
                                qDebug()<<Processes_Queue[i]->Remaining_Time*25;
                                this->Scene->addWidget(draw_process);

                                draw_time = new QLabel();
                                draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                                draw_time->setText(tr(" %1").arg(start_index));
                                draw_time->setGeometry(width_Prev-5,780,60,30);
                                this->Scene->addWidget(draw_time);

                                width_Prev+=Processes_Queue[i]->Remaining_Time*25;

                                //Processes_Queue[i]->Waiting_Time=time -Processes_Queue[i]->Burst_Time;

                                //making remaining time = 0 to indicate process is done
                                Processes_Queue[i]->Remaining_Time=0;
                                tot_rem_time=0;
                            }
                        }

                    }
                }

                if(tot_rem_time==0 && Processes_Queue[k]->Arrival_Time > time)
                {
                    float gap=Processes_Queue[k]->Arrival_Time-time;
                    start_index=time;
                    time =Processes_Queue[k]->Arrival_Time;

                    //drawing the quantum time
                    draw_process = new QLabel();
                    draw_process->setText("GAP");
                    draw_process->setStyleSheet("background-color:white;color:black; border-width: 1px; border-style:solid; border-color: black;");
                    draw_process->setGeometry(width_Prev,700,gap*25,50);
                    //this->layout()->addWidget(draw_process);
                    qDebug()<<gap*25;
                    this->Scene->addWidget(draw_process);


                    draw_time = new QLabel();
                    draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                    draw_time->setText(tr(" %1").arg(start_index));
                    draw_time->setGeometry(width_Prev-5,780,60,30);
                    this->Scene->addWidget(draw_time);


                    width_Prev+=gap*25;

                }
                k--;
             }

            //here we handle the case of remaining time =quantum
            //or remaining time = 0 and there is still time to the next process and gap is drawn
             else
                {

                    if(Processes_Queue[k]->Remaining_Time>0)
                    {
                        vector_done = false;

                        if(Processes_Queue[k]->Remaining_Time>Processes_Queue[k]->quantum_time)
                        {
                            if(first_time)
                            {
                                Processes_Queue[k]->Waiting_Time=time-Processes_Queue[k]->Arrival_Time;
                            }
                            else
                            {
                                Processes_Queue[k]->Waiting_Time+=Processes_Queue[k]->Arrival_Time-Processes_Queue[k]->old_finish_time;
                            }

                            start_index=time;

                            time += Processes_Queue[k]->quantum_time;
                            Processes_Queue[k]->old_finish_time=time;
                            //drawing the quantum time
                            draw_process = new QLabel();
                            draw_process->setText(tr("P %1").arg(Processes_Queue[k]->ID));
                            draw_process->setStyleSheet("background-color:black;color:white; border-width: 1px; border-style:solid; border-color: white;");
                            draw_process->setGeometry(width_Prev,700,Processes_Queue[k]->quantum_time*25,50);
                            //this->layout()->addWidget(draw_process);
                            qDebug()<<Processes_Queue[k]->quantum_time*25;
                            this->Scene->addWidget(draw_process);


                            draw_time = new QLabel();
                            draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                            draw_time->setText(tr(" %1").arg(start_index));
                            draw_time->setGeometry(width_Prev-5,780,60,30);
                            this->Scene->addWidget(draw_time);

                            width_Prev+=Processes_Queue[k]->quantum_time*25;


                            //making remaining time = 0 to indicate process is done
                            Processes_Queue[k]->Remaining_Time-=Processes_Queue[k]->quantum_time;
                        }


                        else
                        {
                            if(first_time)
                            {
                                Processes_Queue[k]->Waiting_Time=time-Processes_Queue[k]->Arrival_Time;
                            }
                            else
                            {
                                Processes_Queue[k]->Waiting_Time+=Processes_Queue[k]->Arrival_Time-Processes_Queue[k]->old_finish_time;
                            }

                            start_index=time;

                            time += Processes_Queue[k]->Remaining_Time;
                            Processes_Queue[k]->old_finish_time=time;

                            //drawing the quantum time
                            draw_process = new QLabel();
                            draw_process->setText(tr("P %1").arg(Processes_Queue[k]->ID));
                            draw_process->setStyleSheet("background-color:black;color:white; border-width: 1px; border-style:solid; border-color: white;");
                            draw_process->setGeometry(width_Prev,700,Processes_Queue[k]->Remaining_Time*25,50);
                            //this->layout()->addWidget(draw_process);
                            qDebug()<<Processes_Queue[k]->Remaining_Time*25;
                            this->Scene->addWidget(draw_process);

                            draw_time = new QLabel();
                            draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
                            draw_time->setText(tr(" %1").arg(start_index));
                            draw_time->setGeometry(width_Prev-5,780,60,30);
                            this->Scene->addWidget(draw_time);

                            width_Prev+=Processes_Queue[k]->Remaining_Time*25;

                            //Processes_Queue[k]->Waiting_Time=time -Processes_Queue[k]->Burst_Time;

                            //making remaining time = 0 to indicate process is done
                            Processes_Queue[k]->Remaining_Time=0;

                        }

                    }



                }
            }


        first_time=0;
        if(vector_done == true)
        {
            start_index=time;
            draw_time = new QLabel();
            draw_time->setStyleSheet("color:black; background-color:rgb(128,128,128);");
            draw_time->setText(tr(" %1").arg(start_index));
            draw_time->setGeometry(width_Prev-15,780,60,30);
            this->Scene->addWidget(draw_time);
            break;
        }
    }
    for(int l=0;l<Processes_Queue.size();l++)
    {
        avg_waiting_time+=Processes_Queue[l]->Waiting_Time;
    }

    Avg_label= new QLabel();
    int n=Processes_Queue.size();
    tot_avg_waiting_time=avg_waiting_time/(float)n;
    qDebug()<<tot_avg_waiting_time;
    Avg_label->setStyleSheet("color:rgb(78,204,163); background-color:rgb(128,128,128);font-size: 40px;");
    Avg_label->setText(tr("Average Waiting Time= %1").arg(abs(tot_avg_waiting_time)));
    Avg_label->setGeometry(500,50,700,70);
    this->Scene->addWidget(Avg_label);

}
//this function is supposed to delete and start a new algorithm
void MainWindow::again(){
   /* Simulate->deleteLater();
    burst_time.clear();
    arrival_time.clear();
    if(Alg_chosen=="SJF") {
        Preemptive_label->deleteLater();
        Preemptive_Checkbox->deleteLater();
        arrival_label->deleteLater();
        burst_label->deleteLater();
        int height=0;
        for (int i = 0; i<this->num_process_chosen.split(" ")[0].toInt(); i++)//convert from QString to int
        {
            deletelabel = new QLabel();
            deletelabel2 = new QLabel();
            deletelabel3 = new QLabel();
            deletelabel->setGeometry(20,300+height,30,30);
            deletelabel->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
            deletelabel2->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
            deletelabel3->setStyleSheet("background-color:rgb(78,204,163); color:rgb(35,41,49); font-size: 15px; font-family: Arial;");
            this->scene_toolbar->addWidget(deletelabel2);
            deletelabel2->setGeometry(60,300+height,80,30);
            this->scene_toolbar->addWidget(deletelabel3);
            deletelabel3->setGeometry(170,300+height,80,30);
            this->scene_toolbar->addWidget(deletelabel);
            height+=50;
        }
        deletechart = new QLabel();
        deletechart->setGeometry(0,700,1000,50);
        deletechart->setStyleSheet("background-color:rgb(128,128,128);");
        Scene->addWidget(deletechart);
       // draw_process->deleteLater();
    }*/


        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0],qApp->arguments());


}
