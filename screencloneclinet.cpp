#include "screencloneclinet.h"
#include "./ui_screencloneclinet.h"
#include <QShortcut>


ScreenCloneClinet::ScreenCloneClinet(QWidget *parent, QHostAddress addr):
    QMainWindow(parent),
    ui(new Ui::ScreenCloneClinet),
    clientPtr_(),
    rcvdImage_(),
    scene_()
{
    ui->setupUi(this);

    //Create the network connection class
    qDebug() << "connecting to: " << addr.toString();
    clientPtr_.reset( new serverClientModule( parent, addr, 1234 ) );
    connect( clientPtr_.get(), &serverClientModule::imageAvalable, this, &ScreenCloneClinet::processNewImage);

    clientPtr_->connectToServer(); //data will start flowing
    this->move(0,0);


    QShortcut * shortcut = new QShortcut(QKeySequence(Qt::Key_Escape),this,SLOT(escapeEvent()));
    shortcut->setAutoRepeat(false);


}

ScreenCloneClinet::~ScreenCloneClinet()
{
    delete ui;
}

void  ScreenCloneClinet::processNewImage()
{

    //qDebug() << "process new image";
    QImage lastImage = clientPtr_->getLastImage();


    if (lastImage.width() < 20)
    {
        qDebug() << "failed to get valid image";
        return;
    }
    //Resize the window
    //this->resize( lastImage.width()+20, lastImage.height()+20 );



    //QGraphicsScene *scene;
    scene_.reset(new QGraphicsScene(this));
    scene_->addPixmap( QPixmap::fromImage( lastImage ) );
    scene_->setSceneRect(lastImage.rect());

    //Scal the size of the imageview widget

    QRect viewSize = ui->imageViewObj_->geometry();
    viewSize.setWidth(lastImage.width());
    viewSize.setHeight(lastImage.height());
    ui->imageViewObj_->setGeometry(0,0,viewSize.width(),viewSize.height());
    ui->imageViewObj_->setScene(scene_.get());

}


void  ScreenCloneClinet::escapeEvent()
{
    qDebug() << "Escape key";
    this->close();
}
