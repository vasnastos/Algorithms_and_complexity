#include "Lexicon.hpp"

std::string getfilename(int counter,char **board)
{
    if(counter!=2)
    {
        std::cerr<<"Invalid number of arguments please try again and run as a.exe filename.(*))"<<std::endl;
        return "";
    }
    return board[counter-1];
}

void summary_board(int first,int second,int third,int fourth)
{
    std::cout<<"========================================================"<<std::endl;
    std::cout<<"\t\tTest of Starts With result:"<<first<<std::endl;
    std::cout<<"\t\tTest of Ends with result:"<<second<<std::endl;
    std::cout<<"\t\tTest of Contains result:"<<third<<std::endl;
    std::cout<<"\t\tTest of SubSequence result:"<<fourth<<std::endl;
    std::cout<<"========================================================"<<std::endl;
}

int main(int argc,char **argv)
{
    std::string filename=getfilename(argc,argv);
    if(filename=="") return 1;
    std::string seq="b-n-n-";
    Lexicon l(filename);
    int startswith=l.startswith("am");
    int endswith=l.endswith("nts");
    int contains=l.contains("t",4);
    int subsequence=l.find_sequence(seq);
    hr();
    summary_board(startswith,endswith,contains,subsequence);
    return 0;
}