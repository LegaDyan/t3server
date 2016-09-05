#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "filetransserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void processPendingData();

public slots:
    void changeLabel(QString);

private:
    Ui::MainWindow *ui;
    QUdpSocket udpSocket;
};

#endif // MAINWINDOW_H
