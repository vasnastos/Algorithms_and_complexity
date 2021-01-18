#ifndef GRAPH_H
#define GRAPH_H
#include <list>
#include "vertex.h"
#include <map>
#include <QVector>

class graph
{
private:
    bool *available;
    int V;
    std::list <int> *adj;
public:
    graph(int v);
    ~graph();
    void free_adjecency();
    void add_connection(int s,int t);
    void export_neibourghs(const QStringList &names);
    std::list <int> list_of_neibourghs(int vrid);
    QVector <int> get_neibourghs(int ver);
    QVector <int> coloring();

    //DSATUR
    std::map <int,int> DSatur();

    //RLF
     size_t numofchecks;
     void swap(int& a, int& b);
     void removeElement(QVector<int>& A, int i);
     void chooseVertex(QVector<int>& X, int& v, int& vPos, QVector<int>& Deg);
     void updateX(QVector<int>& X, QVector<int>& XDeg, int v, int vPos, QVector<int>& Y, QVector<int>& YDeg, QVector< QVector<bool> >& adjacent, QVector<int>& NInY);
     void makeSolution(QVector< QVector<int> >& candSol, QVector<int>& degree,QVector<int>& colNode,QVector< QVector<bool> >& adjacent, int numNodes);
     QVector <int> rlf();


    //BDSATUR
    bool bdsatur();
    void bdsatur_printcoloring();
};

#endif // GRAPH_H
