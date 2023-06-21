#include "socket.h"
#include <QHostAddress>
#include <QDataStream>

SocketClient::SocketClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisConnected()));
}

void SocketClient::connectTo(QString ip, int port)
{
    socket->abort();
    socket->connectToHost(ip, port);

    delayms(1000); //延时1s
    if(socket->state() != QAbstractSocket::ConnectedState)
    {
        // 超时处理
        socket->abort();
    }
}

void SocketClient::onConnected()
{
    isConnected = true;

    QHostAddress peerAddress = socket->peerAddress();
    quint16 peerPort = socket->peerPort();
    emit addInfo(QString("connected to %1:%2").arg(peerAddress.toString()).arg(peerPort), 0);

    qDebug() << "Connected to server";
    QByteArray data = "12345678user     ";
    socket->write(data);
    socket->flush(); //立即发送
}

void SocketClient::onReadyRead()
{
    receiveBag bag;

//    qDebug() << "Data received";
    QByteArray data = socket->readAll();
//    qDebug() << data;

    //注意解包时为double，打包时也要用double
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream >> bag.CO2 >> bag.TVOC >> bag.T >> bag.H;

    emit receiveData(bag);
}

void SocketClient::onDisConnected()
{
    isConnected = false;

    QHostAddress peerAddress = socket->peerAddress();
    quint16 peerPort = socket->peerPort();
    emit addInfo(QString("Disconnected from %1:%2").arg(peerAddress.toString()).arg(peerPort), 1);
}


