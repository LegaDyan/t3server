#ifndef FILETRANSSERVER_H
#define FILETRANSSERVER_H

#include <QThread>
#include <QtNetwork>
#include <QFile>
#include <QDataStream>

class fileTransServer : public QObject
{
    Q_OBJECT

public:
    fileTransServer(QString ip, QString port, QString filename);
    QTcpSocket* tcpClient;

public slots:
    void run();
private:
    QFile *localFile;  //要发送的文件
    qint64 totalBytes;  //数据总大小
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
    qint64 loadSize;   //每次发送数据的大小
    QString fileName;  //保存文件路径
    QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据
    QString ip;
    int port;

signals:
    void updateLabel(QString);


private slots:
    void startTransfer();  //发送文件大小等信息
    void updateClientProgress(qint64); //发送数据，更新进度条
};

#endif // FILETRANSSERVER_H
