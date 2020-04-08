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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString num_process_chosen,Alg_chosen;
    QLineEdit *num_process_line_edit;
    QLineEdit *arrival_input;
    QLineEdit *burst_input;
    QComboBox *Algorithm_dropdown;
    QPushButton *m_button;
    QToolBar *toolbar;
    QLabel *type;
    QLabel *N_process;
    QLabel *burst_label;
    QLabel *arrival_label;
    QLabel *ID_Process;
    QPushButton *Ok;
    QPushButton *Simulate;
    QVector <QLineEdit *> burst_time;
    QVector <QLineEdit *> arrival_time;


    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
