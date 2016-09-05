#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpSocket.bind(5888);
    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeLabel(QString str) {
    ui->label->setText(str);
}

void MainWindow::processPendingData() {
    QByteArray datagram;//拥于存放接收的数据报
    QString ip;
    QString port;
    QString filename;
    do{
        datagram.resize(udpSocket.pendingDatagramSize());//让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
        udpSocket.readDatagram(datagram.data(),datagram.size());//接收数据报，将其存放到datagram中
    }while(udpSocket.hasPendingDatagrams());//拥有等待的数据报
    QDataStream in(&datagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_7);
    in >> ip >> port >> filename;
    ui->label->setText(ip + port + filename);
    update();
    QThread* thread = new QThread;
    fileTransServer* server = new fileTransServer(ip, port, filename);
    server->moveToThread(thread);
    connect(thread, SIGNAL(started()), server, SLOT(run()));
    connect(server, SIGNAL(updateLabel(QString)), this, SLOT(changeLabel(QString)));
    connect(server->tcpClient,SIGNAL(connected()),server,SLOT(startTransfer()));
    connect(server->tcpClient,SIGNAL(bytesWritten(qint64)),server,SLOT(updateClientProgress(qint64)));
    thread->start();
}
