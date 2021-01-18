#ifndef STATISTICS_H
#define STATISTICS_H
#include <QVector>
#include "students.h"
#include "graph.h"
#include "vertex.h"

class statistics
{
private:
    QVector <student> students;
    QVector <vertex> vertices;
public:
    QStringList exams;
    statistics();
    ~statistics();
    void open_data(QString fn);
    void find_exams();
    void fill_vertices();
    int getExamPosition(QString name);
    double density();
    void graph_implemantation(graph &g);
    int nov();
    int max();
    int min();
    int median();
    double mean();
    double cv();
    QVector <QVector <int>> neibourghs()const;
};

#endif // STATISTICS_H
