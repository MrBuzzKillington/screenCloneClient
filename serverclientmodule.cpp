#include "serverclientmodule.h"

#include<QNetworkDatagram>
#include<QImage>
#include <QPixmap>
#include <QTimer>


serverClientModule::serverClientModule(QObject *parent,QHostAddress addr, int port):
    QObject(parent),
    imageSeq_(0),
    dataPackets_(),
    images_(),
    socket_(),
    addr_(),
    port_(),
    connected_(false),
    sendTimer_()
{
    addr_ = addr;
    port_ = port;

    dataPackets_.clear();

    socket_.reset(new QTcpSocket(this));
    QObject::connect(socket_.get(), &QTcpSocket::readyRead, this, &serverClientModule::readyRead);
    QObject::connect(socket_.get(), &QTcpSocket::disconnected, this, &serverClientModule::remoteDisconnected);

    sendTimer_.reset(new QTimer(this));
    connect(sendTimer_.get(), &QTimer::timeout, this, &serverClientModule::tryReconnect);



//    QObject::connect(socketPtr_.get(), &QUdpSocket::readyRead, this, &serverClientModule::processEnetMessage);

}

serverClientModule::~serverClientModule()
{
       socket_->disconnectFromHost();
       socket_->close();
       connected_ = false;
       qDebug() << "closed socket";
}

void serverClientModule::connectToServer()
{
    if (connected_ == false)
    {
        socket_->connectToHost(addr_,port_);

        if(socket_->waitForConnected(3000))
        {
            qDebug() << "Connected!";
            connected_ = true;
            sendTimer_->stop();
        }
        else
        {
            qDebug() << "Not connected!";
            connected_ = false;
            sendTimer_->start( 500 );
        }
    }

}

//q8(type),Q32(seq),q32(frag),q32(payloadsize),qba(image payload)
void serverClientModule::processImageData()
{

  //Generate the message
    QDataStream sBuff(&dataPackets_, QIODevice::ReadOnly);


    quint16 key;
    quint32 imageSeq;
    quint32 payloadSize;
    QByteArray tempImageBuff;

    sBuff >> key;
    sBuff >> imageSeq;
    sBuff >> payloadSize;
    //need to check if the full message was received or not
    // qDebug() << "byteArrayLEn:" << dataPackets_.size() << "key:" << key << " seq:" << imageSeq << " payload:" << payloadSize;
    if (dataPackets_.size() < (payloadSize + 10) ) //key+image_payload
    {
        //qDebug() << "not full fragment";
        return;
    }
    sBuff >> tempImageBuff;

    dataPackets_.clear();




    bool flag;
    //QByteArray ba;
    QDataStream in(tempImageBuff);
   // qDebug() << " full size:" << tempImageBuff.size();
    QImage newImage;
    in >> newImage >> flag;

    if(flag)
    {
        images_.push_back(newImage);
        emit imageAvalable();
       //qDebug() << "numImages: " << images_.size();
    } else {
        qDebug() << "Wrong data";
    }



//    }
   //qDebug() << "h:" << newImage.height() << " w:" << newImage.width();
}


 QImage serverClientModule::getLastImage()
 {

     QImage image = QPixmap(1, 1).toImage();

     if (images_.size() < 1)
     {
         qDebug() << "There was no image to fetch";
         return image;
     }

     QImage theImage = images_.back();
     images_.clear();
     return theImage;

 }


 void serverClientModule::readyRead()
 {
     //qDebug() << "got message" << socket_->size();
     //dataPackets_.clear();
     dataPackets_.append(socket_->readAll());
     processImageData();
 }


 void serverClientModule::remoteDisconnected()
 {
     qDebug() << "got remote dissconnect";
     connected_ = false;
     sendTimer_->start( 500 );
 }


 void serverClientModule::tryReconnect()
 {
     if (connected_ == false)
     {
         qDebug() << "Try connect again";
         connectToServer();
     }
 }



