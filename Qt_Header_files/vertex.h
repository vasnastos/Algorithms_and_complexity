#ifndef VERTEX_H
#define VERTEX_H
#include <QStringList>
#include "students.h"

class vertex
{
private:
    int id;
    int *connver;
public:
    static int size;
    static int counter;
    vertex();
    ~vertex();
    void add_neibourgh(int jpos);
    void allocmemory();
    QVector <int> get_connected_vertices()const;
    int total_collisions()const;
};

#endif // VERTEX_H
