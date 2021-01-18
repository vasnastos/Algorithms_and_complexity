#include "centralgraph.h"

centralgraph::centralgraph(int w,int h):width(w),height(h),selectedvertex("")
{

}

void centralgraph::setVertex(QString v)
{
    this->selectedvertex=v;
    this->points.clear();
    this->colors.clear()
}

void centralgraph::setGraphColoring(QVector<int > &cls)
{
    this->colors=cls;
    this->repaint();
}

void centralgraph::setGraph(QVector<std::list<int> > &g)
{
    this->graph=g;
}

void centralgraph::setColors(QVector<QColor> &cls)
{
    cols=cls;
}

void centralgraph::setResources(QStringList &res)
{
  this->Source=res;
}

void centralgraph::paintEvent(QPaintEvent *event)
{
   QPainter p(this);
   p.setPen(Qt::black);
   p.drawRect(0,0,this->width-1,this->height-1);
   
   for()
   //color for edges-->#e6ffff
   //By coloring algorithm colorAppropriate the vertices
}
