#include "graph.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>


graph::graph(int v):available(nullptr),V(v),adj(new std::list<int>[V]) {}
void graph::add_connection(int s,int t)
{
    adj[s].push_back(t);
}

graph::~graph() {}

void graph::free_adjecency()
{
    delete[] this->adj;
    delete[] available;
}

QVector <int> graph::get_neibourghs(int ver)
{
    QVector <int> neibourghs;
    for(std::list <int>::iterator itr=this->adj[ver].begin();itr!=this->adj[ver].end();itr++)
    {
        neibourghs.append(*itr);
    }
  return neibourghs;
}

void graph::export_neibourghs(const QStringList &names)
{
  QString file=QFileDialog::getOpenFileName(nullptr,"Save Neibourghs",".","text file (*.txt *.csv)");
  if(file.size()==0)
  {
      QMessageBox::critical(nullptr,"Error","Please select an input file");
      return;
  }
  QFile fp(file);
  fp.open(QIODevice::ReadOnly);
  QTextStream st(&fp);
  for(int i=0;i<names.size();i++)
  {
      st<<"******* Vertices for vertex "<<names[i]<<" ******"<<endl;
      for(auto itr=this->adj[i].begin();itr!=this->adj[i].end();itr++)
      {
       st<<"Neibourgh_"<<i+1<<":"<<names[*itr];
      }
  }
  fp.close();
}

QVector <int> graph::coloring()
{
   QVector <int> colors;
   available=new bool[this->V];
   colors.append(0);
   for(int v=0;v<this->V;v++)
   {
      available[v]=true;
      if(v==0) continue;
      colors.append(-1);
   }
   for(int u=0;u<this->V;u++)
   {
       std::list<int>::iterator i;
       for(i=adj[u].begin();i!=adj[u].end();++i)
       {
           if(colors[*i]!=-1)
           {
             available[colors[*i]]=false;
           }
       }
       int cav;
       for(cav=0;cav<this->V;cav++)
       {
           if(available[cav]) break;
       }
       colors[u]=cav;
       for(i=adj[u].begin();i!=adj[u].end();++i)
       {
           available[colors[*i]]=true;
       }
   }
   return colors;
}

using std::map;

map <int,int> graph::DSatur()
{
  map <int,int> nodes;
  map <int,QVector <int>> neibourghs;
  if(this->V==0) {return nodes;}
  for(int i=1;i<=this->V;i++)
  {
      neibourghs.insert(std::make_pair(i,this->get_neibourghs(i-1)));
  }
  int degree=-1;
  int maxvertex=-1;
  for(auto &x:neibourghs)
  {
     if(x.second.size()>degree)
     {
         degree=x.second.size();
         maxvertex=x.first;
     }
  }
  nodes[maxvertex]=0;
  QVector <int> todo;
  map <int,int> saturation_level;
  for(int i=1;i<=this->V;i++)
  {
    if(i!=maxvertex)
    {
        nodes[i]=-1;
        todo.push_back(i);
    }
  }
  for(int i=1;i<=this->V;i++)
  {
      saturation_level[i]=0;
  }
  saturation_level[maxvertex]=INT_MIN;
  for(auto &x:neibourghs[maxvertex])
  {
      saturation_level[x]++;
  }
  while(!todo.empty())
  {
      int saturation=-1;
      int satur_vertex=-1;
      QVector <int> saturation_colors;
      int j;
      for(std::map <int,int>::iterator i=saturation_level.begin();i!=saturation_level.end();i++)
      {
          j=i->second;
         if(j>saturation)
         {
             saturation=i->second;
             satur_vertex=i->first;
             saturation_colors.clear();
             for(auto &x:neibourghs[satur_vertex])
             {
                 saturation_colors.append(nodes[x]);
             }
         }
      }
      for(QVector <int>::iterator titr=todo.begin();titr!=todo.end();titr++)
      {
         if(*titr==satur_vertex)
         {
            todo.erase(titr);
            break;
         }
      }
      int lowest_color=0;
      bool done=false;
      while(!done)
      {
          done=true;
          for(auto x:saturation_colors)
          {
              if(x==lowest_color)
              {
                  lowest_color++;
                  done=false;
              }
          }
      }
      nodes[satur_vertex]=lowest_color;
      for(QVector <int>::iterator i=neibourghs[satur_vertex].begin();i<neibourghs[satur_vertex].end();i++)
      {
          if(saturation_level[*i]!=INT_MIN)
          {
              saturation_level[*i]++;
          }
      }
      saturation_level[satur_vertex]=INT_MIN;
  }
  return nodes;
}

std::list <int> graph::list_of_neibourghs(int vrid)
{
    return this->adj[vrid];
}


//RLF IMPLEMENTATION
void graph::swap(int &a, int &b){
    int x=a; a=b; b=x;
}


void graph::removeElement(QVector<int>& A, int i) {
    //Constant time operation for removing an item from a vector (note that ordering is not maintained)
    swap(A[i], A.back());
    A.pop_back();
}

void graph::chooseVertex(QVector<int>& X, int& v, int& vPos, QVector<int>& Deg) {
    //Select the vertex in X that has the maximum corresponding value in Deg
    //Ties are broken randomly
    int i, max = Deg[0], numMax = 1;
    v = X[0];
    vPos = 0;
    for (i = 1; i < X.size(); i++) {
        if (Deg[i] >= max) {
            if (Deg[i] > max) numMax = 0;
            if (rand() % (numMax + 1) == 0) {
                max = Deg[i];
                v = X[i];
                vPos = i;
            }
            numMax++;
        }
    }
}

//-------------------------------------------------------------------------------------
void graph::updateX(QVector<int>& X, QVector<int>& XDeg, int v, int vPos, QVector<int>& Y, QVector<int>& YDeg, QVector< QVector<bool> >& adjacent, QVector<int>& NInY) {
    int i = 0, j;
    //Remove v from X and update the relevant vectors
    removeElement(X, vPos);
    removeElement(XDeg, vPos);
    removeElement(NInY, vPos);
    //Transfer all vertices in X that are adjacent to v (which has already been removed) into Y. Also update the degree vectors
    while (i < X.size()) {
        numofchecks++;
        if (adjacent[X[i]][v]) {
            //Move vertex at X[i] to Y. Also transfer X[i]'s degree minus 1 (because the edge between v and X[i] has been removed)
            Y.append(X[i]);
            YDeg.append(XDeg[i] - 1);
            removeElement(X, i);
            removeElement(XDeg, i);
            removeElement(NInY, i);

            //Since a new vertex is being moved to Y, any vertex in X that is adjacent has its NInY entry updated
            for (j = 0; j < X.size(); j++) {
                numofchecks++;
                if (adjacent[X[j]][Y.back()]) {
                    NInY[j]++;
                }
            }
        }
        else {
            i++;
        }
    }
}

//-------------------------------------------------------------------------------------
void graph::makeSolution(QVector< QVector<int> >& candSol, QVector<int>& degree, QVector<int>& colNode, QVector< QVector<bool> >& adjacent, int numNodes)
{
    int i, c, v, vPos;
    candSol.clear();

    //X is a vector containing all unplaced vertices that can go into the current colour c (initially contains all unplaced vertices).
    //XDeg contains the degree of the vertices induced by the subgraph of X (i.e. XDeg[i] == degree of X[i])
    //Y is used to hold vertices that clash with vertices currently assigned to colour c (initially empty).
    //YDeg contains the degree of the vertices induced by the subgraph of Y (i.e. YDeg[i] == degree of Y[i])
    //NInY contains the number of neighbours vertex X[i] has in Y (initially zero because Y is empty at the start of each iteration)
    QVector<int> X(numNodes), Y, XDeg, YDeg, NInY(numNodes, 0);
    for (i = 0; i < numNodes; i++) X[i] = i;
    XDeg = degree;

    //Run the RLF algorithm
    c = 0;
    while (!X.empty()) {
        //Open a new colour c
        candSol.append(QVector<int>());
        //Choose the vertex v in X that has the largest degree in the subgraph induced by X, then add v to colour c
        chooseVertex(X, v, vPos, XDeg);
        candSol[c].push_back(v);
        colNode[v] = c;
        updateX(X, XDeg, v, vPos, Y, YDeg, adjacent, NInY);
        while (!X.empty()) {
            //Choose the vertex v that has the largest number of neighbours in Y, then add v to colour c
            chooseVertex(X, v, vPos, NInY);
            candSol[c].push_back(v);
            colNode[v] = c;
            updateX(X, XDeg, v, vPos, Y, YDeg, adjacent, NInY);
        }
        X.swap(Y);
        XDeg.swap(YDeg);
        NInY.resize(X.size());
        c++;
    }
}

QVector <int> graph::rlf()
{
    numofchecks = 0;
    QVector< QVector<int> > candSol;
    QVector<int> colNode(this->V, INT_MIN);
    QVector <int> degree;
    for (int i = 0; i < this->V; i++)
    {
        degree.append(this->adj[i].size());
    }
    QVector <QVector <bool>> adjacent;
    for (int i = 0; i < this->V; i++)
    {
        QVector <bool> ch;
        for (int j = 0; j < this->V; j++)
        {
            bool f = false;
            for (auto itr = this->adj[i].begin(); itr != this->adj[i].end(); itr++)
            {
                if (*itr == j)
                {
                    ch.append(true);
                    f = true;
                    break;
                }
            }
            if (!f)
            {
                ch.append(false);
            }
        }
        adjacent.push_back(ch);
    }
    makeSolution(candSol, degree, colNode, adjacent, this->V);

    QVector <int> colors(this->V);
    int k = 0;
    for (auto& x : candSol)
    {
        for (auto& y : x)
        {
            colors[y]=k;
        }
        k++;
    }
    return colors;
}
