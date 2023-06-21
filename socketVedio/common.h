#ifndef COMMON_H
#define COMMON_H

#include <QPoint>
#include <QMetaType>

struct receiveBag
{
    int CO2;
    int TVOC;
    int T;
    int H;
};

Q_DECLARE_METATYPE(receiveBag) // 声明结构体为元对象 信号与槽机制需要注册自定义类型

#endif // COMMON_H
