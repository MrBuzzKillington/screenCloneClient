#include "serverclientmodule.h"

#include<QNetworkDatagram>


serverClientModule::serverClientModule(QHostAddress addr, int port):
    socketPtr_(),
    imageSeq_(0)
{

    clientAddr_ = addr;
    clientPort_ = port;
    socketPtr_.reset( new QUdpSocket() );
    socketPtr_->bind(clientAddr_, clientPort_);


     //connect( ui->captureBtn_, &QPushButton::released, this, &ScreenCloneServerWindow::handleCaptureButton);
    QObject::connect(socketPtr_.get(), &QUdpSocket::readyRead, this, &serverClientModule::processEnetMessage);
   // connect(socketPtr_, &QUdpSocket::readyRead, this, &serverClientModule::processEnetMessage);

}

serverClientModule::~serverClientModule()
{

}



void serverClientModule::processEnetMessage()
{
//https://www.youtube.com/watch?v=Si6Gz8jQtGQ
    while (socketPtr_->hasPendingDatagrams()) {
           QNetworkDatagram datagram = socketPtr_->receiveDatagram();
           //processTheDatagram(datagram);
           qDebug() << "got message " << datagram.data().size();
       }
}
