#include "serverclientmodule.h"

#include<QNetworkDatagram>
#include<QImage>
#include <QPixmap>


serverClientModule::serverClientModule(QObject *parent,QHostAddress addr, int port):
    QObject(parent),
    imageSeq_(0),
    dataPackets_(),
    images_(),
    socket_(),
    addr_(),
    port_(),
    connected_(false)
{
    addr_ = addr;
    port_ = port;

    dataPackets_.clear();

    socket_.reset(new QTcpSocket(this));
    QObject::connect(socket_.get(), &QTcpSocket::readyRead, this, &serverClientModule::readyRead);

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
        }
        else
        {
            qDebug() << "Not connected!";
            connected_ = false;
        }
    }

}

//void serverClientModule::processEnetMessage()
//{
//    QByteArray tempQBA;
//https://www.youtube.com/watch?v=Si6Gz8jQtGQ
//    while (socketPtr_->hasPendingDatagrams()) {
//           QNetworkDatagram datagram = socketPtr_->receiveDatagram();
//           tempQBA.clear();
//           tempQBA.append(datagram.data());
//           qDebug() << "got message " << tempQBA.size();
//           if (tempQBA.at(0) == 0) //first
//           {
//               dataPackets_.clear(); //This should flush the data
//           }
//           //Add the data to the datapacket;
//           dataPackets_.append( tempQBA );
//           if (tempQBA.at(0)== 2) //End
//           {
//               processImageData();
//           }

//       }
//}


//q8(type),Q32(seq),q32(frag),q32(payloadsize),qba(image payload)
void serverClientModule::processImageData()
{

  //Generate the message
    QDataStream sBuff(&dataPackets_, QIODevice::ReadOnly);

    qDebug() << "DataPAcketSize:" << dataPackets_.size();

   // buf.clear();
  //  sBuff << (quint16) 0x5C5C;
   // sBuff << (qint32) imageSeq_;
   // sBuff << (qint32)payloadSize;
   // sBuff << imageQBA;//.first(payloadSize);

    quint16 key;
    quint32 imageSeq;
    quint32 payloadSize;
    QByteArray tempImageBuff;


    sBuff >> key;
    sBuff >> imageSeq;
    sBuff >> payloadSize;
    sBuff >> tempImageBuff;

    qDebug() << "key:" << key << " seq:" << imageSeq << " payload:" << payloadSize;


    bool flag;
    //QByteArray ba;
    QDataStream in(tempImageBuff);
    qDebug() << " full size:" << tempImageBuff.size();
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


//    sBuff >> newImage;

//    if (key == 0x5c5c)
//    {
//        images_.push_back(newImage);
//        emit imageAvalable();
//        qDebug() << "numImages: " << images_.size();
//    } else {
//        qDebug() << "Wrong data";
//    }
    qDebug() << "h:" << newImage.height() << " w:" << newImage.width();
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
     qDebug() << "got message" << socket_->size();
     dataPackets_.clear();
     dataPackets_.append(socket_->readAll());
     processImageData();
 }
