#ifndef WIDGET_H
#define WIDGET_H

#include <algorithm>
#include "chat.h"

#define SPORT 36000
#define LPORT 35000
#define AQVERSION 0.1

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

QVector<QString>UIP;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void InitForm();

private:
    Ui::Widget *ui;
    int iFlag=0;//程序数量
    QTimer *MainTimer;//主计时器
    QTimer *AcTimer;//心跳包
    QUdpSocket *UdpSocket;
    QTcpServer *TcpServer;
    QTcpSocket *TcpSocket;
    QString USER;
};
#endif // WIDGET_H
