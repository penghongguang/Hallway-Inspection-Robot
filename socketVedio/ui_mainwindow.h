/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *displayBox;
    QHBoxLayout *controlBox;
    QWidget *videoBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *addressLabel;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *pushButton;
    QLabel *TV;
    QGroupBox *commandBox;
    QVBoxLayout *controlBox_2;
    QSpacerItem *verticalSpacer;
    QGroupBox *statusDisplayBox;
    QGridLayout *gridLayout_2;
    QLCDNumber *HLcd;
    QLabel *angleLabel;
    QLabel *label;
    QLabel *accLablel;
    QLCDNumber *TVOCLcd;
    QLCDNumber *TLcd;
    QLabel *speedNowLable;
    QLCDNumber *CO2Lcd;
    QSpacerItem *verticalSpacer_2;
    QTableView *TVTable;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1300, 900);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 0, -1, 0);
        displayBox = new QGroupBox(centralwidget);
        displayBox->setObjectName(QString::fromUtf8("displayBox"));
        controlBox = new QHBoxLayout(displayBox);
        controlBox->setObjectName(QString::fromUtf8("controlBox"));
        videoBox = new QWidget(displayBox);
        videoBox->setObjectName(QString::fromUtf8("videoBox"));
        verticalLayout = new QVBoxLayout(videoBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, -1, 0, -1);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        addressLabel = new QLabel(videoBox);
        addressLabel->setObjectName(QString::fromUtf8("addressLabel"));

        horizontalLayout->addWidget(addressLabel);

        ipLineEdit = new QLineEdit(videoBox);
        ipLineEdit->setObjectName(QString::fromUtf8("ipLineEdit"));

        horizontalLayout->addWidget(ipLineEdit);

        portLineEdit = new QLineEdit(videoBox);
        portLineEdit->setObjectName(QString::fromUtf8("portLineEdit"));

        horizontalLayout->addWidget(portLineEdit);

        pushButton = new QPushButton(videoBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalLayout->setStretch(1, 3);
        horizontalLayout->setStretch(3, 1);

        verticalLayout->addLayout(horizontalLayout);

        TV = new QLabel(videoBox);
        TV->setObjectName(QString::fromUtf8("TV"));
        TV->setMinimumSize(QSize(960, 540));
        TV->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(TV);


        controlBox->addWidget(videoBox);

        commandBox = new QGroupBox(displayBox);
        commandBox->setObjectName(QString::fromUtf8("commandBox"));
        controlBox_2 = new QVBoxLayout(commandBox);
        controlBox_2->setObjectName(QString::fromUtf8("controlBox_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        controlBox_2->addItem(verticalSpacer);

        statusDisplayBox = new QGroupBox(commandBox);
        statusDisplayBox->setObjectName(QString::fromUtf8("statusDisplayBox"));
        gridLayout_2 = new QGridLayout(statusDisplayBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(10);
        HLcd = new QLCDNumber(statusDisplayBox);
        HLcd->setObjectName(QString::fromUtf8("HLcd"));

        gridLayout_2->addWidget(HLcd, 1, 1, 1, 1);

        angleLabel = new QLabel(statusDisplayBox);
        angleLabel->setObjectName(QString::fromUtf8("angleLabel"));

        gridLayout_2->addWidget(angleLabel, 2, 0, 1, 1);

        label = new QLabel(statusDisplayBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 3, 0, 1, 1);

        accLablel = new QLabel(statusDisplayBox);
        accLablel->setObjectName(QString::fromUtf8("accLablel"));

        gridLayout_2->addWidget(accLablel, 1, 0, 1, 1);

        TVOCLcd = new QLCDNumber(statusDisplayBox);
        TVOCLcd->setObjectName(QString::fromUtf8("TVOCLcd"));

        gridLayout_2->addWidget(TVOCLcd, 3, 1, 1, 1);

        TLcd = new QLCDNumber(statusDisplayBox);
        TLcd->setObjectName(QString::fromUtf8("TLcd"));

        gridLayout_2->addWidget(TLcd, 0, 1, 1, 1);

        speedNowLable = new QLabel(statusDisplayBox);
        speedNowLable->setObjectName(QString::fromUtf8("speedNowLable"));

        gridLayout_2->addWidget(speedNowLable, 0, 0, 1, 1);

        CO2Lcd = new QLCDNumber(statusDisplayBox);
        CO2Lcd->setObjectName(QString::fromUtf8("CO2Lcd"));

        gridLayout_2->addWidget(CO2Lcd, 2, 1, 1, 1);


        controlBox_2->addWidget(statusDisplayBox);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        controlBox_2->addItem(verticalSpacer_2);

        controlBox_2->setStretch(0, 1);
        controlBox_2->setStretch(1, 4);
        controlBox_2->setStretch(2, 1);

        controlBox->addWidget(commandBox);


        verticalLayout_2->addWidget(displayBox);

        TVTable = new QTableView(centralwidget);
        TVTable->setObjectName(QString::fromUtf8("TVTable"));

        verticalLayout_2->addWidget(TVTable);

        verticalLayout_2->setStretch(0, 3);
        verticalLayout_2->setStretch(1, 1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1300, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        addressLabel->setText(QCoreApplication::translate("MainWindow", "\347\275\221\347\273\234\350\247\206\351\242\221\345\234\260\345\235\200\357\274\232", nullptr));
        ipLineEdit->setText(QString());
        ipLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "ip\345\234\260\345\235\200", nullptr));
        portLineEdit->setText(QString());
        portLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\347\275\221\347\273\234\347\253\257\345\217\243", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245", nullptr));
        TV->setText(QString());
        statusDisplayBox->setTitle(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201\346\230\276\347\244\272", nullptr));
        angleLabel->setText(QCoreApplication::translate("MainWindow", "CO2\346\265\223\345\272\246", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "TVOC\346\265\223\345\272\246", nullptr));
        accLablel->setText(QCoreApplication::translate("MainWindow", "\346\271\277\345\272\246", nullptr));
        speedNowLable->setText(QCoreApplication::translate("MainWindow", "\346\270\251\345\272\246", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
