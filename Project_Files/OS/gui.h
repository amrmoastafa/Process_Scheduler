#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QCoreApplication>

class gui : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
private:
    QPushButton *Ok;
public:
    explicit gui(QWidget *parent = nullptr);
    void connection();
public slots:
    void get_data();

};

#endif // GUI_H
