#ifndef MAPIMAGE_H
#define MAPIMAGE_H

#include <QPainter>
#include <QMouseEvent>
#include <QDialog>

namespace Ui {
class mapImage;
}

class mapImage : public QDialog
{
    Q_OBJECT

public:
    mapImage(QDialog *parent = nullptr);
    ~mapImage();

    QPoint point; //选择的点

private:
    Ui::mapImage *ui;

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void addInfo(QString info, int level);
    void setDestination(QPoint pos);
};


#endif // MAPIMAGE_H
