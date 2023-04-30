#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUdpSocket>
#include <QLibrary>//AFS1:安全检测DLL
#include <QMessageBox>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>
#include <QProcess>
#include <windows.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#define SPORT 10000//默认发送端口 10000
#define LPORT 20000//默认接收端口 20000
#define VERSION 0.1
typedef int(*VP)(DWORD PID);

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void InitLogin();//初始化登录界面
    void CoreContain(QString s0,QString s1,QString s2,QString s3,QString s4,QString s5);//UDP核心接收处理
    void Hearts();//心跳包
    void Verify();//进程检查

private slots:
    void on_LOGIN_BUTTON_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_commandLinkButton_clicked();

private:
    Ui::Widget *ui;
    int nT;
    QString ID;//账号
    QString PWD;//密码
    QUdpSocket *socket;
    QTimer *timerHearts;
    QTimer *timer;
};
#endif // WIDGET_H
