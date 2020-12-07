#include "Lexicon.hpp"
#include <ctime>
#include <ratio>
#include <chrono>

void hr()
{
    std::cout<<"##############################################################################"<<std::endl<<std::endl;;
}

Lexicon::~Lexicon() {std::cout<<std::endl<<"Destruct the lexicon Thank you for use it"<<std::endl;}

Lexicon::Lexicon(std::string filename)
{
   std::fstream fs;
   fs.open(filename,std::ios::in);
   std::string line;
   std::chrono::high_resolution_clock::time_point t1=std::chrono::high_resolution_clock::now();
   while(std::getline(fs,line))
   {
       this->words.push_back(line);
   }
   fs.close();
   std::chrono::high_resolution_clock::time_point t2=std::chrono::high_resolution_clock::now();
   std::chrono::duration <double> timecount=std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
   std::cout<<"##### wordlist inserted after "<<timecount.count()<<" ####"<<std::endl;
   std::cout<<"\t\tTotal words inserted:"<<this->words.size()<<std::endl;
   hr();
}

int Lexicon::startswith(const std::string &reg)
{
    std::string regularexpression=reg+"([^]*)";
    std::regex r(regularexpression);
    std::list <std::string> regexwords;
    std::chrono::high_resolution_clock::time_point t1=std::chrono::high_resolution_clock::now();
    for(auto &x:this->words)
    {
        if(std::regex_match(x,r))
        {
            regexwords.push_back(x); 
        }
    }
    std::chrono::high_resolution_clock::time_point t2=std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> timecount=std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
    std::cout<<"#### Words which starts with "<<reg<<" found after "<<timecount.count()<<" #####"<<std::endl;
    std::cout<<"\t\tTotal words found:"<<regexwords.size()<<std::endl;
    hr();
    char answer;
    std::cout<<"Show Words(y-YES,n-NO):";
    std::cin>>answer;
    answer=std::tolower(answer);
    hr();
    if(answer=='y')
    {
        std::cout<<"\t\tWords Start with "<<reg<<std::endl;
        for(std::list <std::string>::iterator itr=regexwords.begin();itr!=regexwords.end();itr++)
        {
            std::cout<<*itr<<std::endl;
        }
       std::cout<<std::endl;
    }
    return regexwords.size();
}

int Lexicon::endswith(const std::string &reg)
{
    std::string regularexpression="([^]*)"+reg+"$";
    std::regex r(regularexpression);
    std::list <std::string> regexwords;
    std::chrono::high_resolution_clock::time_point t1=std::chrono::high_resolution_clock::now();
    for(auto &x:this->words)
    {
        if(std::regex_match(x,r))
        {
            regexwords.push_back(x);
        }
    }
    std::chrono::high_resolution_clock::time_point t2=std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> timecount=std::chrono::duration_cast<std::chrono::duration <double>>(t2-t1);
    std::cout<<"### Find words end with "<<reg<<" after "<<timecount.count()<<" ###"<<std::endl;
    std::cout<<"\t\tTotal words found:"<<regexwords.size()<<std::endl;
    hr();
    char answer;
    std::cout<<"Show words(y/Y-YES,n/N-NO):";
    std::cin>>answer;
    answer=std::tolower(answer);
    hr();
    if(answer=='y')
    {
        std::cout<<"Words end with "<<reg<<std::endl;
        for(auto i=regexwords.begin();i!=regexwords.end();i++)
        {
            std::cout<<*i<<std::endl;
        }
    } 
    std::cout<<std::endl;
    return regexwords.size();
}

int Lexicon::contains(const std::string &reg,int reps)
{
    std::string regularexpression="(([^]*)"+reg+"([^]*)){"+std::to_string(reps)+"}";
    std::smatch m;
    std::regex r(regularexpression);
    std::list <std::string> regexwords;
    std::chrono::high_resolution_clock::time_point t1=std::chrono::high_resolution_clock::now();
    for(auto &x:this->words)
    {
        if(std::regex_match(x,r))
        {
            regexwords.push_back(x);
        }
    }
    std::chrono::high_resolution_clock::time_point t2=std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> time_count=std::chrono::duration_cast<std::chrono::duration <double>>(t2-t1);
    std::cout<<"#### Words which contains "<<reg<<" "<<reps<<" times found at "<<time_count.count()<<" ####"<<std::endl;
    std::cout<<"\t\tTotal words:"<<regexwords.size()<<std::endl;
    hr();
    char answer;
    std::cout<<"Show Words(y/Y-YES,n/N-NO):";
    std::cin>>answer;
    answer=std::tolower(answer);
    hr();
    if(answer=='y')
    {
        std::cout<<"Words which contain "<<reg<<" "<<reps<<" times"<<std::endl;
        for(auto i=regexwords.begin();i!=regexwords.end();i++)
        {
            std::cout<<*i<<std::endl;
        }
    }
    std::cout<<std::endl;
    return regexwords.size();
}

int Lexicon::find_sequence(std::string &reg)
{
    std::string replace="[^]";
    std::string regularexpression="";
    std::chrono::high_resolution_clock::time_point t1=std::chrono::high_resolution_clock::now();
    for(int i=0;i<reg.length();i++)
    {
        if(reg.at(i)=='-')
        {
            regularexpression+=replace;
            continue;
        }
        regularexpression+=reg.at(i);
    } 
    std::list <std::string> regexwords;
    std::regex r(regularexpression);
    for(auto &x:words)
    {
        if(std::regex_match(x,r))
        {
           regexwords.push_back(x);
        }
    }
    std::chrono::high_resolution_clock::time_point t2=std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> timecount=std::chrono::duration_cast<std::chrono::duration <double>>(t2-t1);
    std::cout<<"#### Words which following the RE format "<<regularexpression<<" found after "<<timecount.count()<<" ms ####"<<std::endl;
     std::cout<<"\t\tTotal Words:"<<regexwords.size()<<std::endl;
     hr();
     char answer;
     std::cout<<"Show Words(y/Y-YES,n/N-No):";
     std::cin>>answer;
     answer=std::tolower(answer);
     if(answer=='y')
     {
        std::cout<<"Words which contain "<<reg<<std::endl;
        for(auto i=regexwords.begin();i!=regexwords.end();i++)
        {
            std::cout<<*i<<std::endl;
        }
     }
     std::cout<<std::endl;
     return regexwords.size();
}