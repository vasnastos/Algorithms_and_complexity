#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include "graphic.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QMenu>
#include "database.h"
#include <QApplication>
#include <QMenuBar>
#include <QComboBox>
#include <QLabel>
#include <queue>
#include <QTableWidget>
#include <QPushButton>
#include "statistics.h"
#include "graph.h"
#include "visualization.h"
#include <QFileDialog>
#include <stack>
#include <QStackedLayout>

using std::stack;
using std::queue;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QWidget *mw;
    QStackedLayout *lay;
    void make_menu();
    void window1();
    void window2();
    void window3();
    void window4();
    void window5();
    void window6();
    void visual_window();
    QLabel *showic;
    QComboBox *cb;
    QMap <QString,int> res;
    graphic *geffects;
    QLabel *answer,*algorithmlabel;
    QString filename="";
    QString savestring;
    QVector <QColor> colorlabels;
    statistics *s;
    graph *agraph=nullptr;
    visualization *v;
    QVector <int> spots;
    database *mydb;
    QComboBox *algos;
    stack <int> page;
    stack <int> pagef;
    void first_fit_save();
    void DSatur_save();
    void rlf_save();
    void exportNeibourghs();
    QComboBox *algorithmoption;
    bool dsatur_enabled=false;
    bool ffenabled=false;
    bool rlf_enabled=false;
    QTableWidget *tab,*algotable,*specify;
    QString executable_algorithm();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void algoslot(QString al);
    void show_stats();
    void choosefile(QAction *a);
    void home();
    void saveslot();
    void backslot();
    void github();
    void twitter();
    void alcorep();
    void algorithmselector();
    void colors_sum();
    void visualize();
    void finalgraph();
    void quit();
    void frontslot();
    void copyright();
};
#endif // MAINWINDOW_H
