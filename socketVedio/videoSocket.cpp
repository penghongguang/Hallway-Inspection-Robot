#include "videoSocket.h"
#include <QImage>
#include <QTcpSocket>
#include <QHostAddress>
#include <QPixmap>
#include <QDataStream>
#include "opencv2/opencv.hpp"

videoSocket::videoSocket(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisConnected()));
}

void videoSocket::connectTo(QString ip, int port)
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

void videoSocket::onConnected()
{
    isConnected = true;

    QHostAddress peerAddress = socket->peerAddress();
    quint16 peerPort = socket->peerPort();
    emit addInfo(QString("connected to %1:%2").arg(peerAddress.toString()).arg(peerPort), 0);

    qDebug() << "Connected to server";
}

void videoSocket::onReadyRead()
{
    if(!sizeGotFlag) //获取图片大小
    {
        QByteArray data = socket->read(4);
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream >> imgSize;
//        emit addInfo(QString("image size is %1").arg(imgSize), 0);

        sizeGotFlag = true;
        buffer = ""; //清除之前的数据
    }
    else //获取数据
    {
        QByteArray data = socket->readAll();
        buffer += data;

        if(buffer.size() >= imgSize)
        {
            // 对数据进行预处理
            cv::Mat image = cv::imdecode(std::vector<uchar>(buffer.begin(), buffer.end()), cv::IMREAD_COLOR);
            if (!image.empty()) {
                // 转换图像格式
                cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
//                cv::imshow("test", image);
//                cv::waitKey(1);

                // 创建QImage
                QImage qimage((uchar*)image.data, image.cols, image.rows, QImage::Format_RGB888);
                qimage = qimage.scaled(imgW,imgH);

                emit updateImage(qimage); //发送信号更新图片
            }
            buffer = "";

            socket->write("ack"); //应答信号
            sizeGotFlag = false;
        }
    }
}

void videoSocket::onDisConnected()
{
    isConnected = false;
    sizeGotFlag = false;

    QHostAddress peerAddress = socket->peerAddress();
    quint16 peerPort = socket->peerPort();
    emit addInfo(QString("Disconnected from %1:%2").arg(peerAddress.toString()).arg(peerPort), 1);
}

