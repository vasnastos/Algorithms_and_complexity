#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <set>
#include <thread>
#include <chrono>
#include <fstream>
#define VERTICES 139

int summaryofcolors;
std::map<int, int> previous_saturation;
std::map<int, std::list<int>> vertices;
std::vector<int> saturation_level;
std::vector<int> coloring;
std::vector<std::set<int>> occurences;

//max clique
void max_vertex()
{
    int mv = -1;
    int ms = -1;
    int scd;
    const int size = vertices.size();
    for (auto itr = vertices.begin(); itr != vertices.end(); itr++)
    {
        scd = itr->second.size();
        if (scd > ms)
        {
            ms = itr->second.size();
            mv = itr->first;
        }
    }
    coloring[mv] = 1;
    saturation_level[mv] = INT_MIN;
    for (auto i = vertices[mv].begin(); i != vertices[mv].end(); i++)
    {
        saturation_level[*i]++;
    }
}

void OnInit()
{
    for (int i = 0; i <= VERTICES; i++)
    {
        coloring.push_back(-2);
        saturation_level.push_back(0);
    }
    max_vertex();
}

int max_saturated_vertex()
{
    int ms = -1;
    int msv = -1;
    for (int i = 0; i < saturation_level.size(); i++)
    {
        if (saturation_level.at(i) > msv && saturation_level.at(i) != INT_MIN)
        {
            msv = saturation_level.at(i);
            ms = i;
        }
    }
    for (auto i = vertices[ms].begin(); i != vertices[ms].end(); i++)
    {
        saturation_level[*i]++;
    }
    if(ms==-1) return -1;
    previous_saturation[ms] = saturation_level.at(ms);
    saturation_level[ms] = INT_MIN;
    return ms;
}

bool is_safe(int c, int vertex)
{
    for (std::list<int>::iterator i = vertices[vertex].begin(); i != vertices[vertex].end(); i++)
    {
        if (coloring[*i] == c)
        {
            return false;
        }
    }
    return true;
}

bool check_coloring()
{
    for (auto &x : coloring)
    {
        if (x < 0)
        {
            return false;
        }
    }
    for (int i = 0, t = coloring.size(); i < t; i++)
    {
        for (auto &x : vertices[i])
        {
            if (coloring.at(i) == coloring.at(x))
            {
                return false;
            }
        }
    }
    return true;
}

void reset(int vi)
{
    saturation_level[vi] = previous_saturation[vi];
    for (auto &x : vertices[vi])
    {
        saturation_level.at(x)--;
    }
}

bool make_coloring()
{
    int vi = max_saturated_vertex();
    if (vi == -1)
    {
        return true;
    }
    //std::cout<<vi<<std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    int color = 1;
    while (color <= summaryofcolors)
    {
        if (is_safe(color, vi))
        {
            coloring[vi] = color;
            if (make_coloring())
            {
                return true;
            }
            else
            {
                coloring[vi] = -1;
                reset(vi);
            }
        }
        color++;
    }
    return false;
}

int count_common_elements(std::set<int> &s1, std::set<int> &s2)
{
    int common = 0;
    for (auto &x : s1)
    {
        for (auto &y : s2)
        {
            if (x == y)
            {
                common++;
            }
        }
    }
    return common;
}

void initiate_data()
{
    occurences.push_back(std::set<int>());
    for (int i = 1; i <= VERTICES; i++)
    {
        occurences.push_back(std::set<int>());
        vertices[i] = std::list<int>();
    }
}

void fill_adjacency()
{
    for (int i = 1, t = occurences.size(); i < t; i++)
    {
        for (int j = 1, sz = occurences.size(); j < sz; j++)
        {
            if (i == j)
                continue;
            if (count_common_elements(occurences[i], occurences[j]) != 0)
            {
                vertices[i].push_back(j);
            }
        }
    }
}

void read_data()
{
    initiate_data();
    std::string line;
    std::ifstream is;
    is.open("../sta-f-83.stu");
    int student = 1;
    std::string word;
    while (std::getline(is, line))
    {
        std::istringstream iss(line);
        while (std::getline(iss, word, ' '))
        {
            if (word.empty())
            {
                continue;
            }
            occurences[std::stoi(word)].insert(student);
        }
        student++;
    }
    is.close();
    fill_adjacency();
}

//read data and test dsatur
int main()
{
    read_data();
    OnInit();
    std::cout << "Give Number of colors:";
    std::cin >> summaryofcolors;
    if (make_coloring() && check_coloring())
    {
        std::ofstream out("results_bds.csv");
        std::cout << "Solution found!!!" << std::endl;
        std::cout << "\tBDSATUR SOLUTION" << std::endl;
        std::cout << "******************************************" << std::endl;
        out << "\t\tFileName:sta-f-83.stu" << std::endl;
        out << "\t\tFile Created at:" << __TIME__ << std::endl;
        out << "\t\tBacktracking DSatur " << std::endl;
        out << "\t\tColors used:" << summaryofcolors << std::endl;
        out << "--------------------------------------------" << std::endl;
        out << ";;;VERTEX;COLOR" << std::endl;
        for (int i = 1; i < coloring.size(); i++)
        {
            out << ";;;VERTEX_" << i << ";" << coloring.at(i) << std::endl;
            std::cout << "Vertex " << i << "-->Color:" << coloring.at(i) << std::endl;
        }
        out.close();
    }
    else
    {
        std::cerr << "Can not find a solution with these amount of colors" << std::endl;
        ;
    }
    return 0;
}