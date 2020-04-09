#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

namespace Ui {
class MainWindow;
}
class Process
{
public:
    int Arrival_Time, Burst_Time, Remaining_Time, Priority, Termination_Time;
    int TurnAround_Time = Termination_Time - Arrival_Time;
    int Waiting_Time = TurnAround_Time - Burst_Time;
    QString Process_name;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString num_process_chosen,Alg_chosen;
    QLineEdit *num_process_line_edit;
    QLineEdit *arrival_input;
    QLineEdit *burst_input;
    QComboBox *priority_input;
    QComboBox *Algorithm_dropdown;
    QPushButton *m_button;
    QToolBar *toolbar;
    QLabel *type;
    QLabel *N_process;
    QLabel *burst_label;
    QLabel *priortiy_label;
    QLabel *arrival_label;
    QLabel *ID_Process;
    QLabel *Preemptive_label;
    QPushButton *Ok;
    QPushButton *Simulate;
    QCheckBox *Preemptive_Checkbox;
    QVector <Process *> Processes_Queue;
    //  -)The vector that contains pointers to processes can be treated  as queue
    //  -)Each element represents a process carrying its information
    QVector <QLineEdit *> burst_time;
    QVector <QLineEdit *> arrival_time;
    QVector <QComboBox *> priority_vect;


    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int General_layout();
    int SJF_layout();
    int FCFS_layout();
    int RR_layout();
    int PRIORITY_layout();

private:

   // Ui::MainWindow *ui;

public slots:
    void get_param();
    void Get_Text();

};




#endif // MAINWINDOW_H
