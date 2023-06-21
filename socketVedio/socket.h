#ifndef SOCKET_H
#define SOCKET_H

#include <QTcpSocket>
#include "common.h"
#include "QtCore/QDateTime"
#include <QCoreApplication>

class SocketClient : public QObject
{
    Q_OBJECT

public:
    explicit SocketClient(QObject *parent = nullptr);
    void connectTo(QString ip, int port);

    bool isConnected = false;

public slots:
    void onConnected();
    void onReadyRead();
    void onDisConnected();

private:
    QTcpSocket *socket;

    void delayms(int msec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(msec);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

signals:
    void addInfo(QString info, int level);
    void receiveData(receiveBag bag);
};

#endif // SOCKET_H
