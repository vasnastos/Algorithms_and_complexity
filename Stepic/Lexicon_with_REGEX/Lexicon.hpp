#include <iostream>
#include <regex>
#include <vector>
#include <list>
#include <fstream>

class Lexicon
{
    std::vector <std::string> words;
    public:
       Lexicon(std::string filename);
       ~Lexicon();
       int startswith(const std::string &reg);
       int endswith(const std::string &reg);
       int contains(const std::string &reg,int reps);
       int find_sequence(std::string &reg);
};

void hr();