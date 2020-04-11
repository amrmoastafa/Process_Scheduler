#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>
#include <QVector>
#include <QList>
#include <QDebug>
#include <QCheckBox>
#include <QProcess>
#include <QMessageBox>



namespace Ui {
class MainWindow;
}
class Process
{
public:
    int Arrival_Time, Burst_Time, Remaining_Time, Priority, Termination_Time,ID,quantum_time;
    int TurnAround_Time = Termination_Time - Arrival_Time;
    int Waiting_Time = TurnAround_Time - Burst_Time;
    QString Process_name;
};
class DrawingQueue
{
public:
    QString p_next;
    int p_width;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    DrawingQueue *Temp;
    QGraphicsScene *Scene;
    QGraphicsView *view;
    QString num_process_chosen,Alg_chosen;
    QLineEdit *num_process_line_edit;
    QLineEdit *arrival_input;
    QLineEdit *burst_input;
    QLineEdit *time_quantum_input;
    QComboBox *priority_input;
    QComboBox *Algorithm_dropdown;
    QPushButton *m_button;
    QToolBar *toolbar;
    QLabel *type;
    QLabel *N_process;
    QLabel *burst_label;
    QLabel *priortiy_label;
    QLabel *time_quantum_label;
    QLabel *arrival_label;
    QLabel *ID_Process;
    QLabel *Preemptive_label;
    QPushButton *Ok;
    QPushButton *restart;
    QPushButton *Simulate;
    QCheckBox *Preemptive_Checkbox;
    QMessageBox *msg_box;
    QVector <Process *> Processes_Queue;
    //  -)The vector that contains pointers to processes can be treated  as queue
    //  -)Each element represents a process carrying its information
    QVector <QLineEdit *> burst_time;
    QVector <QLineEdit *> arrival_time;
    QVector <QComboBox *> priority_vect;
    QVector <DrawingQueue *>  DrawingQueueFCFS;

    /**for drawing**/
    QLabel *draw_process;
    QLabel *draw_time;
    QGraphicsScene *scene_toolbar;
    QScrollArea * scrollArea;
    QGraphicsView *view_toolbar;
    QLabel *deletelabel;
    QLabel *deletelabel2;
    QLabel *deletelabel3;
    QLabel *deletechart;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//    int General_layout();
    int SJF_layout();
    int FCFS_layout();
    int RR_layout();
    int PRIORITY_layout();
    void SJF_NONP_Alg();
    void SJF_P_Alg();
    void FCFS_Alg();
    void RR_Alg();


private:

   // Ui::MainWindow *ui;

public slots:
    void get_param();
    void Get_Text();
    void again();

};




#endif // MAINWINDOW_H
