#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QtCore/QDateTime"
#include <QFileDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<receiveBag>("receiveBag");// 注册结构体类型到元对象系统中

    setFocus(); //设置焦点，实现键盘响应
    systemInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::systemInit()
{
    IPType = QString("控制信息");
    ui->ipLineEdit->setText("10.16.207.115");
    ui->portLineEdit->setText("1235");

    initTVInfo();
    AddInfo(QString("TVTbale is ready!"), 0);

    // 视频socket
    videosocket = new videoSocket;
    connect(videosocket, SIGNAL(addInfo(QString,int)), this, SLOT(AddInfo(QString,int)));
    connect(videosocket, SIGNAL(updateImage(QImage)), this, SLOT(updateImg(QImage)));

    // 信息socket
    socketIP = QString("10.16.200.7");
    socketPORT = 1234;
    socketClient = new SocketClient;
    connect(socketClient, SIGNAL(addInfo(QString,int)), this, SLOT(AddInfo(QString,int)));
    connect(socketClient, SIGNAL(receiveData(receiveBag)), this, SLOT(receiveData(receiveBag)));
    socketClient->connectTo(socketIP, socketPORT);

    ui->portLineEdit->setValidator(new QIntValidator(0, 50000, this)); //设置限制数字输入

    systemRunTime = 0;
    timerId = this->startTimer(100); //启动定时器
}

void MainWindow::receiveData(receiveBag bag)
{
    receivebag = bag;
}


void MainWindow::on_pushButton_clicked()
{
    if(ui->ipLineEdit->text()=="" || ui->portLineEdit->text()=="")
    {
        AddInfo(QString("warning: please input ip and port."), 1);
        return;
    }

    videosocket->connectTo(ui->ipLineEdit->text(), ui->portLineEdit->text().toInt());

}

void MainWindow::updateImg(QImage img)
{
    ui->TV->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::delayms(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::initTVInfo()
{
    QStandardItemModel *student_model = new QStandardItemModel();
    student_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("时间")));
    student_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("内容")));
    //利用setModel()方法将数据模型与QTableView绑定
    ui->TVTable->setModel(student_model);

    //设置选中时为整行选中
    ui->TVTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->TVTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHeaderView *header = ui->TVTable->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);// 设置列的自适应模式为 ResizeToContents
    header->resizeSections(QHeaderView::ResizeToContents); // 根据设置的自适应模式自动调整列宽
    ui->TVTable->horizontalHeader()->setStretchLastSection(true);//最后一列填充整个表

    //不显示行号
    ui->TVTable->verticalHeader()->hide();

    //隐藏列头
    ui->TVTable->horizontalHeader()->hide();

    //隐藏水平滚动条
    ui->TVTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //自动滚动到底部
    ui->TVTable->scrollToBottom();

    //添加右键菜单
    ui->TVTable->setContextMenuPolicy(Qt::CustomContextMenu);
    TVTablePopMenu = new QMenu(ui->TVTable);         //这里不打算释放，程序退出后操作系统会自动清理
    QAction *actionSave = new QAction();
    QAction *actionClear = new QAction();
    actionSave->setText(QString("保存"));
    actionSave->setIcon(QIcon(":/ico/save.png"));   //给菜单添加图标
    actionClear->setText(QString("清空"));
    actionClear->setIcon(QIcon(":/ico/Clean.png"));
    TVTablePopMenu->addAction(actionSave);
    TVTablePopMenu->addAction(actionClear);

    connect(ui->TVTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));
    connect(actionSave,SIGNAL(triggered()),this,SLOT(MenuActionSave()));
    connect(actionClear,SIGNAL(triggered()),this,SLOT(MenuActionClear()));
}

void MainWindow::slotContextMenu(QPoint)
{
    TVTablePopMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}

//点击了保存菜单
void MainWindow::MenuActionSave()
{
    QFileDialog *fileDialog = new QFileDialog(this);        //创建一个QFileDialog对象，构造函数中的参数可以有所添加。
    fileDialog->setWindowTitle(tr("保存日志"));              //设置文件保存对话框的标题
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);     //设置文件对话框为保存模式
    fileDialog->setFileMode(QFileDialog::AnyFile);          //设置文件对话框弹出的时候显示任何文件，不论是文件夹还是文件
    fileDialog->setViewMode(QFileDialog::Detail);           //文件以详细的形式显示，显示文件名，大小，创建日期等信息；
    fileDialog->setDirectory(".");                          //设置文件对话框打开时初始打开的位置
    fileDialog->selectFile("运行日志.txt");                      //设置默认的文件名
    QStringList nameFilters;
    nameFilters << "文本文件(*.txt)";
    fileDialog->setNameFilters(nameFilters);//设置文件类型过滤器
    if(fileDialog->exec() == QDialog::Accepted)
    {
        //注意使用的是QFileDialog::Accepted或者QDialog::Accepted,不是QFileDialog::Accept
        QString path = fileDialog->selectedFiles()[0];//得到用户选择的文件名
        //打开文件
        QFile SaveFile(path);
        if(!SaveFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            //无法打开文件
            AddInfo(QString("unable to open file ") + path, 2);
            delete fileDialog;
            return;
        }
        QTextStream outFile(&SaveFile);
        //保存日志信息
        QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->TVTable->model());
        int rowcount = model->rowCount();
        for(int i = 0; i < rowcount; i++)
        {
            //获取时间
            QModelIndex index = ui->TVTable->model()->index(i, 0);
            QString strdata = ui->TVTable->model()->data(index).toString();
            strdata += "  ";
            //获取内容
            index = ui->TVTable->model()->index(i, 1);
            strdata += ui->TVTable->model()->data(index).toString();
            strdata += "\r\n";
            outFile << strdata;
        }
        SaveFile.close();
    }
    delete fileDialog;
}

//点击了清空菜单
void MainWindow::MenuActionClear()
{
    QMessageBox messageBox(QMessageBox::NoIcon,"清除日志", "你确定要清除所有日志信息吗?",QMessageBox::Yes | QMessageBox::No, NULL);
    if(messageBox.exec() == QMessageBox::Yes)
    {
        QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->TVTable->model());
        model->removeRows(0,model->rowCount());
    }

}

//向实时信息显示区添加信息，iLevel为显示等级：0->提示信息；1->报警；2->故障；3->故障恢复
void MainWindow::AddInfo(QString info, int level)
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->TVTable->model());

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    int iIndex = model->rowCount();

    model->setItem(iIndex, 0, new QStandardItem(current_date));
    model->setItem(iIndex, 1, new QStandardItem(info));
    model->item(iIndex, 0)->setForeground(QBrush(QColor(0, 0, 0)));
    model->item(iIndex, 1)->setForeground(QBrush(QColor(0, 0, 0)));

    //报警和故障信息用红色显示
    if(level == 2)
    {
        model->item(iIndex, 0)->setForeground(QBrush(QColor(255, 0, 0)));
        model->item(iIndex, 1)->setForeground(QBrush(QColor(255, 0, 0)));
    }
    else if(level == 1)   //黄色
    {
        model->item(iIndex, 0)->setForeground(QBrush(QColor(205, 205, 0)));
        model->item(iIndex, 1)->setForeground(QBrush(QColor(205, 205, 0)));
    }
    else if(level == 0)    //绿色
    {
        model->item(iIndex, 0)->setForeground(QBrush(QColor(0, 0xa3, 0x5e)));
        model->item(iIndex, 1)->setForeground(QBrush(QColor(0, 0xa3, 0x5e)));
    }
    ui->TVTable->scrollToBottom();
}

void MainWindow::timerEvent(QTimerEvent * event)
{
    if(event->timerId() == timerId)
    {
        if(socketClient->isConnected) // socket连接上才进行通信
        {
            //更新当前状态
            ui->TLcd->display(receivebag.T/10.0);
            ui->HLcd->display(receivebag.H/10.0);
            ui->CO2Lcd->display(receivebag.CO2);
            ui->TVOCLcd->display(receivebag.TVOC);
        }

        if(!socketClient->isConnected && systemRunTime%10==0) // 未连接则1s重连
        {
            socketClient->connectTo(socketIP, socketPORT);
        }

        systemRunTime++;
    }
}
