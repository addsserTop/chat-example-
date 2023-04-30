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
    setFixedSize(300,700);
    setWindowTitle(tr("AQ.exe"));
    //首先检查是否为唯一进程 一台机器只允许有一个AQ，这是与互联网聊天软件不同的
    HANDLE handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    PROCESSENTRY32 pe32;
    pe32.dwSize=sizeof (PROCESSENTRY32);
    while (Process32Next(handle, &pe32))
    {
        QString processName = QString::fromWCharArray(pe32.szExeFile);
        if(processName=="AQ.exe")
        {
            iFlag++;
        }
    }

    MainTimer=new QTimer;
    MainTimer->start(1000);
    connect(MainTimer,&QTimer::timeout,this,[=](){
        if(iFlag>1)
        {
            //退出
            QMessageBox::warning(this,tr("警告"),tr("检测到多个AQ.exe运行，即将退出"));
            close();
        }
    });

    //必要检测完成，进行初始化
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&  ipAddressesList.at(i).toIPv4Address())
        {
            UIP.append(ipAddressesList.at(i).toString());
            //存储IP
        }
    }

    USER=QHostInfo::localHostName();
    ui->UserName->setText(USER);
    UdpSocket=new QUdpSocket;
    UdpSocket->bind(QHostAddress::Any,LPORT);
    TcpServer=new QTcpServer;
    TcpServer->listen(QHostAddress::Any,46000);
    TcpSocket=new QTcpSocket;
    AcTimer=new QTimer;
    AcTimer->start(3000);

    connect(TcpServer,&QTcpServer::newConnection,this,[=](){
            TcpSocket=TcpServer->nextPendingConnection();
            connect(TcpSocket, &QTcpSocket::readyRead, [=](){
                QByteArray data = TcpSocket->readAll();
                QDataStream in(&data, QIODevice::ReadOnly);
                quint64 fileSize = 0;
                QString receivedFileName;
                in >> fileSize >> receivedFileName;

                // 创建并打开文件
                QString fileName;//可以自定义文件路径
                QFile file(fileName);
                if (!file.open(QIODevice::WriteOnly)) {
                    qDebug() << "无法创建文件";
                    return;
                }

                // 从 socket 中接收文件内容
                qint64 bytesReceived = 0;
                while (bytesReceived < fileSize) {
                    if (!TcpSocket->waitForReadyRead()) {
                        qDebug() << "等待客户端数据失败:" << TcpSocket->errorString();
                        return;
                    }
                    qint64 blockSize = 4096;
                    qint64 bytesToRead = fileSize - bytesReceived;
                    if (bytesToRead > blockSize) {
                        bytesToRead = blockSize;
                    }
                    QByteArray buffer = TcpSocket->read(bytesToRead);
                    bytesReceived += file.write(buffer);
                }

                // 关闭文件和 socket
                file.close();
                TcpSocket->close();
                TcpSocket->deleteLater();
            });
    });
    connect(UdpSocket,&QUdpSocket::readyRead,this,[=](){
        //接收UDP消息
        while(UdpSocket->hasPendingDatagrams())
        {
            QByteArray array;
            array.resize(UdpSocket->pendingDatagramSize());
            UdpSocket->readDatagram(array.data(),array.size());
            QJsonObject obj=QJsonDocument::fromJson(array).object();
            QString Version=obj.value("VERSION").toString();
            QString Flage=obj.value("FLAGE").toString();
            QString Id=obj.value("ID").toString();
            QString User=obj.value("USER").toString();
            QString Time=obj.value("TIME").toString();
            QString Data=obj.value("DATA").toString();
            //关键处理函数
        }
    });
    connect(AcTimer,&QTimer::timeout,this,[=](){
        //心跳发送本机信息
        QJsonObject obj;
        obj.insert("VERSION",AQVERSION);
        obj.insert("FLAGE","0000");
        obj.insert("USER",USER);
        obj.insert("TIME",QDateTime::currentDateTime().toString());
        obj.insert("DATA","");
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray arr=doc.toJson();
        UdpSocket->writeDatagram(arr.data(),arr.size(),QHostAddress::Broadcast,SPORT);
    });
}
