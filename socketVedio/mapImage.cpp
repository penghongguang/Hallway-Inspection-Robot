#include "mapImage.h"
#include "ui_mapImage.h"

mapImage::mapImage(QDialog *parent):
    QDialog(parent),
    ui(new Ui::mapImage)
{
    ui->setupUi(this);

    setFixedSize(960,540);
    setWindowTitle("select a pose");
}

mapImage::~mapImage()
{

}

void mapImage::paintEvent(QPaintEvent *)
{
    // 加载图片
    QPixmap pixmap(":/sources/map.pgm");

    // 绘制图片
    QPainter painter(this);
    painter.drawPixmap(rect(), pixmap, QRect());

    // Draw a circle at the stored point
    if (!point.isNull())
    {
        QPen pen(Qt::red);
        pen.setWidth(6);
        pen.setCapStyle(Qt::RoundCap); //圆形画笔
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPoint(point);
    }
}

void mapImage::mousePressEvent(QMouseEvent *event)
{
    point = event->pos();
    emit addInfo(QString("select pose (%1,%2)").arg(point.x()).arg(point.y()), 0);
    emit setDestination(point);
    update();
}
