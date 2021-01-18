#include "mainwindow.h"
#include <QDebug>
#include <random>
#include <QMovie>
#include <QMessageBox>
#include <QDesktopServices>

//Παραγωγή τυχαίων χρωμάτων
QVector <QColor> color_table(int N)
{
  std::mt19937 mt(time(nullptr));
  std::uniform_int_distribution <int> rgb(0,255);
  QVector <QColor> colors;
  for(int i=0;i<=N;i++)
  {
      QColor c1;
      c1.setRgb(rgb(mt),rgb(mt),rgb(mt));
      colors<<c1;
  }
  return colors;
}

//Εύρεση εκτελέσιμου αλγορίθμου
QString MainWindow::executable_algorithm()
{
    if(this->ffenabled)
    {
       return "First Fit";
    }
    else if(this->dsatur_enabled)
    {
       return "DSatur";
    }
    else if(this->rlf_enabled)
    {
       return "RLF";
    }
    else
    {
        return "Backtracking DSatur";
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(900,800);
    this->setWindowTitle("Algorithms execution");
    //this->setWindowIcon(QIcon());
    mw=new QWidget;
    mw->setFixedSize(this->width(),this->height());
    this->setCentralWidget(mw);
    lay=new QStackedLayout;
    mw->setLayout(lay);
    this->mydb=new database;
    this->make_menu();
    this->window1();
    this->window2();
    this->window3();
    this->window4();
    this->window5();
    this->window6();
    this->visual_window();
    this->s=new statistics;
}

//Κατασκευή μενού
void MainWindow::make_menu()
{
    QMenu *filemenu=new QMenu("FILES");
    QFile fp(":/files.txt");
    fp.open(QIODevice::ReadOnly);
    QTextStream stream(&fp);
    QString line;
    while(!stream.atEnd())
    {
        line=stream.readLine();
        filemenu->addAction(QIcon(":/file.png"),line);
        mydb->insert_sequence(line);
        mydb->insertfile(line);
    };
    QAction *a=new QAction(QIcon(":/home.jpg"),"");
    QAction *b=new QAction(QIcon(":/save.png"),"");
    QAction *c=new QAction(QIcon(":/back.png"),"");
    QAction *d=new QAction(QIcon(":/front.png"),"");
    QAction *e=new QAction(QIcon(":/colors.png"),"");
    QAction *f=new QAction(QIcon(":/quit.png"),"");
    QAction *cop=new QAction(QIcon(":/copyright.png"),"");
    menuBar()->addMenu(filemenu);
    menuBar()->addAction(cop);
    menuBar()->addAction(c);
    menuBar()->addAction(a);
    menuBar()->addAction(d);
    menuBar()->addAction(b);
    menuBar()->addAction(e);
    menuBar()->addAction(f);
    menuBar()->setStyleSheet("color:blue; font-weight:bold;");
    connect(filemenu,SIGNAL(triggered(QAction *)),this,SLOT(choosefile(QAction *)));
    connect(a,SIGNAL(triggered()),this,SLOT(home()));
    connect(b,SIGNAL(triggered()),this,SLOT(saveslot()));
    connect(c,SIGNAL(triggered()),this,SLOT(backslot()));
    connect(d,SIGNAL(triggered()),this,SLOT(frontslot()));
    connect(e,SIGNAL(triggered()),this,SLOT(colors_sum()));
    connect(f,SIGNAL(triggered()),this,SLOT(quit()));
    connect(cop,SIGNAL(triggered()),this,SLOT(copyright()));
}

MainWindow::~MainWindow()
{
    delete this->s;
    delete this->mydb;
    agraph->free_adjecency();
    delete this->agraph;
    delete this->v;
    delete this->geffects;
}

//Κατασκευή παραθύρου 1(Κεντρικού παραθύρου εφαρμογής)
void MainWindow::window1()
{
   QWidget *w=new QWidget;
   w->setFixedSize(this->width(),this->height());
   lay->addWidget(w);
   QVBoxLayout *ml=new QVBoxLayout;
   w->setLayout(ml);
   ml->setSpacing(0);
   QLabel *central=new QLabel;
   central->setFixedSize(0.98*this->width(),0.4*this->height());
   central->setStyleSheet("border:2px solid; margin-bottom:6px;");
   QMovie *m=new QMovie();
   m->setFileName(":/graph.gif");
   m->setScaledSize(QSize(central->width(),central->height()));
   central->setMovie(m);
   central->movie()->start();
   ml->addWidget(central);
   QLabel *lb=new QLabel;
   lb->setFixedSize(0.25*this->width(),0.04*this->height());
   lb->setStyleSheet("background-color:#d1d0c2;"
                     "font-size:18px;"
                     "color:#85132f;");
   lb->setAlignment(Qt::AlignCenter);
   lb->setText("Select Algorithm");
   QComboBox *bx=new QComboBox();
   bx->setFixedSize(0.3*this->width(),0.04*this->height());
   QStringList algs;
   algs<<"Statistics"<<"first fit"<<"DSatur"<<"RLF"<<"Backtracking DSatur"<<"Neibourghs"<<"Massive Solution";
   bx->addItems(algs);
   bx->setStyleSheet("border:1px solid; font-size:18px; text-align:center; color:blue;");
   QPushButton *button=new QPushButton("STATS");
   button->setFixedSize(0.12*this->width(),0.04*this->height());
   button->setStyleSheet("QPushButton::hover{background-color:lightgreen; color:red;} QPushButton::color {background-color:#d1d0c2; color:#85132f; font-size:28px;}");
   QHBoxLayout *row=new QHBoxLayout;
   row->setSpacing(2);
   ml->addLayout(row);
   row->addWidget(lb);
   row->addWidget(bx);
   row->addWidget(button);
   connect(bx,SIGNAL(activated(QString)),this,SLOT(algoslot(QString)));
   connect(button,SIGNAL(clicked(bool)),this,SLOT(show_stats()));
   QLabel *label=new QLabel;
   label->setFixedSize(0.98*this->width(),0.23*this->height());
   QPushButton *but=new QPushButton;
   but->setFixedWidth(0.1*this->width());
   but->setFixedHeight(0.06*this->height());
   QSize butfixed(but->width(),but->height());
   but->setStyleSheet("border:null; border:collapse;");
   but->setIconSize(butfixed);
   but->setIcon(QIcon(":/github.png"));
   QPushButton *but1=new QPushButton;
   but1->setFixedWidth(0.06*this->width());
   but1->setFixedHeight(0.06*this->height());
   QSize twittersize(but1->width(),but1->height());
   but1->setIconSize(twittersize);
   but1->setStyleSheet("border:null;");
   but1->setIcon(QIcon(":/twitter.png"));
   QPushButton *but2=new QPushButton;
   but2->setFixedSize(0.06*this->width(),0.06*this->height());
   QSize alcrep(but2->width(),but2->height());
   but2->setIconSize(alcrep);
   but2->setIcon(QIcon(":/repository.png"));
   but2->setStyleSheet("border:null;");
   but2->setToolTip("Algorithms and Complexity repository");
   QLabel *infolabel=new QLabel;
   infolabel->setFixedSize(0.25*this->width(),0.1*this->height());
   infolabel->setStyleSheet("border:null; border:collapse; color:#73142e;");
   infolabel->setAlignment(Qt::AlignCenter);
   infolabel->setText("<h4>More Information<h4>");
   QLabel *hr=new QLabel;
   hr->setFixedSize(0.98*this->width(),0.05*this->height());
   hr->setText("<hr>");
   ml->addWidget(hr);
   QHBoxLayout *row1=new QHBoxLayout;
   ml->addLayout(row1);
   row1->addWidget(infolabel);
   row1->addWidget(but);
   row1->addWidget(but1);
   row1->addWidget(but2);
   connect(but,SIGNAL(clicked(bool)),this,SLOT(github()));
   connect(but1,SIGNAL(clicked(bool)),this,SLOT(twitter()));
   connect(but2,SIGNAL(clicked(bool)),this,SLOT(alcorep()));
}

//Κατασκευή παραθύρου 2(Εμφάνιση στατιστικών)
void MainWindow::window2()
{
  QWidget *w=new QWidget;
  w->setFixedSize(this->width(),this->height());
  lay->addWidget(w);
  QVBoxLayout *ml=new QVBoxLayout;
  w->setLayout(ml);
  showic=new QLabel;
  showic->setFixedSize(0.98*this->width(),0.46*this->height());
  showic->setStyleSheet("border:colllapse;");
  QMovie *statsg=new QMovie;
  statsg->setFileName(":/stats.gif");
  QSize s(showic->width(),showic->height());
  statsg->setScaledSize(s);
  showic->setMovie(statsg);
  showic->movie()->start();
  ml->addWidget(showic);
  answer=new QLabel;
  answer->setFixedSize(0.98*this->width(),0.4*this->height());
  answer->setStyleSheet("border:2px solid;"
                        "font-size:17px;"
                        "color:blue;"
                        "font-weight:bold;");
  answer->setAlignment(Qt::AlignCenter);
  ml->addWidget(answer);
}

//Κατασκευή παραθύρου 3(Προβολή απότελεσμάτων αλγορίθμων)
void MainWindow::window3()
{
  QWidget *w=new QWidget;
  w->setFixedSize(this->width(),this->height());
  lay->addWidget(w);
  QVBoxLayout *ml=new QVBoxLayout;
  w->setLayout(ml);
  algorithmlabel=new QLabel;
  algorithmlabel->setFixedSize(0.98*this->width(),0.2*this->height());
  algorithmlabel->setAlignment(Qt::AlignCenter);
  algorithmlabel->setStyleSheet("font-family:calibri; font-weight:bold; border:null; border:collapse; background-color:#94919e; color:#52040c;");
  ml->addWidget(algorithmlabel);
  algotable=new QTableWidget;
  algotable->setFixedSize(0.98*this->width(),0.57*this->height());
  QStringList header;
  header<<"Vertex"<<"Color Number"<<"Color"<<"Graph";
  algotable->setColumnCount(header.size());
  algotable->setStyleSheet("font-size:21px; color:red;");
  algotable->setColumnWidth(0,algotable->width()*0.98/header.size());
  algotable->setColumnWidth(1,algotable->width()*0.98/header.size());
  algotable->setColumnWidth(2,algotable->width()*0.98/header.size());
  algotable->setColumnWidth(3,algotable->width()*0.98/header.size());
  algotable->setHorizontalHeaderLabels(header);
  ml->addWidget(algotable);
}

//Επιστροφή στο αρχικό μενου.
void MainWindow::home()
{
    pagef.push(lay->currentIndex());
    lay->setCurrentIndex(0);
}

//Κατασκευή παραθύρου 4(Εύρεση στατιστικών.Πατώντας το κουμπί stats της αρχικής σελίδας)
void MainWindow::window4()
{
  QWidget *w=new QWidget;
  w->setFixedSize(this->width(),this->height());
  lay->addWidget(w);
  QVBoxLayout *ml=new QVBoxLayout;
  w->setLayout(ml);
  QLabel *infolabel=new QLabel;
  infolabel->setFixedSize(0.98*this->width(),0.12*this->height());
  infolabel->setStyleSheet("background-color:#d1d0c2; color:#73142e; border:2px solid; font-size:30px;");
  infolabel->setAlignment(Qt::AlignCenter);
  infolabel->setText("General Statistics");
   ml->addWidget(infolabel);
   tab=new QTableWidget;
   tab->setFixedSize(0.98*this->width(),0.7*this->height());
   tab->setStyleSheet("font-size:20px; color:red;");
   QStringList headers;
   headers<<"File"<<"Vertices"<<"Density"<<"Min"<<"Median"<<"Max"<<"Mean"<<"Cv";
   tab->setColumnCount(headers.size());
   tab->setHorizontalHeaderLabels(headers);
   for(int i=0;i<headers.size();i++)
   {
       tab->setColumnWidth(i,0.98*tab->width()/headers.size());
   }
   ml->addWidget(tab);
}

//Κατασκευή παραθύρου 5(Αναπαράσταση συνδέσεων μεταξύ κορυφών)
void MainWindow::window5()
{
    QWidget *w=new QWidget;
    w->setFixedSize(this->width(),this->height());
    lay->addWidget(w);
    QVBoxLayout *wl=new QVBoxLayout;
    w->setLayout(wl);
    QLabel *lb=new QLabel;
    lb->setFixedSize(0.2*this->width(),0.05*this->height());
    lb->setStyleSheet("background-color:#baa4b6; color:blue; font-weight:bold; font-family:calibri; font-size:17px;");
    lb->setAlignment(Qt::AlignCenter);
    lb->setText("Choose Vertex");
    cb=new QComboBox;
    cb->setStyleSheet("border:2px solid; text-align:center; color:red; font-size:18px;");
    cb->setFixedSize(0.1*this->width(),0.04*this->height());
    QPushButton *b=new QPushButton("Visualize");
    b->setFixedSize(0.12*this->width(),0.04*this->height());
    b->setStyleSheet("background-color:#b0a0a4; color:#030d3d; font-size:19px;");
    QHBoxLayout *la=new QHBoxLayout();
    wl->addLayout(la);
    la->addWidget(lb);
    la->addWidget(cb);
    la->addWidget(b);
    connect(b,SIGNAL(clicked(bool)),this,SLOT(visualize()));
    geffects=new graphic(0.98*this->width(),0.87*this->height());
    wl->addWidget(geffects);
}

//SLOT  για την ενέργεια που θα πραγματοποιηθεί με βάση την επιλογή του αλγορίθμου
void MainWindow::algoslot(QString al)
{
  if(al=="Massive Solution")
  {
        page.push(lay->currentIndex());
       lay->setCurrentIndex(5);
       pagef.push(lay->currentIndex());
  }
  else if(filename.isEmpty())
  {
      QMessageBox::information(this,"Info","Go to <p style=\"color:blue;\">FILES Menu and select one of the available files</p>");
      return;
  }
  else if(al=="Statistics")
  {
      page.push(lay->currentIndex());
    lay->setCurrentIndex(1);
    pagef.push(lay->currentIndex());
    QString filecode=filename;
    filecode.remove(0,2);
    QString message="<h2>Statistics for file:"+filecode+"<h2><ul><hr>";
    savestring="<html><body style=\"background-color:cyan;\"><center><h2>Statistics for File:"+filename+"</h2><hr><table border=\"3\" style=\"background-color:#ebf0a3; color:blue; width=60%; height:auto; font-family:calibri;\">";
    savestring+="<tr><th>SEARCH FOR</td><td>VALUE</th></tr>";
    stats st;
    st.fileid=filename;
    st.nov=s->nov();
    st.max=s->max();
    st.min=s->min();
    st.mean=s->mean();
    st.median=s->median();
    st.density=s->density();
    st.cv=s->cv();
    message+="<li>Exams:"+QString::number(st.nov)+"</li>";
    savestring+="<tr><td>EXAMS</td><td>"+QString::number(st.nov)+"</td></tr>";
    message+="<li>Density:"+QString::number(st.density,'f',3)+"</li>";
    savestring+="<tr><td>DENSITY</td><td>"+QString::number(st.density)+"</td></tr>";
    message+="<li>Max:"+QString::number(st.max,'f',3)+"</li>";
    savestring+="<tr><td>MAX</td><td>"+QString::number(st.max)+"</td></tr>";
    message+="<li>Min:"+QString::number(st.min,'f',3)+"</li>";
    savestring+="<tr><td>MIN</td><td>"+QString::number(st.min)+"</td></tr>";
    message+="<li>Median:"+QString::number(st.median,'f',3)+"</li>";
    savestring+="<tr><td>MEDIAN</td><td>"+QString::number(st.median)+"</td></tr>";
    message+="<li>Mean:"+QString::number(st.mean,'f',3)+"</li>";
    savestring+="<tr><td>MEAN</td><td>"+QString::number(st.mean)+"</td></tr>";
    message+="<li>Cv:"+QString::number(st.cv,'f',3)+"</li>";
    savestring+="<tr><td>COEFFICIENCE OF VARIATION</td><td>"+QString::number(st.cv)+"</td></td>";
    message+="</ul>";
    mydb->insert(st);
    answer->setText(message);
    savestring+="</table></center></body></html>";
  }
  else if(al=="first fit")
  {
      dsatur_enabled=false;
      ffenabled=true;
      rlf_enabled=false;
      page.push(lay->currentIndex());
      QString message="<h2>Algorithm:First Fit</h2><h2>File:"+this->filename+"</h2><h2>Vertices:"+QString::number(mydb->getNOD(this->filename))+"</h2>";
      algorithmlabel->setText(message);
      if(vertex::size==0)
      {
          QMessageBox::critical(this,"Error","No input data detected!!!!");
          return;
      }
      lay->setCurrentIndex(2);
      pagef.push(lay->currentIndex());
      spots=agraph->coloring();
      int maximumnumber=*std::max_element(spots.begin(),spots.end());
      colorlabels=color_table(maximumnumber);
      mydb->update_ff(this->filename,maximumnumber+1);
      algotable->setRowCount(spots.size());
      for(int i=0;i<algotable->rowCount();i++)
      {
         QTableWidgetItem *it1=new QTableWidgetItem();
         it1->setTextAlignment(Qt::AlignCenter);
         QColor c1;
         c1.setNamedColor("#0d112b");
         it1->setForeground(c1);
         it1->setText(s->exams[i]);
         algotable->setItem(i,0,it1);
         QTableWidgetItem *it2=new QTableWidgetItem;
         it2->setTextAlignment(Qt::AlignCenter);
         it2->setForeground(c1);
         it2->setText(QString::number(spots[i]));
         algotable->setItem(i,1,it2);
         QTableWidgetItem *it3=new QTableWidgetItem("");
         it3->setBackground(colorlabels.at(spots[i]));
         algotable->setItem(i,2,it3);
         QPushButton *b=new QPushButton("Visualize");
         b->setProperty("vertex",s->exams[i]);
         b->setStyleSheet("background-color:#cddefa; color:#36030a; font-family:calibri; font-size:16px; font-weight:bold;");
         connect(b,SIGNAL(clicked(bool)),this,SLOT(finalgraph()));
         algotable->setCellWidget(i,3,b);
      }
  }
  else if(al=="Neibourghs")
  {
      page.push(lay->currentIndex());
    if(vertex::size==0)
    {
        QMessageBox::critical(this,"Error","No input vertices detected on graph,Please select a file");
        return;
    }
    lay->setCurrentIndex(4);
  }
  else if(al=="DSatur")
  {
      dsatur_enabled=true;
      ffenabled=false;
      rlf_enabled=false;
      page.push(lay->currentIndex());
      lay->setCurrentIndex(2);
      pagef.push(lay->currentIndex());
      QString message="<h2>Algorithm:DSATUR</h2><h2>File:"+this->filename+"</h2><h2>Vertices:"+QString::number(mydb->getNOD(this->filename))+"</h2>";
      algorithmlabel->setText(message);
      std::map <int,int> dsaturalgo=agraph->DSatur();
      int max=-1;
      for(auto i=dsaturalgo.begin();i!=dsaturalgo.end();i++)
      {
        if(i->second>max)
        {
           max=i->second;
        }
      }
      //Το vector spots θα χρησιμοποιηθεί ώστε να μπορέσω να αναπαραστήσω τον τελικό χρωματισμό.
      //Άρα θα εισάγω τα δεδομένα απο το map στο vector
      mydb->update_DS(this->filename,max+1);
      algotable->setRowCount(s->exams.size());
      int maxcolornum=0;
      this->spots.clear();
      const int size=dsaturalgo.size();
      for(int i=0;i<size;i++)
      {
         this->spots.push_back(-1);
      }
      for(std::map <int,int>::iterator i=dsaturalgo.begin();i!=dsaturalgo.end();i++)
      {
          if(i->second>maxcolornum)
          {
              maxcolornum=i->second;
          }
          this->spots[i->first]=i->second;
      }
      colorlabels=color_table(maxcolornum);
      for(int i=0;i<algotable->rowCount();i++)
      {
         QTableWidgetItem *it1=new QTableWidgetItem();
         it1->setTextAlignment(Qt::AlignCenter);
         QColor c1;
         c1.setNamedColor("#0d112b");
         it1->setForeground(c1);
         it1->setText(s->exams[i]);
         algotable->setItem(i,0,it1);
         QTableWidgetItem *it2=new QTableWidgetItem;
         it2->setTextAlignment(Qt::AlignCenter);
         it2->setForeground(c1);
         it2->setText(QString::number(dsaturalgo[i+1]));
         algotable->setItem(i,1,it2);
         QTableWidgetItem *it3=new QTableWidgetItem("");
         it3->setBackground(colorlabels.at(dsaturalgo[i+1]));
         algotable->setItem(i,2,it3);
         QPushButton *b=new QPushButton("Visualize");
         b->setProperty("vertex",s->exams[i]);
         b->setStyleSheet("background-color:#cddefa; color:#36030a; font-family:calibri; font-size:16px; font-weight:bold;");
         connect(b,SIGNAL(clicked(bool)),this,SLOT(finalgraph()));
         algotable->setCellWidget(i,3,b);
      }
  }
  else if(al=="RLF")
  {
      page.push(lay->currentIndex());
      dsatur_enabled=false;
      ffenabled=false;
      rlf_enabled=true;
      lay->setCurrentIndex(2);
      pagef.push(lay->currentIndex());
      QVector <int> rlfresults=this->agraph->rlf();
      this->spots=rlfresults;
      algotable->setRowCount(rlfresults.size());
      QString mesg="<h2>Algorithm:RLF</h2><h2>File:"+this->filename+"</h2><h2>Vertices:"+QString::number(mydb->getNOD(this->filename))+"</h2>";
      algorithmlabel->setText(mesg);
      colorlabels=color_table(*std::max_element(rlfresults.begin(),rlfresults.end()));
      for(int i=0,t=algotable->rowCount();i<t;i++)
      {
         QTableWidgetItem *it1=new QTableWidgetItem();
         QColor c1("#0d112b");
         it1->setForeground(c1);
         it1->setTextAlignment(Qt::AlignCenter);
         it1->setText(s->exams[i]);
         algotable->setItem(i,0,it1);
         QTableWidgetItem *it2=new QTableWidgetItem();
         it2->setForeground(c1);
         it2->setTextAlignment(Qt::AlignCenter);
         it2->setText(QString::number(rlfresults[i]));
         algotable->setItem(i,1,it2);
         QTableWidgetItem *it3=new QTableWidgetItem();
         it3->setTextAlignment(Qt::AlignCenter);
         it3->setBackground(colorlabels[rlfresults[i]]);
         algotable->setItem(i,2,it3);
         QPushButton *b=new QPushButton("Visualize");
         b->setProperty("vertex",s->exams[i]);
         b->setStyleSheet("background-color:#cddefa; color:#36030a; font-family:calibri; font-size:16px; font-weight:bold;");
         connect(b,SIGNAL(clicked(bool)),this,SLOT(finalgraph()));
         algotable->setCellWidget(i,3,b);
      }
      mydb->update_rlf(this->filename,*std::max_element(rlfresults.begin(),rlfresults.end())+1);
  }
  else
  {
      QMessageBox::information(this,"Info","<h3>Algorithms implemantation page under construction");
  }
}

//Εμφάνιση αθροίσματος χρωμάτων
void MainWindow::colors_sum()
{
    if(lay->currentIndex()!=2) {return;}
    if(dsatur_enabled)
    {
       QMessageBox::information(this,"DSatur","<h2>Total colors used:"+QString::number(colorlabels.size())+"</h2>");
    }
    else if(ffenabled)
    {
       QMessageBox::information(this,"First Fit","<h2>Total colors used:"+QString::number(colorlabels.size()));
    }
    else if(rlf_enabled)
    {
        QVector <int> rlf=this->agraph->rlf();
        QMessageBox::information(this,"RLF","Total COLORS USED:"+QString::number(*std::max_element(rlf.begin(),rlf.end()))+1);
    }
}

//Ανοιγμα external web pages
void MainWindow::github()
{
     QDesktopServices::openUrl(QUrl("https://github.com/vasnastos", QUrl::TolerantMode));
}

void MainWindow::twitter()
{
    QDesktopServices::openUrl(QUrl("https://twitter.com/NastosVasileios",QUrl::TolerantMode));
}

void MainWindow::alcorep()
{
    QDesktopServices::openUrl(QUrl("https://vasnastos.github.io/Algorithms_and_complexity/",QUrl::TolerantMode));
}


//Επιλογή αρχείων από το μενού file
void MainWindow::choosefile(QAction *a)
{
    filename=":/"+a->text();
    s->open_data(filename);
    s->fill_vertices();
    if(vertex::size==0)
    {
        QMessageBox::critical(this,"Error","No input data detected!!!!");
        return;
    }
    agraph=new graph(vertex::size);
    s->graph_implemantation(*agraph);
    QVector <QVector <int>> adj=s->neibourghs();
    //Fill with vertices the selector fro the neibourgh visualization window
    cb->clear();
    cb->addItems(this->s->exams);
    visualization::vertexnames=s->exams;
}

void MainWindow::saveslot()
{
   if(lay->currentIndex()==1)
   {
       QString fn=filename;
       fn.remove(0,2);
       QString savingurl=QDir::currentPath()+"//statistics_"+fn+".html";
       QFile fp(savingurl);
       fp.open(QIODevice::WriteOnly);
       QTextStream  stream(&fp);
       stream<<this->savestring;
       fp.close();
       int option=QMessageBox::information(this,"Open Statistics","<h4 style=\"color:blue;\">File Save as:"+savingurl+".Do you want to open it?</h4>",QMessageBox::Yes,QMessageBox::No);
       if(option==QMessageBox::Yes)
       QDesktopServices::openUrl(QUrl(savingurl,QUrl::TolerantMode));
   }
   //first fit save to file info
   else if(lay->currentIndex()==2 && this->ffenabled)
   {
       this->first_fit_save();
   }
   //DSatur save to file info
   else if(lay->currentIndex()==2 && this->dsatur_enabled)
   {
       this->DSatur_save();
   }
   //rlf save to file info
   else if(lay->currentIndex()==2 &&this->rlf_enabled)
   {
       this->rlf_save();
   }
   else if(lay->currentIndex()==3)
   {
       QString fn=QFileDialog::getSaveFileName(this,"Save Stats",".","image files (*.png)");
       if(fn.isEmpty()) return;
       tab->grab().save(fn);
   }
   else if(lay->currentIndex()==4)
   {
       QString sample=this->filename;
       sample.remove(0,2);
       QString fn=QDir::currentPath()+"//"+sample+"_Neiborghs.html";
       QFile fp(fn);
       fp.open(QIODevice::WriteOnly);
       QTextStream st(&fp);
       QString inf="<html><body><center><div style=\"text-align:center; width:30%; overflow:hidden; border:2px solid; background-color:cyan; color:red; font-size:21px; font-weight:bold;\">Nastos Vasileios 2021<br>Neibourghs for filename "+this->filename+"<br>Total Vertices:"+QString::number(this->s->exams.size())+"</div>";
       inf+="<br><br><table border=\"3\" style=\"text-align:center; font-size:18px; font-weight:italics; font-color:blue; background-color:lightgray;\"><tr style=\"background-color:white; color:#330c1e;\">";
       inf+="<th>EXAM</th><th>COLLIDED EXAMS</th><tr>";
       QString nbs="";
       for(int i=0,t=this->s->exams.size();i<t;i++)
       {
            nbs="";
            inf+="<tr><td>"+s->exams[i]+"</td>";
            for(auto &x:this->agraph->get_neibourghs(i))
            {
                nbs+=s->exams[x]+"-";
            }
            nbs.remove(nbs.length()-1,1);
            inf+="<td><b>"+nbs+"</b></td></tr>";
       }
       inf+="</table></center></body></html>";
       st<<inf<<endl;
       fp.close();
       int op=QMessageBox::information(this,"Html Source","<h3 style=\"color:blue;\">File Saved as "+fn+"</h3>",QMessageBox::Yes,QMessageBox::No);
       if(op==QMessageBox::Yes)
       QDesktopServices::openUrl(QUrl(fn,QUrl::TolerantMode));
   }
   else if(lay->currentIndex()==5)
   {
       //Going into a specification of process from the database
       //---------------------------------------------------------
       //Export the board in a png format
       QString fn=QFileDialog::getSaveFileName(this,"Board Image",".","image file (*.png)");
       if(fn.isEmpty())
       {
           QMessageBox::critical(this,"Error","You did not select a file");
           return;
       }
       specify->grab().save(fn);
   }
   else
   {
       qDebug()<<"No register option";
   }
}

void MainWindow::rlf_save()
{
   QString fn=QFileDialog::getSaveFileName(this,"Save RlF RESULTS",".","Comma seperated value file(*.csv)");
   if(fn.isEmpty())
   {
       QMessageBox::critical(this,"ERROR","<h3 style=\"color:red;\">No file Selected");
       return;
   }
   QFile fp(fn);
   fp.open(QIODevice::WriteOnly);
   QTextStream stream(&fp);
   QString date=__DATE__;
   QString time=__TIME__;
   QString copyright="©Nastos Vasileios 2021\n";
   QString message="Algorithms and Complexity Course!!\nAll Rights Reservered\n";
   stream<<copyright<<message;
   stream<<"File Created at"<<date<<"-"<<time<<endl;
   stream<<"File used:"<<this->filename<<endl;
   stream<<"Total Exams:"<<this->mydb->getNOD(this->filename)<<endl;
   stream<<"Colors used:"<<(*std::max_element(this->spots.begin(),this->spots.end())+1)<<endl;
   stream<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl<<endl;
   stream<<"VERTEX;COLOR_NUMBER"<<endl;
   int pos=0;
   for(auto &x:this->spots)
   {
      stream<<this->s->exams[pos++]<<";"<<x<<endl;
   }
   fp.close();
}


void MainWindow::show_stats()
{
    page.push(lay->currentIndex());
  lay->setCurrentIndex(3);
  QVector <stats> statisticsvec=mydb->get_stats();
  if(statisticsvec.size()==0)
  {
      QMessageBox::critical(this,"Error","No input data detected in the database");
      return;
  }
  tab->setRowCount(statisticsvec.size());
  for(int i=0;i<tab->rowCount();i++)
  {
      QTableWidgetItem *it1=new QTableWidgetItem;
      it1->setTextAlignment(Qt::AlignCenter);
      it1->setForeground(Qt::blue);
      QString filecode=statisticsvec.at(i).fileid;
      it1->setText(filecode);
      tab->setItem(i,0,it1);
      QTableWidgetItem *it2=new QTableWidgetItem;
      it2->setTextAlignment(Qt::AlignCenter);
      it2->setForeground(Qt::blue);
      it2->setText(QString::number(statisticsvec.at(i).nov));
      tab->setItem(i,1,it2);
      QTableWidgetItem *it3=new QTableWidgetItem;
      it3->setTextAlignment(Qt::AlignCenter);
      it3->setForeground(Qt::blue);
      it3->setText(QString::number(statisticsvec.at(i).density,'f',3));
      tab->setItem(i,2,it3);
      QTableWidgetItem *it4=new QTableWidgetItem;
      it4->setTextAlignment(Qt::AlignCenter);
      it4->setForeground(Qt::blue);
      it4->setText(QString::number(statisticsvec.at(i).min,'f',3));
      tab->setItem(i,3,it4);
      QTableWidgetItem *it5=new QTableWidgetItem;
      it5->setTextAlignment(Qt::AlignCenter);
      it5->setForeground(Qt::blue);
      it5->setText(QString::number(statisticsvec.at(i).median));
      tab->setItem(i,4,it5);
      QTableWidgetItem *it6=new QTableWidgetItem;
      it6->setTextAlignment(Qt::AlignCenter);
      it6->setForeground(Qt::blue);
      it6->setText(QString::number(statisticsvec.at(i).max));
      tab->setItem(i,5,it6);
      QTableWidgetItem *it7=new QTableWidgetItem;
      it7->setTextAlignment(Qt::AlignCenter);
      it7->setForeground(Qt::blue);
      it7->setText(QString::number(statisticsvec.at(i).mean,'f',3));
      tab->setItem(i,6,it7);
      QTableWidgetItem *it8=new QTableWidgetItem;
      it8->setTextAlignment(Qt::AlignCenter);
      it8->setForeground(Qt::blue);
      it8->setText(QString::number(statisticsvec.at(i).cv,'f',3));
      tab->setItem(i,7,it8);
  }
}

void MainWindow::backslot()
{
    int previous;
    if(!page.empty())
   {
        previous=page.top();
        page.pop();
        pagef.push(lay->currentIndex());
    }
    else
    {
        previous=0;
        pagef.push(lay->currentIndex());
    }
   lay->setCurrentIndex(previous);
}

void MainWindow::frontslot()
{
    int next=0;
    int current=lay->currentIndex();
    if(!pagef.empty())
    {
        next=pagef.top();
        pagef.pop();
        page.push(current);
    }
    else
    {
        lay->setCurrentIndex(0);
        return;
    }
    lay->setCurrentIndex(next);
}

void MainWindow::first_fit_save()
{
    QString fn=QFileDialog::getSaveFileName(this,"Save first fit results",".","comma seperated values file (*.csv)");
    if(fn.length()==0)
    {
        QMessageBox::critical(this,"Error","No file selected");
        return;
    }
    QFile fp(fn);
    fp.open(QIODevice::WriteOnly);
    QTextStream st(&fp);
    const int sizespot=spots.size();
    QString date=__DATE__;
    QString time=__TIME__;
    QString copyright="©Nastos Vasileios 2021\n";
    QString message="Algorithms and Complexity Course!!\nAll Rights Reservered\n";
    st<<copyright<<message;
    st<<"File Created at"<<date<<"-"<<time<<endl;
    st<<"File used:"<<this->filename<<endl;
    st<<"Total Exams:"<<this->mydb->getNOD(this->filename)<<endl;
    st<<"Colors used:"<<(*std::max_element(this->spots.begin(),this->spots.end())+1)<<endl;
    st<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
    st<<"VERTEX;COLOR_NUMBER"<<endl;
    for(int i=0;i<sizespot;i++)
    {
        st<<s->exams[i]<<";"<<spots[i]<<endl;
    }
    fp.close();
}

void MainWindow::DSatur_save()
{
   QString fn=QFileDialog::getSaveFileName(this,"Save DSatur Results",".","comma seperated value file (*.csv))");
   if(fn.isEmpty())
   {
       QMessageBox::critical(this,"ERROR","No file selected");
       return;
   }
   QFile fp(fn);
   fp.open(QIODevice::WriteOnly);
   QTextStream stream(&fp);
   QString date=__DATE__;
   QString time=__TIME__;
   QString copyright="©Nastos Vasileios 2021\n";
   QString message="Algorithms and Complexity Course!!\nAll Rights Reservered\n";
   stream<<copyright<<message;
   stream<<"File Created at"<<date<<"-"<<time<<endl;
   stream<<"File used:"<<this->filename<<endl;
   stream<<"Total Exams:"<<this->mydb->getNOD(this->filename)<<endl;
   stream<<"Colors used:"<<(*std::max_element(this->spots.begin(),this->spots.end())+1)<<endl;
   stream<<"\t VERTEX\tCOLOR_NUMBER"<<endl;
   stream<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl<<endl;
   stream<<"EXAM;COLOR"<<endl;
   for(int j=0,t=s->exams.size();j<t;j++)
   {
       stream<<s->exams[j]<<";"<<spots[j]<<endl;
   }
   fp.close();
}

//Εμφάνιση γειτόνων
void MainWindow::exportNeibourghs()
{
    agraph->export_neibourghs(s->exams);
}

void MainWindow::window6()
{
    QWidget *w=new QWidget;
    w->setFixedSize(width(),height());
    lay->addWidget(w);
    QVBoxLayout *l=new QVBoxLayout;
    w->setLayout(l);
    //---------------------------------------------------------------------
    //algorithm selector
    l->setMargin(2);
    l->setSpacing(2);
    QHBoxLayout *hl=new QHBoxLayout();
    hl->setSpacing(10);
    algos=new QComboBox;
    algos->addItem("FIRST FIT");
    algos->addItem("DSATUR");
    algos->addItem("RLF");
    algos->addItem("BDSATUR");
    algos->addItem("MASSIVE");
    algos->setFixedWidth(0.3*this->width());
    algos->setFixedHeight(0.04*this->height());
    algos->setStyleSheet("border:2px solid; color:red; text-align:center; font-size:15px;");
    QLabel *lb=new QLabel;
    lb->setFixedSize(0.4*this->width(),0.04*this->height());
    lb->setAlignment(Qt::AlignCenter);
    lb->setText("ALGORITHM SELECTION");
    lb->setStyleSheet("background-color:#b3adb2; border:collapse; font-size:21px; color:#0c065e;");
    QPushButton *abutton=new QPushButton;
    abutton->setFixedWidth(0.1*this->width());
    abutton->setFixedHeight(0.04*this->height());
    abutton->setText("SELECT");
    abutton->setStyleSheet("background-color:#b3adb2; color:#c90822; font-family:calibri; font-size:21px;");
    l->addLayout(hl);
    hl->addWidget(lb);
    hl->addWidget(algos);
    hl->addWidget(abutton);
    connect(abutton,SIGNAL(clicked(bool)),this,SLOT(algorithmselector()));


    //----------------------------------------------------------------------------
    //Table show algorithm results
    QStringList headers;
    headers<<"FILE"<<"VERTICES"<<"COLORS USED"<<"ALGORITHM";
    specify=new QTableWidget;
    specify->setStyleSheet("font-size:21px; font-weight:bold; color:red;");
    specify->setFixedSize(0.99*this->width(),0.7*this->height());
    specify->setColumnCount(headers.size());
    specify->setHorizontalHeaderLabels(headers);
    for(int i=0,t=headers.size();i<t;i++)
    specify->setColumnWidth(i,specify->width()/t);
    l->addWidget(specify);
}

void MainWindow::algorithmselector()
{
   QString salgo=algos->currentText();
   if(salgo=="MASSIVE")
   {
     specify->clear();
     QStringList headers;
     headers<<"FILE"<<"VERTICES"<<"FIRST FIT"<<"DSATUR"<<"RLF"<<"BDSATUR";
     specify->setColumnCount(headers.size());
     specify->setHorizontalHeaderLabels(headers);
     for(int i=0,t=headers.size();i<t;i++)
     {
         specify->setColumnWidth(i,specify->width()/t);
     }
     QVector <algo> results=mydb->MASSIVE();
     specify->setRowCount(results.size());
     for(int i=0,t=specify->rowCount();i<t;i++)
     {
         QTableWidgetItem *it1=new QTableWidgetItem();
         it1->setTextAlignment(Qt::AlignCenter);
         it1->setForeground(Qt::blue);
         it1->setText(results.at(i).filename);
         specify->setItem(i,0,it1);
         QTableWidgetItem *it2=new QTableWidgetItem();
         it2->setTextAlignment(Qt::AlignCenter);
         it2->setForeground(Qt::blue);
         it2->setText(QString::number(mydb->getNOD(results.at(i).filename)));
         specify->setItem(i,1,it2);
         QTableWidgetItem *it3=new QTableWidgetItem();
         it3->setTextAlignment(Qt::AlignCenter);
         it3->setForeground(Qt::blue);
         it3->setText(QString::number(results.at(i).ff));
         specify->setItem(i,2,it3);
         QTableWidgetItem *it4=new QTableWidgetItem();
         it4->setTextAlignment(Qt::AlignCenter);
         it4->setForeground(Qt::blue);
         it4->setText(QString::number(results.at(i).ds));
         specify->setItem(i,3,it4);
         QTableWidgetItem *it5=new QTableWidgetItem();
         it5->setTextAlignment(Qt::AlignCenter);
         it5->setForeground(Qt::blue);
         it5->setText(QString::number(results.at(i).rlf));
         specify->setItem(i,4,it5);
         QTableWidgetItem *it6=new QTableWidgetItem();
         it6->setTextAlignment(Qt::AlignCenter);
         it6->setForeground(Qt::blue);
         it6->setText(QString::number(results.at(i).bds));
         specify->setItem(i,5,it6);
     }
   }
   else if(salgo=="FIRST FIT")
   {
       QString alc="FIRST FIT";
       specify->clear();
       QStringList headers;
       headers<<"FILE"<<"VERTICES"<<"COLORS"<<"ALGORITHM";
       specify->setColumnCount(headers.size());
       for(int i=0,t=headers.size();i<t;i++)
       {
           specify->setColumnWidth(i,specify->width()/t);
       }
       specify->setHorizontalHeaderLabels(headers);
       QMap <QString,int> res=mydb->FIRST_FIT();
       specify->setRowCount(res.size());
       for(int i=0,t=specify->rowCount();i<t;i++)
       {
          QTableWidgetItem *it1=new QTableWidgetItem();
          it1->setTextAlignment(Qt::AlignCenter);
          it1->setForeground(Qt::blue);
          it1->setText((res.begin()+i).key());
          specify->setItem(i,0,it1);
          QTableWidgetItem *it2=new QTableWidgetItem();
          it2->setTextAlignment(Qt::AlignCenter);
          it2->setForeground(Qt::blue);
          QString afile=(res.begin()+i).key();
          it2->setText(QString::number(mydb->getNOD(afile)));
          specify->setItem(i,1,it2);
          QTableWidgetItem *it3=new QTableWidgetItem();
          it3->setTextAlignment(Qt::AlignCenter);
          it3->setForeground(Qt::blue);
          it3->setText(QString::number((res.begin()+i).value()));
          specify->setItem(i,2,it3);
          QTableWidgetItem *it4=new QTableWidgetItem();
          it4->setForeground(Qt::blue);
          it4->setTextAlignment(Qt::AlignCenter);
          it4->setText(alc);
          specify->setItem(i,3,it4);
       }

   }
   else if(salgo=="DSATUR")
   {
       QString alc="DSATUR";
       specify->clear();
       QStringList headers;
       headers<<"FILE"<<"VERTICES"<<"COLORS"<<"ALGORITHM";
       specify->setColumnCount(headers.size());
       specify->setHorizontalHeaderLabels(headers);
       for(int i=0,t=headers.size();i<t;i++)
       {
           specify->setColumnWidth(i,specify->width()/t);
       }
       res=mydb->DSATUR_RESULTS();
       specify->setRowCount(res.size());
       for(int i=0,t=specify->rowCount();i<t;i++)
       {
          QTableWidgetItem *it1=new QTableWidgetItem();
          it1->setTextAlignment(Qt::AlignCenter);
          it1->setForeground(Qt::blue);
          it1->setText((res.begin()+i).key());
          specify->setItem(i,0,it1);
          QTableWidgetItem *it2=new QTableWidgetItem();
          it2->setTextAlignment(Qt::AlignCenter);
          it2->setForeground(Qt::blue);
          it2->setText(QString::number(mydb->getNOD((res.begin()+i).key())));
          specify->setItem(i,1,it2);
          QTableWidgetItem *it3=new QTableWidgetItem();
          it3->setTextAlignment(Qt::AlignCenter);
          it3->setForeground(Qt::blue);
          it3->setText(QString::number((res.begin()+i).value()));
          specify->setItem(i,2,it3);
          QTableWidgetItem *it4=new QTableWidgetItem();
          it4->setForeground(Qt::blue);
          it4->setTextAlignment(Qt::AlignCenter);
          it4->setText(alc);
          specify->setItem(i,3,it4);
       }
   }
   else if(salgo=="RLF")
   {
       QString alc="RLF";
       specify->clear();
       QStringList headers;
       headers<<"FILE"<<"VERTICES"<<"COLORS"<<"ALGORITHM";
       specify->setColumnCount(headers.size());
       specify->setHorizontalHeaderLabels(headers);
       for(int i=0,t=headers.size();i<t;i++)
       {
           specify->setColumnWidth(i,specify->width()/t);
       }
       res=mydb->RLF_RESULTS();
       specify->setRowCount(res.size());
       for(int i=0,t=specify->rowCount();i<t;i++)
       {
          QTableWidgetItem *it1=new QTableWidgetItem();
          it1->setTextAlignment(Qt::AlignCenter);
          it1->setForeground(Qt::blue);
          it1->setText((res.begin()+i).key());
          specify->setItem(i,0,it1);
          QTableWidgetItem *it2=new QTableWidgetItem();
          it2->setTextAlignment(Qt::AlignCenter);
          it2->setForeground(Qt::blue);
          it2->setText(QString::number(mydb->getNOD((res.begin()+i).key())));
          specify->setItem(i,1,it2);
          QTableWidgetItem *it3=new QTableWidgetItem();
          it3->setTextAlignment(Qt::AlignCenter);
          it3->setForeground(Qt::blue);
          it3->setText(QString::number((res.begin()+i).value()));
          specify->setItem(i,2,it3);
          QTableWidgetItem *it4=new QTableWidgetItem();
          it4->setForeground(Qt::blue);
          it4->setTextAlignment(Qt::AlignCenter);
          it4->setText(alc);
          specify->setItem(i,3,it4);
       }
   }
   else
   {
       QString alc="BDSATUR";
       specify->clear();
       QStringList headers;
       headers<<"FILE"<<"VERTICES"<<"COLORS"<<"ALGORITHM";
       specify->setColumnCount(headers.size());
       specify->setHorizontalHeaderLabels(headers);
       for(int i=0,t=headers.size();i<t;i++)
       {
           specify->setColumnWidth(i,specify->width()/t);
       }
       QMap <QString,int> res=mydb->BDSATUR_RESULTS();
       specify->setRowCount(res.size());
       for(int i=0,t=specify->rowCount();i<t;i++)
       {
          QTableWidgetItem *it1=new QTableWidgetItem();
          it1->setTextAlignment(Qt::AlignCenter);
          it1->setForeground(Qt::blue);
          it1->setText((res.begin()+i).key());
          specify->setItem(i,0,it1);
          QTableWidgetItem *it2=new QTableWidgetItem();
          it2->setTextAlignment(Qt::AlignCenter);
          it2->setForeground(Qt::blue);
          it2->setText(QString::number(mydb->getNOD((res.begin()+i).key())));
          specify->setItem(i,1,it2);
          QTableWidgetItem *it3=new QTableWidgetItem();
          it3->setTextAlignment(Qt::AlignCenter);
          it3->setForeground(Qt::blue);
          it3->setText(QString::number((res.begin()+i).value()));
          specify->setItem(i,2,it3);
          QTableWidgetItem *it4=new QTableWidgetItem();
          it4->setForeground(Qt::blue);
          it4->setTextAlignment(Qt::AlignCenter);
          it4->setText(alc);
          specify->setItem(i,3,it4);
       }
   }
}

//visualize sub-graphs

void MainWindow::visualize()
{
    if(this->geffects==nullptr) return;
    QString selectedvertex=cb->currentText();
    int pos=-1;
    for(int i=0,t=this->s->exams.size();i<t;i++)
    {
        if(this->s->exams[i]==selectedvertex)
        {
            pos=i;
        }
    }
    if(pos==-1) return;
    QVector <int> neibourghs=this->agraph->get_neibourghs(pos);
    QStringList n;
    for(auto &x:neibourghs)
    {
      n.append(this->s->exams.at(x));
    }
    geffects->setSource(selectedvertex);
    geffects->setTargets(n);
}

void MainWindow::visual_window()
{
    //Αποτελέσματα κορυφών με χρήση αντικειμένου QPainter.
    QWidget *mw=new QWidget;
    mw->setFixedSize(this->width(),this->height());
    lay->addWidget(mw);
    QVBoxLayout *mainwinlay=new QVBoxLayout;
    mw->setLayout(mainwinlay);
    v=new visualization(mw->width()*0.98,mw->height()*0.95);
    mainwinlay->addWidget(v);
}

//Εκχώρηση πληροφοριών για εμφάνισητου τελικού γράφου.
void MainWindow::finalgraph()
{
    v->clear();
    QPushButton *signal=(QPushButton *)sender();
    QString ver=signal->property("vertex").toString();
    int pos=-1;
    int counter=0;
    for(auto &x:s->exams)
    {
      if(x==ver)
      {
         pos=counter;
         break;
      }
      counter++;
    }
    QVector <int> nbs=agraph->get_neibourghs(pos);
    v->setList(nbs);
    v->setSelectedVertex(ver);
    QVector <QColor> allcolors;
    v->setStartColor(colorlabels.at(spots.at(pos)));
    for(auto &x:nbs)
    {
       allcolors.append(colorlabels.at(spots.at(x)));
    }
    v->setColors(allcolors);
    page.push(lay->currentIndex());
    lay->setCurrentIndex(6);
}

void MainWindow::quit()
{
  exit(EXIT_SUCCESS);
}

void MainWindow::copyright()
{
   QMessageBox *msg=new QMessageBox();
   //msg->setStyleSheet();
   msg->setIconPixmap(QPixmap(":/copyright.png").scaled(40,40));
   msg->setWindowTitle("COPYRIGHTS");
   QString copies="<html><h2>Copyrights</h2><hr style=\"border-top:2px solid blue;\"><h4><center>©Nastos Vasileios 2021</center><br>All rights reserved</h4></html>";
   msg->setText(copies);
   msg->showNormal();
}
