#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    InitForm();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::InitForm()
{
    mainTimer=new QTimer;
    TcpClient=new QTcpSocket;
    UdpSerice=new QUdpSocket;

    TcpClient->connectToHost(IP,PORT);
    if(TcpClient->waitForConnected(3000))
    {
        QMessageBox::warning(this,tr("警告"),tr("无法连接服务器，请联系管理员"));
    }

    QString Str=QDir::homePath()+"/Addsser/ALL/message.db";
    QFile File(Str);
    if(!File.exists())
    {
        db=QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(Str);
        db.setHostName("192.168.1.1");
        db.setUserName("root");
        db.setPort(3306);
        db.setPassword("root");
        db.open();
        QSqlQuery query(db);
        QString sql=QString("create table msg(id varchar(20),name varchar(10),message varchar(1024))");
        query.exec(sql);
    }
    else
    {
        db=QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(Str);
        db.setHostName("192.168.1.1");
        db.setUserName("root");
        db.setPort(3306);
        db.setPassword("root");
        db.open();
    }

    connect(mainTimer,&QTimer::timeout,this,[=](){
        QJsonObject obj;
        obj.insert("VERSION",VERSION);
        obj.insert("FLAGE",0000);
        obj.insert("ID","");
        obj.insert("USER","");
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA","");
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        TcpClient->write(arr.data(),arr.size());
        TcpClient->flush();
    });

    connect(UdpSerice,&QUdpSocket::readyRead,this,[=](){
        while(UdpSerice->hasPendingDatagrams())
        {
            QByteArray array;
            array.resize(UdpSerice->pendingDatagramSize());
            UdpSerice->readDatagram(array.data(),array.size());
            QJsonObject obj=QJsonDocument::fromJson(array).object();
            QString Version=obj.value("VERSION").toString();
            QString Flage=obj.value("FLAGE").toString();
            QString Id=obj.value("ID").toString();
            QString User=obj.value("USER").toString();
            QString Time=obj.value("TIME").toString();
            QString Data=obj.value("DATA").toString();
            //关键处理函数
            UdpRec(Version,Flage,Id,User,Time,Data);
        }
    });
    connect(TcpClient,&QTcpSocket::readyRead,this,[=](){
        QByteArray array=TcpClient->readAll();
        QJsonObject obj=QJsonDocument::fromJson(array).object();
        QString Version=obj.value("VERSION").toString();
        QString Flage=obj.value("FLAGE").toString();
        QString Id=obj.value("ID").toString();
        QString User=obj.value("USER").toString();
        QString Time=obj.value("TIME").toString();
        QString Data=obj.value("DATA").toString();
        TcpRec(Version,Flage,Id,User,Time,Data);
    });

    connect(TcpClient,&QTcpSocket::disconnected,this,[=](){
        QMessageBox::warning(this,tr("警告"),tr("与服务器连接断开"));
    });

}

void Widget::UdpRec(QString s0,QString s1,QString s2,QString s3,QString s4,QString s5)
{
    switch (s1.toUInt())
    {
    case 0000:
        {
        QJsonObject obj;
        obj.insert("VERSION","");
        obj.insert("FLAGE","0000");
        obj.insert("ID","");
        obj.insert("USER","");
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA","YES");
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        UdpSerice->writeDatagram(arr.data(),arr.size(),QHostAddress::Broadcast,UDPSPORT);
        UdpSerice->flush();
        }
        break;
    case 0001:
        {
        QJsonObject obj;
        obj.insert("VERSION",s0);
        obj.insert("FLAGE",s1);
        obj.insert("ID",s2);
        obj.insert("USER",s3);
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA",s5);
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        TcpClient->write(arr.data(),arr.size());
        TcpClient->flush();
        }
        break;
    case 0002:
        {
        QJsonObject obj;
        obj.insert("VERSION",s0);
        obj.insert("FLAGE",s1);
        obj.insert("ID",s2);
        obj.insert("USER",s3);
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA",s5);
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        TcpClient->write(arr.data(),arr.size());
        TcpClient->flush();
        }
        break;
    case 0003:
        {
        QJsonObject obj;
        obj.insert("VERSION",s0);
        obj.insert("FLAGE",s1);
        obj.insert("ID",s2);
        obj.insert("USER",s3);
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA",s5);
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        TcpClient->write(arr.data(),arr.size());
        TcpClient->flush();
        }
        break;
    case 0004:
        {
        QJsonObject obj;
        obj.insert("VERSION",s0);
        obj.insert("FLAGE",s1);
        obj.insert("ID",s2);
        obj.insert("USER",s3);
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA",s5);
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        TcpClient->write(arr.data(),arr.size());
        TcpClient->flush();
        }
        break;
    case 0005:
        {
        QJsonObject obj;
        obj.insert("VERSION",s0);
        obj.insert("FLAGE",s1);
        obj.insert("ID",s2);
        obj.insert("USER",s3);
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA",s5);
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        TcpClient->write(arr.data(),arr.size());
        TcpClient->flush();
        }
        break;
    case 0006:
        {
        QJsonObject obj;
        obj.insert("VERSION",s0);
        obj.insert("FLAGE",s1);
        obj.insert("ID",s2);
        obj.insert("USER",s3);
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA",s5);
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        TcpClient->write(arr.data(),arr.size());
        TcpClient->flush();
        }
        break;
    case 0007:
        {
        QJsonObject obj;
        obj.insert("VERSION",s0);
        obj.insert("FLAGE",s1);
        obj.insert("ID",s2);
        obj.insert("USER",s3);
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA",s5);
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        TcpClient->write(arr.data(),arr.size());
        TcpClient->flush();
        }
        break;
    default:break;
    }
}

void Widget::TcpRec(QString s0, QString s1, QString s2, QString s3, QString s4, QString s5)
{
    switch (s1.toUInt())
    {
    case 0000:
        {
        //保留
        if(s5=="REFRESH")
        {
            QMessageBox::warning(this,tr("警告"),tr("请更新程序"));
            QJsonObject obj;
            obj.insert("VERSION",VERSION);
            obj.insert("FLAGE",0000);
            obj.insert("ID","");
            obj.insert("USER","QUser");
            obj.insert("TIME",QDateTime::currentDateTime().toString());
            obj.insert("DATA","QUIT");
            QJsonDocument doc;
            doc.setObject(obj);
            QByteArray arr=doc.toJson();
            UdpSerice->writeDatagram(arr.data(),arr.size(),QHostAddress::Broadcast,UDPSPORT);
            UdpSerice->flush();
        }
        }
        break;
    case 0001:
        {
        QJsonObject obj;
        obj.insert("VERSION",s0);
        obj.insert("FLAGE",s1);
        obj.insert("ID",s2);
        obj.insert("USER",s3);
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA",s5);
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        UdpSerice->writeDatagram(arr.data(),arr.size(),QHostAddress::Broadcast,UDPSPORT);
        UdpSerice->flush();
        }
        break;
    case 0002:
        {
        //更新用户数据库，更新聊天信息
        QSqlQuery query(db);
        QString sql=QString("insert into friend msg (%1,%2,%3)").arg(s2).arg(s3).arg(s5);
        query.exec(sql);
        }
        break;
    case 0003:
        {
        //好友添加处理
        QJsonObject obj;
        obj.insert("VERSION",s0);
        obj.insert("FLAGE",s1);
        obj.insert("ID",s2);
        obj.insert("USER",s3);
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA",s5);
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        UdpSerice->writeDatagram(arr.data(),arr.size(),QHostAddress::Broadcast,UDPSPORT);
        UdpSerice->flush();
        }
        break;
    default:break;
    }
}
