#ifndef SCREENCLONECLINET_H
#define SCREENCLONECLINET_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ScreenCloneClinet; }
QT_END_NAMESPACE

class ScreenCloneClinet : public QMainWindow
{
    Q_OBJECT

public:
    ScreenCloneClinet(QWidget *parent = nullptr);
    ~ScreenCloneClinet();

private:
    Ui::ScreenCloneClinet *ui;
};
#endif // SCREENCLONECLINET_H
