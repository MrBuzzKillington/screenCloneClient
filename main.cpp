#include "screencloneclinet.h"

#include <QApplication>

#include <string>
#include <iostream>
#include<QMessageBox>
#include<QScreen>

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


    QList <QScreen*> screens = QGuiApplication::screens();
    int totalWidth = 0;
    int totalHeight = 0;

    int screenNum = 0;
    QRect  screenGeometry;
    //QScreen *screen = screens.front();//QGuiApplication::primaryScreen();
    for(auto itr : screens)
    {
       screenGeometry = itr->geometry();
       //qDebug() << "index" << screenNum << "X" << screenGeometry.x() << " y " << screenGeometry.y() << " W " << screenGeometry.width() << " H " << screenGeometry.height();

       if (screenGeometry.x() + screenGeometry.width() > totalWidth)
           totalWidth = screenGeometry.x() + screenGeometry.width();

       if (screenGeometry.y() + screenGeometry.height() > totalHeight)
           totalHeight = screenGeometry.y() + screenGeometry.height();
       screenNum++;
    }




    ScreenCloneClinet w(nullptr , destIP);
    w.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    w.setGeometry(2560,0,totalWidth,totalHeight);
    w.show();

    //w.setWindowState(Qt::WindowFullScreen );
   // w.show();
    return a.exec();
}
