#include "database.h"
#include <QDebug>

database::database()
{
  db=QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("Stats.db");
  db.open();
  QSqlQuery q(db);
  QString sql="create table if not exists statistics(name text,nov integer,density double,max integer,min integer,median integer,mean double,cv double,primary key(name))";
  q.exec(sql);
  sql="create table if not exists algorithms(fn varchar(60),ff integer,ds integer,rlf integer,bds integer,primary key(fn))";
  q.exec(sql);
}
database::~database() {
    db.close();
}

bool database::exists_in_db(QString fn)
{
    QString sql="select * from statistics where name='"+fn+"'";
    QSqlQuery q(db);
    q.exec(sql);
    return q.next();
}

void database::insert(const stats &s)
{
  QString sql="insert into statistics(name,nov,density,max,min,median,mean,cv) values(?,?,?,?,?,?,?,?)";
  QSqlQuery q(db);
  q.prepare(sql);
  if(this->exists_in_db(s.fileid)) {return;}
  q.addBindValue(s.fileid);
  q.addBindValue(s.nov);
  q.addBindValue(s.density);
  q.addBindValue(s.max);
  q.addBindValue(s.min);
  q.addBindValue(s.median);
  q.addBindValue(s.mean);
  q.addBindValue(s.cv);
  q.exec();
}

void database::insert_sequence(QString filename)
{
 if(this->is_in(filename)) {return;}
  QSqlQuery q(db);
  QString sql="insert int algorithms(filename) values(?)";
  q.prepare(sql);
  q.addBindValue(filename);
  q.exec();
}

void database::update_ff(QString fn, int ff)
{
    if(fn.startsWith(":"))
    {
        fn.remove(0,2);
    }
   QSqlQuery q(db);
   QString sql="update algorithms set ff=? where fn=?";
   q.prepare(sql);
   q.addBindValue(ff);
   q.addBindValue(fn);
   q.exec();
}

void database::update_DS(QString fn, int ds)
{
    if(fn.startsWith(":"))
    {
        fn.remove(0,2);
    }
  QSqlQuery q(db);
  QString sql="update algorithms set ds=? where fn=?";
  q.prepare(sql);
  q.addBindValue(ds);
  q.addBindValue(fn);
  q.exec();
}

void database::update_rlf(QString fn, int rlf)
{
    if(fn.startsWith(":"))
    {
        fn.remove(0,2);
    }
  QString sql="update algorithms set rlf=? where fn=?";
  QSqlQuery q(db);
  q.prepare(sql);
  q.addBindValue(rlf);
  q.addBindValue(fn);
  q.exec();
}

bool database::is_in(QString file)
{
   QString sql="select * from algorithms where fn=?";
   QSqlQuery q(db);
   q.prepare(sql);
   q.addBindValue(file);
   q.exec();
   return q.next();
}

QVector <stats> database::get_stats()
{
    QVector <stats> statists;
    QString sql="select * from statistics";
    QSqlQuery q(db);
    q.exec(sql);
    while(q.next())
    {
        stats s;
        s.fileid=q.value(0).toString();
        s.nov=q.value(1).toInt();
        s.density=q.value(2).toDouble();
        s.max=q.value(3).toInt();
        s.min=q.value(4).toInt();
        s.median=q.value(5).toInt();
        s.mean=q.value(6).toDouble();
        s.cv=q.value(7).toDouble();
        s.fileid.remove(0,2);
        statists.append(s);
    }
    return statists;
}

QVector <algo> database::show_algorithm_tracking()
{
    QVector <algo> algostats;
    QString sql="select * from algorithms";
    QSqlQuery q(this->db);
    q.exec(sql);
    while(q.next())
    {
        algo a;
        a.filename=q.value(0).toString();
        a.ff=q.value(1).toInt();
        a.ds=q.value(2).toInt();
        a.rlf=q.value(3).toInt();
        a.bds=q.value(4).toInt();
        algostats.append(a);
    }
    return algostats;
}


 QMap <QString,int> database::RLF_RESULTS()
{
   QMap <QString,int> rlf;
   QString sql="select fn,rlf from algorithms";
   QSqlQuery q(this->db);
   q.exec(sql);
   while(q.next())
   {
        rlf.insert(q.value(0).toString(),q.value(1).toInt());
   }
   return rlf;
}

 QMap <QString,int> database::FIRST_FIT()
 {
   QMap <QString,int> res;
   QString sql="select fn,ff from algorithms";
   QSqlQuery q(this->db);
   q.exec(sql);
   while(q.next())
   {
       res.insert(q.value(0).toString(),q.value(1).toInt());
   }
   return res;
 }
 QMap <QString,int> database::DSATUR_RESULTS()
 {
     QMap <QString,int> res;
     QString sql="select fn,ds from algorithms";
     QSqlQuery q(this->db);
     q.exec(sql);
     while(q.next())
     {
       res.insert(q.value(0).toString(),q.value(1).toInt());
     }
     return res;
 }
 QMap <QString,int> database::BDSATUR_RESULTS()
 {
    QMap <QString,int> res;
    QString  sql="select fn,bds from algorithms";
    QSqlQuery q(db);
    q.exec(sql);
    while(q.next())
    {
       res.insert(q.value(0).toString(),q.value(1).toInt());
    }
    return res;
 }
 QVector <algo> database::MASSIVE()
 {
     QVector <algo> res;
     QString sql="select * from algorithms";
     QSqlQuery q(this->db);
     q.exec(sql);
     while(q.next())
     {
        algo a;
        a.filename=q.value(0).toString();
        a.ff=q.value(1).toInt();
        a.ds=q.value(2).toInt();
        a.rlf=q.value(3).toInt();
        a.bds=q.value(4).toInt();
        res<<a;
     }
     return res;
 }

 int database::getNOD(QString file)
 {
     QString filename;
     if(!file.startsWith(":/"))
    filename=":/"+file;
     else filename=file;
    QString sql="select nov from statistics where name='"+filename+"'";
    QSqlQuery q(this->db);
    q.exec(sql);
    q.next();
    return  q.value(0).toInt();
 }


 bool database::fileloaded(QString f)
 {
   QString sql="select * from algorithms where fn='"+f+"'";
   QSqlQuery q(this->db);
   q.exec(sql);
   return q.next();
 }
 void database::insertfile(QString file)
 {
     if(this->fileloaded(file)) return;
     QString sql="insert into algorithms(fn) values(?)";
     QSqlQuery q(this->db);
     q.prepare(sql);
     q.addBindValue(file);
     q.exec();
 }
