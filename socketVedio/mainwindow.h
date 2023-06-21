#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoSocket.h"
#include "common.h"
#include "socket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void systemInit();
    void delayms(int msec);
    void initTVInfo();

private slots:
    void on_pushButton_clicked();

    void slotContextMenu(QPoint);
    void MenuActionSave();
    void MenuActionClear();
    void AddInfo(QString strInfo, int level);
    void updateImg(QImage img);

    void receiveData(receiveBag bag);

private:
    Ui::MainWindow *ui;

    QMenu*  TVTablePopMenu; //信息显示区右键菜单
    QString IPType; //信息地址/视频地址
    videoSocket* videosocket; //视频获取接口
    struct receiveBag receivebag; //接收的数据包
    int timerId; //定时器ID
    SocketClient* socketClient; //socket客户端
    QString socketIP;
    int socketPORT;
    unsigned long int systemRunTime;

    void timerEvent(QTimerEvent * event) override;
};
#endif // MAINWINDOW_H
