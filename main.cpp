#include "screencloneclinet.h"

#include <QApplication>

#include <string>
#include <iostream>
#include<QMessageBox>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    QHostAddress destIP = QHostAddress::LocalHost;
    if (a.arguments().size() > 1)
    {
        qDebug() << "server IP is " << a.arguments().at(1);
        std::cout << "IP ISL:" << a.arguments().at(1).toStdString() <<std::endl;
        destIP.setAddress(a.arguments().at(1));

    } else {
        std::cout << "no argument" << std::endl;
    }
    if (a.arguments().size() > 2)
    {
        QMessageBox ipPopup(nullptr);
        ipPopup.setText(destIP.toString());
        ipPopup.exec();
    }


    ScreenCloneClinet w(nullptr , destIP);
    w.show();
    return a.exec();
}
