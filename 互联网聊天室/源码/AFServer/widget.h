#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTimer>
#define UDPLPORT 10000//默认接收端口 10000
#define UDPSPORT 20000//默认发送端口 20000
#define IP "182.92.66.233"
#define PORT 42000
#define VERSION 0.1
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void InitForm();

    void UdpRec(QString s0,QString s1,QString s2,QString s3,QString s4,QString s5);
    void TcpRec(QString s0,QString s1,QString s2,QString s3,QString s4,QString s5);
private:
    Ui::Widget *ui;
    QUdpSocket *UdpSerice;
    QTcpSocket *TcpClient;
    QSqlDatabase db;
    QTimer *mainTimer;
};
#endif // WIDGET_H
