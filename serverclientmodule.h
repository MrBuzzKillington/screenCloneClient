#ifndef SERVERCLIENTMODULE_H
#define SERVERCLIENTMODULE_H

#include "qimage.h"
#include<QtNetwork/QUdpSocket>
#include<QtNetwork/QHostAddress>
#include<QtNetwork/QLocalSocket>
#include <QObject>


class serverClientModule : public QObject
{
    Q_OBJECT
public:
    serverClientModule(QHostAddress addr, int port);
    ~serverClientModule();
    QImage getLastImage();

private:
    std::unique_ptr<QUdpSocket> socketPtr_;
    int imageSeq_;
    QHostAddress clientAddr_;
    int clientPort_;
    QByteArray dataPackets_;

    std::vector<QImage>images_;

    void processImageData();



private slots:
  void processEnetMessage();

signals:
  void imageAvalable();
};

#endif // SERVERCLIENTMODULE_H
