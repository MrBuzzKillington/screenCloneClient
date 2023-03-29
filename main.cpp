#include "screencloneclinet.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenCloneClinet w;
    w.show();
    return a.exec();
}
