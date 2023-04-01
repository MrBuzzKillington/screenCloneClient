#ifndef SERVERCLIENTMODULE_H
#define SERVERCLIENTMODULE_H

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

private:
    std::unique_ptr<QUdpSocket> socketPtr_;
    int imageSeq_;
    QHostAddress clientAddr_;
    int clientPort_;



private slots:
  void processEnetMessage();

};

#endif // SERVERCLIENTMODULE_H
