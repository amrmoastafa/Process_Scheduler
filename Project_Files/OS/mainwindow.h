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
#include <vector>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString num_process,alg;
    QLineEdit *number;
    QLineEdit *arrival_time;
    QLineEdit *burst_time;
    QComboBox *Algorithm;
    QPushButton *m_button;
    QToolBar *toolbar;
    QLabel *type;
    QLabel *N_process;
    QLabel *burst_label;
    QLabel *arrival_label;
    QLabel *ID;
    QPushButton *Ok;
    QPushButton *Simulate;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

   // Ui::MainWindow *ui;

public slots:
    void get_param();

};




#endif // MAINWINDOW_H
