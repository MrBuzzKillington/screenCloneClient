#ifndef SERVERCLIENTMODULE_H
#define SERVERCLIENTMODULE_H

#include "qimage.h"
#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QAbstractSocket>


class serverClientModule : public QObject
{
    Q_OBJECT
public:
    explicit serverClientModule(QObject *parent = 0,QHostAddress addr=QHostAddress::LocalHost, int port=1234);
    ~serverClientModule();
    void connectToServer();
    QImage getLastImage();

protected:

private:
    void processImageData();

    int imageSeq_;   
    std::unique_ptr<QTcpSocket> socket_;

    QByteArray dataPackets_;
    std::vector<QImage>images_;
    QHostAddress addr_;
    int port_;
    bool connected_;


private slots:
    void readyRead();

signals:
    void imageAvalable();
};

#endif // SERVERCLIENTMODULE_H
