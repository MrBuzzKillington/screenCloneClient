#include "screencloneclinet.h"
#include "./ui_screencloneclinet.h"

ScreenCloneClinet::ScreenCloneClinet(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::ScreenCloneClinet),
    netModPtr_()
{
    ui->setupUi(this);

    //Create the network connection class
    netModPtr_.reset(new serverClientModule(QHostAddress::LocalHost, 1234));
    //imageViewObj_
}

ScreenCloneClinet::~ScreenCloneClinet()
{
    delete ui;
}

