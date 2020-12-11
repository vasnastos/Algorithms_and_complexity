#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <set>
#include <queue>

std::vector <std::string> vertices;

void read_data(std::string fn,std::vector <std::list <int>> &g)
{
    std::fstream fs;
    fs.open(fn,std::ios::in);
    std::string line,word;
    std::vector <int> data;
    while(std::getline(fs,line))
    {
      std::list <int> a_list;
      std::stringstream ss(line);
      data.clear();
      while(std::getline(ss,word,','))
      {
         data.push_back(std::stoi(word));
      }
      for(int j=0,t=data.size();j<t;j++)
      {
          if(data.at(j)!=0)
          a_list.push_back(j);
      }
      g.push_back(a_list);
    }
    fs.close();
    const int totalvertices=g.size();
    for(int i=0;i<totalvertices;i++)
    {
        char name='A'+i;
        std::string stringname="";
        stringname+=name;
        vertices.push_back(stringname);
    }
}

void BFS(std::string fn)
{
    std::vector <std::list <int>> g;
    read_data(fn,g);
    std::set <int> visited;
    std::vector <int> child;
    std::queue <int> trset;
    trset.push(0);
    for(std::list<int>::iterator i=g[0].begin();i!=g[0].end();i++)
    {
        child.push_back(*i);
    }
    visited.insert(0);
    trset.pop();
    for(auto &x:child)
    {
        trset.push(x);
    }
    while(!trset.empty())
    {
        child.clear();
        int checkvertex=trset.front();
        for(std::list <int>::iterator i=g[checkvertex].begin();i!=g[checkvertex].end();i++)
        {
            child.push_back(*i);
        }
        visited.insert(checkvertex);
        trset.pop();
        for(auto &x:child)
        {
            trset.push(x);
        }
    }
    std::cout<<"@@@@@@@@@@@@@@ BFS RESULTS  @@@@@@@@@@@@@"<std::endl;
    for(std::set <int>::iterator itr=visited.begin();itr!=visited.end();itr++)
    {
        std::cout<<vertices[*itr]<<" ";
    }
    std::cout<<std::endl;
}

int main(int argc,char **argv)
{
    if(argc!=2)
    {
        std::cerr<<"Please give the correct amount of arguments!!!a.exe *.txt"<<std::endl;
        return 1;
    }
    BFS(argv[1]);
}