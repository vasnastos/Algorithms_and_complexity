#ifndef DATABASEH_H
#define DATABASEH_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include "stats_info.cpp"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QVariant>

class database
{
private:
    QSqlDatabase db;
public:
    database();
    ~database();
    bool is_in(QString file);
    bool fileloaded(QString f);
    void insertfile(QString file);
    bool exists_in_db(QString fn);
    void insert(const stats &s);
    void insert_sequence(QString filename);
    void update_ff(QString fn,int ff);
    void update_DS(QString fn,int ds);
    void update_rlf(QString fn,int rlf);
    QVector <stats> get_stats();
    QVector <algo> show_algorithm_tracking();
    QMap <QString,int> RLF_RESULTS();
    QMap <QString,int> FIRST_FIT();
    QMap <QString,int> DSATUR_RESULTS();
    QMap <QString,int> BDSATUR_RESULTS();
    QVector <algo> MASSIVE();
    int getNOD(QString file);
};

#endif // DATABASEH_H
