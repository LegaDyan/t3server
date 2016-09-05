#include "filetransserver.h"

fileTransServer::fileTransServer(QString ip, QString port, QString filename) :ip(ip), port(port.toInt()), fileName(filename)
{
    loadSize = 4*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    tcpClient = new QTcpSocket;

}

void fileTransServer::startTransfer()  //实现文件大小等信息的发送
{
    qDebug() << "start to trans";
    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }
    totalBytes = localFile->size();
    //文件总大小
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    sendOut << qint64(0) << qint64(0) << currentFileName;
    //依次写入总大小信息空间，文件名大小信息空间，文件名
    totalBytes += outBlock.size();
    //这里的总大小是文件名大小等信息和实际文件大小的总和
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));
    //返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    bytesToWrite = totalBytes - tcpClient->write(outBlock);
    qDebug() << totalBytes << bytesToWrite;
    //发送完头数据后剩余数据的大小
    emit updateLabel("Connected");
    outBlock.resize(0);
}

void fileTransServer::updateClientProgress(qint64 numBytes) //更新进度条，实现文件的传送
{
    qDebug() << "Get Here";
    qDebug() << bytesWritten << totalBytes;
    bytesWritten += (int)numBytes;
    if(bytesToWrite > 0) //如果已经发送了数据
    {
        outBlock = localFile->read(qMin(bytesToWrite,loadSize));
        bytesToWrite -= (int)tcpClient->write(outBlock);
        outBlock.resize(0);
    }
    else
    {
        localFile->close(); //如果没有发送任何数据，则关闭文件
    }
    if(bytesWritten == totalBytes) //发送完毕
    {
        emit updateLabel("Succeed to trans" + fileName);
        localFile->close();
        tcpClient->close();
        return;
    }
}

void fileTransServer::run() {
    bytesWritten = 0;
    emit updateLabel("connecting");
    qDebug() << ip << port;
    tcpClient->connectToHost(QHostAddress(ip), port);//连接
}
