#include "screencloneclinet.h"
#include "./ui_screencloneclinet.h"

ScreenCloneClinet::ScreenCloneClinet(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScreenCloneClinet)
{
    ui->setupUi(this);


    //imageViewObj_
}

ScreenCloneClinet::~ScreenCloneClinet()
{
    delete ui;
}

