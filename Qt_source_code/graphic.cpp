#include "graphic.h"
#include <random>

graphic::graphic(int xaxis,int yaxis):source(""),width(xaxis),height(yaxis)
{
}

void graphic::Reset()
{
    this->targets.clear();
}

bool graphic::detectcollision(const QPoint &p)
{
  for(auto &pt:this->targets)
  {
      if(abs(p.x()-pt.x())<35.0 && abs(p.y()-pt.y())<35.0)
      {
            return true;
      }
  }
  return false;
}
void graphic::RandomizeTargetPoints()
{
  std::mt19937 mt(time(nullptr));
  std::uniform_int_distribution <int> rwidth(width/6+10,width-50);
  std::uniform_int_distribution <int> rheight(10,height-100);
  int i=1;
  const int size=this->targs.size();
  while(i<=size)
  {
      QPoint p(rwidth(mt),rheight(mt));
      while(this->detectcollision(p))
      {
          p.setX(rwidth(mt));
          p.setY(rheight(mt));
      }
      this->targets.push_back(p);
      i++;
  }
}

void graphic::setSource(QString s)
{
    this->source=s;
}

void graphic::setTargets(QStringList &targ)
{
    this->Reset();
    this->targs=targ;
    this->repaint();
}

void graphic::paintEvent(QPaintEvent *e)
{
    this->RandomizeTargetPoints();
    QPainter p(this);
    p.setPen(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);
    QFont f;
    f.setWordSpacing(3.0);
    f.setBold(true);
    f.setPixelSize(18);
    p.setFont(f);
    p.drawRect(0,0,width-1,height-1);
    if(this->source!="")
     p.setPen(Qt::darkGreen);
    p.drawText(10,height/4,source);
    int i=0;
    p.setPen(Qt::darkBlue);
    for(auto &y:this->targs)
    {
       p.drawText(this->targets.at(i).x(),this->targets.at(i).y(),y);
       i++;
    }
    p.setPen(Qt::white);
    for(auto &x:this->targets)
    {
        p.drawLine(14*source.size(),height/4,x.x(),x.y());
    }
    p.end();
}
