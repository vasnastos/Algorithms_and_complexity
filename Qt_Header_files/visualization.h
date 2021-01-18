#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QPainter>
#include <QDesktopWidget>
#include <random>
#include <QPoint>
#include <chrono>

class visualization:public QWidget
{
private:
    std::list <int> adjacency_list;
    QVector <QPoint> points;
    QVector <QColor> colors;
    bool detectcollision(QPoint &p);
    void fillTargetPoints();
    QColor startcolor;
    QString selectedvertex;
public:
    static QStringList vertexnames;
    static QString selvertex;
    visualization(int width,int height);
    void clear();
    static QString getSelectedVertex();
    void setStartColor(const QColor &c);
    void setSelectedVertex(QString a_vertex);
    void setList(QVector<int> &al);
    void setColors(QVector <QColor> &cls);
    void paintEvent(QPaintEvent *event);
};

#endif // VISUALIZATION_H
