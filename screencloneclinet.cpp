#include "screencloneclinet.h"
#include "./ui_screencloneclinet.h"


ScreenCloneClinet::ScreenCloneClinet(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::ScreenCloneClinet),
    netModPtr_(),
    rcvdImage_()
{
    ui->setupUi(this);

    //Create the network connection class
    netModPtr_.reset(new serverClientModule(QHostAddress::LocalHost, 1234));

    connect( netModPtr_.get(), &serverClientModule::imageAvalable, this, &ScreenCloneClinet::processNewImage);

    //imageViewObj_
}

ScreenCloneClinet::~ScreenCloneClinet()
{
    delete ui;
}

void  ScreenCloneClinet::processNewImage()
{
    //qDebug() << "process new image";
    QImage lastImage = netModPtr_->getLastImage();


    if (lastImage.width() < 20)
    {
        qDebug() << "failed to get valid image";
        return;
    }
    //Resize the window
    this->resize( lastImage.width()+20, lastImage.height()+20 );



    QGraphicsScene *scene;
    scene = new QGraphicsScene(this);
    scene->addPixmap( QPixmap::fromImage( lastImage ) );
    scene->setSceneRect(lastImage.rect());

    //Scal the size of the imageview widget
    QRect viewSize = ui->imageViewObj_->geometry();
    viewSize.setWidth(lastImage.width());
    viewSize.setHeight(lastImage.height());
    ui->imageViewObj_->setGeometry(viewSize);
    ui->imageViewObj_->setScene(scene);



}
