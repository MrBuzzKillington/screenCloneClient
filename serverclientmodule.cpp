#include "serverclientmodule.h"

#include<QNetworkDatagram>
#include<QImage>
#include <QPixmap>


serverClientModule::serverClientModule(QHostAddress addr, int port):
    socketPtr_(),
    imageSeq_(0),
    dataPackets_(),
    images_()
{

    clientAddr_ = addr;
    clientPort_ = port;
    socketPtr_.reset( new QUdpSocket() );
    socketPtr_->bind(clientAddr_, clientPort_);

    dataPackets_.clear();
     //connect( ui->captureBtn_, &QPushButton::released, this, &ScreenCloneServerWindow::handleCaptureButton);
    QObject::connect(socketPtr_.get(), &QUdpSocket::readyRead, this, &serverClientModule::processEnetMessage);
   // connect(socketPtr_, &QUdpSocket::readyRead, this, &serverClientModule::processEnetMessage);

}

serverClientModule::~serverClientModule()
{

}



void serverClientModule::processEnetMessage()
{
    QByteArray tempQBA;
//https://www.youtube.com/watch?v=Si6Gz8jQtGQ
    while (socketPtr_->hasPendingDatagrams()) {
           QNetworkDatagram datagram = socketPtr_->receiveDatagram();
           tempQBA.clear();
           tempQBA.append(datagram.data());
           //qDebug() << "got message " << tempQBA.size();
           if (tempQBA.at(0) == 0) //first
           {
               dataPackets_.clear(); //This should flush the data
           }
           //Add the data to the datapacket;
           dataPackets_.append( tempQBA );
           if (tempQBA.at(0)== 2) //End
           {
               processImageData();
           }

       }
}


//q8(type),Q32(seq),q32(frag),q32(payloadsize),qba(image payload)
void serverClientModule::processImageData()
{
    quint8 packetType = 0;//0==first,1==mid,2==last
    quint32 seqNum = 0;
    quint32 fragment = 0;
    quint32 bytesSent = 0;
    quint32 payloadSize = 0;


  //Generate the message
    QByteArray tempImageBuff;
    QByteArray fullImageBuff;
    QDataStream sBuff(&dataPackets_, QIODevice::ReadOnly);

    while(sBuff.atEnd() == false)
    {
        sBuff >> packetType;
        sBuff >> seqNum;
        sBuff >> fragment;
        sBuff >> payloadSize;
        sBuff >> tempImageBuff;
        fullImageBuff.append(tempImageBuff);
        //qDebug() << "type:"<<packetType<< " seq:"<<seqNum << "frag:"<< fragment<<" payloadSize:" << payloadSize << " imageSize:" << fullImageBuff.size();
    }



    bool flag;
    //QByteArray ba;
    QDataStream in(fullImageBuff);
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
