#include <iostream>
#include <vector>
#include <map>
#include <list>

int count_common_elements(std::set <int> &s1,std::set <int> &s2)
{
    int common=0;
    for(auto &x:s1)
    {
        for(auto &y:s2)
        {
            if(x==y)
            {
                common++;
            }
        }
    }
    return common;
}

class graph
{
    std::list <int> *adj;
    int V;
    public:
      graph(int v):V(v) {
          this->adj=new std::list<int>[v+1];
      }
      graph() {delete[] this->adj;}
      void fill_graph(std::vector <std::set <int>> &lst)
      {
         for(int i=1,t=lst.size();i<t;i++)
         {
             for(int j=1;j<lst.size();j++)
             {
                 if(i==j) continue;
                 if(count_common_elements(lst[i],lst[j])!=0)
                 {
                     this->adj[i].push_back(j);
                 }
             }
         }
      }
      std::vector <int> coloring()
      {
          std::vector <int> res;
          for(int i=0;i<this->V+1;i++)
          {
              res.push_back(INT_MIN);
          }
          bool *available=new bool[this->V+1];
          for(int i=1;i<=this->V;i++)
          {
              res[i]=-1;
              available[i]=true;
          } 
          available[0]=false;
          res[0]=INT_MIN;
          res[1]=0;
          for(int i=2;i<=this->V;i++)
          {
             std::list <int>::iterator itr=this->adj[i].begin();
             while(itr!=this->adj[i].end())
             {
                 if(res[*itr]!=-1)
                 {
                    available[*itr]=false;
                 }
                 itr++;
             }
             int av;
             for(av=0;av<=this->V;av++)
             {
                 if(available[av])
                 {
                     break;
                 }
             }
             res[i]=av;
             for(auto j=this->adj[i].begin();j!=this->adj[i].end();j++)
             {
                 available[*j]=true;
             }
          }
         delete[] available;
         return res;
      }
};