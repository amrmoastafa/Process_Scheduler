#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QToolBar>
#include "gui.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    gui *gui;
    QString t1,t2;
    QString *alg;
    QString *n;
    QLineEdit *number;
    QLineEdit *num2;
    QComboBox *Algorithm;
    QPushButton *m_button;
    QToolBar *toolbar;
    QLabel *type;
    QLabel *N_process;
    QPushButton *Ok;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindow *ui;

   // QPushButton *Ok ;
public slots:
    void get_param();

};




#endif // MAINWINDOW_H
