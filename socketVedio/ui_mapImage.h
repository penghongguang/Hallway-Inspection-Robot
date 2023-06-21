/********************************************************************************
** Form generated from reading UI file 'mapImage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPIMAGE_H
#define UI_MAPIMAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_mapImage
{
public:

    void setupUi(QDialog *mapImage)
    {
        if (mapImage->objectName().isEmpty())
            mapImage->setObjectName(QString::fromUtf8("mapImage"));
        mapImage->resize(400, 300);

        retranslateUi(mapImage);

        QMetaObject::connectSlotsByName(mapImage);
    } // setupUi

    void retranslateUi(QDialog *mapImage)
    {
        mapImage->setWindowTitle(QCoreApplication::translate("mapImage", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mapImage: public Ui_mapImage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPIMAGE_H
