#include "vertex.h"
#include <algorithm>
#include <QDebug>

int vertex::size=0;
int vertex::counter=0;

vertex::vertex() {
    this->connver=new int[vertex::size];
    for(int i=0;i<vertex::size;i++)
    {
       this->connver[i]=0;
    }
    this->id=vertex::counter;
    vertex::counter++;
}
vertex::~vertex() {}

void vertex::allocmemory()
{
    delete[] this->connver;
}

void vertex::add_neibourgh(int jpos)
{
   this->connver[jpos]=1;
}

QVector <int> vertex::get_connected_vertices()const
{
    QVector <int> vers;
    for(int i=0;i<vertex::size;i++)
    {
        if(connver[i]==1)
        {
            vers.append(i);
        }
    }
    return vers;
}

int vertex::total_collisions()const
{
    return  std::count_if(this->connver,this->connver+vertex::size,[](const int &a) {return a==1;});
}
