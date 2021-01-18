#ifndef CENTRALGRAPH_H
#define CENTRALGRAPH_H

#include <QWidget>
#include <QPainter>

class centralgraph:public QWidget
{
private:
    int width,height;
    QStringList Source;
    QVector <int> colors;
    QVector <std::list <int>> graph;
    QVector <QColor> cols;
    QVector <QPoint> points;
    QString selectedvertex;
public:
    centralgraph(int w,int h);
    void setVertex(QString v);
    void setGraphColoring(QVector <int> &cls);
    void setResources(QStringList &res);
    void setGraph(QVector <std::list <int>> &g);
    void setColors(QVector <QColor> &cls);
    void paintEvent(QPaintEvent *event) override;
};

#endif // CENTRALGRAPH_H
