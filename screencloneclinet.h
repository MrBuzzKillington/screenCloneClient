#ifndef SCREENCLONECLINET_H
#define SCREENCLONECLINET_H

#include <QMainWindow>
#include "serverclientmodule.h"
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui { class ScreenCloneClinet; }
QT_END_NAMESPACE

class ScreenCloneClinet : public QMainWindow
{
    Q_OBJECT

public:
    ScreenCloneClinet(QWidget *parent = nullptr, QHostAddress addr=QHostAddress::LocalHost);
    ~ScreenCloneClinet();

private:
    Ui::ScreenCloneClinet *ui;
    std::unique_ptr<serverClientModule> clientPtr_;
    QImage rcvdImage_;

private slots:
  void processNewImage();
};
#endif // SCREENCLONECLINET_H
