#include "visualization.h"
#include <QDebug>

QString visualization::selvertex="";
QStringList visualization::vertexnames=QStringList();

visualization::visualization(int width,int height)
{
   this->setFixedSize(width,height);
}

bool visualization::detectcollision(QPoint &p)
{
   for(auto &pt:this->points)
   {
      if(abs(p.x()-pt.x())<40.0 && abs(p.y()-pt.y())<40.0)
       {
           return true;
       }
   }
   return false;
}

void visualization::clear()
{
    this->points.clear();
    this->colors.clear();
    this->adjacency_list.clear();
    this->selectedvertex="";
}

void visualization::setStartColor(const QColor &c)
{
  this->startcolor=c;
}

void visualization::fillTargetPoints()
{
    std::mt19937 mt(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution <int> rwidth(this->width()/6+7,this->width()-100);
    std::uniform_int_distribution <int> rheight(10,this->height()-150);
    int i=0;
    const int size=this->adjacency_list.size();
    while(i<size)
     {
            QPoint p(rwidth(mt),rheight(mt));
            while(this->detectcollision(p))
            {
                p.setX(rwidth(mt));
                p.setY(rheight(mt));
            }
            this->points.push_back(p);
            i++;
  }
}

void visualization::setSelectedVertex(QString a_vertex)
{
    this->selectedvertex=a_vertex;
    visualization::selvertex=a_vertex;
}


void visualization::setList(QVector <int> &al)
{
    this->adjacency_list.clear();
   for(auto &x:al)
   {
     this->adjacency_list.push_back(x);
   }
}

void visualization::setColors(QVector<QColor> &cls)
{
    this->colors=cls;
    this->repaint();
}

void visualization::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(Qt::blue);
    QFont f;
    f.setBold(true);
    f.setFamily("calibri");
    f.setPixelSize(21);
    p.setFont(f);
    p.drawRect(0,0,this->width()*0.99,this->height()*0.96);
    this->fillTargetPoints();
    if(this->points.isEmpty()) {return;}
    int pos=-1;
    for(int i=0;i<visualization::vertexnames.size();i++)
    {
        if(visualization::vertexnames.at(i)==selectedvertex)
        {
            pos=i;
        }
    }
    if(pos==-1) {return;}
    p.setPen(this->startcolor);
    p.drawText(7,this->height()/4,this->selectedvertex);
    QPoint startpoint(57,this->height()/4);
    std::list <int>::iterator i=this->adjacency_list.begin();
    int counter=0;
    for(auto &pnts:this->points)
    {
       p.setPen(this->colors[counter]);
       p.drawText(pnts,visualization::vertexnames.at(*i));
       i++;
       counter++;
    }
    QColor linecolor("#d2d9d6");
    p.setPen(linecolor);
    for(auto &pnts:this->points)
    {
       p.drawLine(startpoint,pnts);
    }
    p.end();
}

