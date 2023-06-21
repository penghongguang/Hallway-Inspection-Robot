#ifndef VIDEOSOCKET_H
#define VIDEOSOCKET_H

#include <QThread>
#include <QImage>
#include <QTcpSocket>
#include "QtCore/QDateTime"
#include <QCoreApplication>

class videoSocket : public QObject
{
    Q_OBJECT

public:
    explicit videoSocket(QObject *parent = nullptr);
    void connectTo(QString ip, int port);

    bool isConnected = false;
    int imgW = 960;
    int imgH = 540;

public slots:
    void onConnected();
    void onReadyRead();
    void onDisConnected();

private:
    QTcpSocket *socket;
    bool sizeGotFlag = false;
    int imgSize;
    QByteArray buffer;

    void delayms(int msec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(msec);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

signals:
    void updateImage(QImage img);
    void addInfo(QString info, int level);
};

#endif // VIDEOSOCKET_H
