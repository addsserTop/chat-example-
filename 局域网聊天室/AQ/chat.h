#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include <QTimer>
#include <QFileDialog>
#include <QVector>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

namespace Ui {
class chat;
}

class chat : public QWidget
{
    Q_OBJECT

public:
    explicit chat(QString s0,QString s1,QString s2,QWidget *parent = nullptr);
    ~chat();
    void InitForm();

private slots:
    void on_selectBtn_clicked();

    void on_sendBtn_clicked();

private:
    Ui::chat *ui;
    QString MyIP;//我的IP地址
    QString FrIP;//朋友的IP地址
    QString FrName;//朋友的名字
    QString FileS;
};

#endif // CHAT_H
