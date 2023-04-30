#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    InitLogin();
    //Verify();//暂时停用
}

Widget::~Widget()
{
    delete ui;
}

void Widget::InitLogin()
{
    //初始化登录界面
    ui->back->hide();
    ui->front->show();
    setFixedSize(420,340);
    setWindowTitle(tr("ASLogin"));

    nT=0;
    timer=new QTimer(this);
    timerHearts=new QTimer(this);
    timerHearts->start(5000);
    socket=new QUdpSocket(this);
    socket->bind(LPORT);

    connect(socket,&QUdpSocket::readyRead,this,[=](){
        while(socket->hasPendingDatagrams())
        {
            QByteArray array;
            array.resize(socket->pendingDatagramSize());
            socket->readDatagram(array.data(),array.size());
            QJsonObject obj=QJsonDocument::fromJson(array).object();
            QString Version=obj.value("VERSION").toString();
            QString Flage=obj.value("FLAGE").toString();
            QString Id=obj.value("ID").toString();
            QString User=obj.value("USER").toString();
            QString Time=obj.value("TIME").toString();
            QString Data=obj.value("DATA").toString();
            //关键处理函数
            CoreContain(Version,Flage,Id,User,Time,Data);
        }
    });
    connect(timer,&QTimer::timeout,this,[=](){
        QMessageBox::warning(this,"警告","登录超时");
        ui->front->show();
        ui->back->hide();
        ui->ID_EDIT->clear();
        ui->PWD_EDIT->clear();
        timer->stop();
    });
    connect(timerHearts,&QTimer::timeout,this,[=](){
        if(nT>=3)
        {
            QMessageBox::warning(this,tr("警告"),tr("后台服务未启动"));
            //尝试启动后台服务--------
            QStringList list;
            list<<"start";
            QProcess Pro;
            Pro.start(QDir::homePath()+"/Addsser/ALL/AFServer.exe",list);
        }
        nT++;
    });
}

void Widget::CoreContain(QString s0, QString s1, QString s2, QString s3, QString s4, QString s5)
{
    UNREFERENCED_PARAMETER(s0);
    UNREFERENCED_PARAMETER(s3);
    UNREFERENCED_PARAMETER(s4);
    switch (s1.toUInt())
    {
    case 0000:
        if(s5=="YES")
        {
            //后台服务在线
            nT=0;
        }
        break;
    case 0001:
        if(s2==ID&&s5=="YES")
        {
            //登陆成功 启动QUser.exe
            QStringList list;
            list<<s3<<s2;
            QProcess Pro;
            Pro.start(QDir::currentPath()+"/QUser.exe",list);
            qApp->closeAllWindows();
        }else if (s2==ID&&s5=="NO")
        {
            //登陆失败
            QMessageBox::warning(this,"警告","密码错误");
            timer->stop();
            ui->PWD_EDIT->clear();
        }
        break;
    default:break;
    }
}
void Widget::Verify()
{
    //检查进程环境
    QLibrary lib("AFS1.dll");
    if(!lib.load())
    {
        QMessageBox::warning(this,tr("警告"),tr("文件缺失，退出登录"));
        qApp->closeAllWindows();
    }
    VP svp=(VP)lib.resolve("ProcessVerify");
    int result=svp(GetCurrentProcessId());
    if(result==2)
    {
        QMessageBox::warning(this,tr("警告"),tr("检测到多个登陆界面"));
        qApp->closeAllWindows();
    }
}
void Widget::on_LOGIN_BUTTON_clicked()
{
    //登录点击处理
    if(ui->ID_EDIT->text().isEmpty())
    {
        QMessageBox::warning(this,tr("警告"),tr("账号未输入"));
        return;
    }
    if(ui->PWD_EDIT->text().isEmpty())
    {
        QMessageBox::warning(this,tr("警告"),tr("密码未输入"));
        return;
    }
    ID=ui->ID_EDIT->text();
    PWD=ui->PWD_EDIT->text();
    QJsonObject obj;
    obj.insert("VERSION",VERSION);
    obj.insert("FLAGE","0001");
    obj.insert("ID",ID);
    obj.insert("USER","");
    obj.insert("TIME",QDateTime::currentDateTime().toString());
    obj.insert("DATA",PWD);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray arr=doc.toJson();
    socket->writeDatagram(arr.data(),arr.size(),QHostAddress::Broadcast,SPORT);
    socket->flush();
    ui->front->hide();
    ui->back->show();
    timer->start(10000);
}
void Widget::Hearts()
{
    QJsonObject obj;
    obj.insert("VERSION",VERSION);
    obj.insert("FLAGE","0000");
    obj.insert("ID","");
    obj.insert("USER","");
    obj.insert("TIME",QDateTime::currentDateTime().toString());
    obj.insert("DATA","");
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray arr=doc.toJson();
    socket->writeDatagram(arr.data(),arr.size(),QHostAddress::Broadcast,SPORT);
    socket->flush();
}

void Widget::on_checkBox_stateChanged(int arg1)
{
    //保存密码
    if(arg1==2)
    {
        //选中
    }
    else if(arg1==0)
    {
        //未选中
    }
}

void Widget::on_commandLinkButton_clicked()
{
    //打开网页
    //作者的网站还没弄好，所以打开什么都没有
    QDesktopServices::openUrl(QUrl("https://addsser.top", QUrl::TolerantMode));
}
