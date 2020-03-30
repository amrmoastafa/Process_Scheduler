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

public slots:


};

#endif // GUI_H
