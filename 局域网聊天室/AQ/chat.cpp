#include "chat.h"
#include "ui_chat.h"

chat::chat(QString s0,QString s1,QString s2,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chat)
{
    ui->setupUi(this);
    MyIP=s0;
    FrIP=s1;
    FrName=s2;
    InitForm();
}

chat::~chat()
{
    delete ui;
}

void chat::InitForm()
{
    //
    setWindowTitle(FrName);
    setFixedSize(600,500);
    ui->selectEdit->setEnabled(false);
}

void chat::on_selectBtn_clicked()
{
    //选择文件
    FileS=QFileDialog::getOpenFileName(this, tr("选择DLL文件"), "/", tr("Image Files (*.dll)"));
    ui->selectEdit->setText(FileS);

}

void chat::on_sendBtn_clicked()
{
    QFile file(FileS);
    QTcpSocket socket;
    socket.connectToHost(FrIP, 46000);
    if (!socket.waitForConnected()) {
            QMessageBox::warning(this,"警告","发送失败，请检查连接");
            return;
        }
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << quint64(0) << FileS << qint64(file.size());
    out.device()->seek(0);
    out << quint64(block.size() - sizeof(quint64));
    socket.write(block);
    socket.waitForBytesWritten();
    ui->progressBar->setMaximum(file.size());
    qint64 Size=0;
    while (!file.atEnd()) {
            QByteArray buffer = file.read(4096);
            socket.write(buffer);
            socket.waitForBytesWritten();
            Size+=4096;
            ui->progressBar->setValue(Size);
        }
    file.close();
    socket.close();
}
