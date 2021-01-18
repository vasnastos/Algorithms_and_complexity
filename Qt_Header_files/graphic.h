#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QObject>
#include <QPoint>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <cmath>


//Σχεδίαση γειτονικών κορυφών με μία κορυφή.
class graphic:public QWidget
{
    Q_OBJECT
private:
    QPoint init;
    QString source;
    QStringList targs;
    int width;
    int height;
    bool detectcollision(const QPoint &p);
    void RandomizeTargetPoints();
    void Intiatiate();
    QVector <QPoint> targets;
public:
    graphic(int xaxis,int yaxis);
    void Reset();
    void setSource(QString s);
    void setTargets(QStringList &targ);
    void paintEvent(QPaintEvent *e);
};

#endif // GRAPHIC_H
