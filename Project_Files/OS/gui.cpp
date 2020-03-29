#include "gui.h"

gui::gui (QWidget *parent) : QWidget(parent)
{
  Ok = new QPushButton("OK");
}

void gui::connection(){
    QObject :: connect(Ok,SIGNAL(clicked()),this,SLOT(get_data())) ;

}

void gui::get_data(){
    qDebug("hi");
}
