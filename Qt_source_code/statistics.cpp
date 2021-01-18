#include "statistics.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>
#include <numeric>
#include <algorithm>
#include <QDebug>
#include <QMessageBox>
#include <cmath>

int sum(int s,const vertex &v)
{
    return s+v.total_collisions();
}

statistics::statistics()
{
   QString message="<html><body>";
   message+="<h4>Το πρόβλημα του χρωματισμού γραφήματος είναι ένα NP‐hard πρόβλημα συνδυαστικής βελ‐"
           "τιστοποίησης.Αφορά την ανάθεση ενός χρώματος σε κάθε κορυφή ενός γραφήματος"
           "έτσι ώστε γειτονικές κορυφές να χρωματίζονται με διαφορετικό χρώμα, ενώ"
           "παράλληλα χρησιμοποιείται ο ελάχιστος αριθμός διαφορετικών χρωμάτων. Στην παρούσα ερ‐"
           "γασία ζητείται η υλοποίηση τεσσάρων αλγορίθμων χρωματισμού γραφημάτων και η εφαρμογή"
           "τους σε γνωστά προβλήματα από τη βιβλιογραφία</h4>";
   message+="</body></html>";
   QMessageBox::information(nullptr,"App Info",message);
}

int statistics::getExamPosition(QString name)
{
    for(int i=0,t=this->exams.size();i<t;i++)
    {
        if(this->exams.at(i)==name)
        {
            return i;
        }
    }
    return -1;
}

statistics::~statistics()
{
    for(auto &x:this->vertices)
    {
        x.allocmemory();
    }
}

void statistics::open_data(QString fn)
{
   vertex::counter=0;
   this->vertices.clear();
   this->students.clear();
   this->exams.clear();
   QFile fp(fn);
   fp.open(QIODevice::ReadOnly);
   QTextStream st(&fp);
   QString line;
   while(!st.atEnd())
   {
     line=st.readLine();
     QStringList data=line.split(" ");
     student s;
     for(auto &k:data)
     {
         if(k.length()==0)
          continue;
         s.add_exam(k);
     }
     if(s.totalexams()==0)
     {
         continue;
     }
     this->students.append(s);
   }
   fp.close();
   this->find_exams();
}
void statistics::find_exams()
{
  for(auto &x:students)
  {
     const int studentexams=x.totalexams();
     for(int l=0;l<studentexams;l++)
     {
       bool found=false;
       for(auto &y:exams)
       {
          if(x.get(l)==y)
          {
              found=true;
              break;
          }
       }
       if(!found)
       {
         exams.append(x.get(l));
       }
     }
  }
  vertex::size=this->exams.size();
  for(int i=0;i<vertex::size;i++)
  {
      vertex x;
      this->vertices.append(x);
  }
}

void statistics::graph_implemantation(graph &g)
{
    QVector <int> connected;
   for(int i=0,size=this->vertices.size();i<size;i++)
   {
     connected=this->vertices[i].get_connected_vertices();
     for(auto &y:connected)
     {
       g.add_connection(i,y);
     }
   }
}

void statistics::fill_vertices()
{
    QStringList enrolls;
    for(auto &x:students)
    {
        enrolls=x.getExams();
        const int s=enrolls.size();
        for(int i=0;i<s;i++)
        {
           for(int j=0;j<s;j++)
           {
               if(i==j) continue;
               int m1=this->getExamPosition(enrolls[i]);
               int m2=this->getExamPosition(enrolls[j]);
               this->vertices[m1].add_neibourgh(m2);
           }
        }
    }
}

double statistics::density()
{
    const int size=this->vertices.size();
  return (double)std::accumulate(this->vertices.begin(),this->vertices.end(),0,sum)/pow(size,2);
}

int statistics::nov()
{
  return this->vertices.size();
}

int statistics::max()
{
    return std::max_element(this->vertices.begin(),this->vertices.end(),[](const vertex &v1,const vertex &v2) {return v1.total_collisions()<v2.total_collisions();})->total_collisions();
}

int statistics::min()
{
   int minfactor=this->vertices[0].total_collisions();
   for(auto &x:this->vertices)
   {
       if(x.total_collisions()<minfactor)
       {
           minfactor=x.total_collisions();
       }
   }
   return minfactor;
}

int statistics::median()
{
  QVector <vertex> sortvertices=this->vertices;
  std::sort(sortvertices.begin(),sortvertices.end(),[](const vertex &v1,const vertex &v2) {return v1.total_collisions()<v2.total_collisions();});
  return sortvertices.at(sortvertices.size()/2).total_collisions();
}

double statistics::mean()
{
   int summary=0;
   for(auto &x:vertices)
   {
     summary+=x.total_collisions();
   }
   return (double)summary/vertex::size;
}

double statistics::cv()
{
  double average=this->mean();
  double percent=0;
  for(auto &x:this->vertices)
  {
    percent+=pow(x.total_collisions()-average,2);
  }

  percent/=(vertex::size-1);
  percent=(sqrt(percent)/average);
  return percent*100.0;
}

QVector <QVector <int>> statistics::neibourghs()const
{
    QVector <QVector <int>> connections;
    QVector <int> v;
    for(const auto &x:this->vertices)
    {
        v=x.get_connected_vertices();
        connections.append(v);
    }
    return connections;
}
