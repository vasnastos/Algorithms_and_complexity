#include <QString>
struct stats
{
    QString fileid;
    int nov;
    int max,min,median;
    double mean;
    double density,cv;
};

struct algo
{
   QString filename;
   int ff;
   int ds;
   int rlf;
   int bds;
};
