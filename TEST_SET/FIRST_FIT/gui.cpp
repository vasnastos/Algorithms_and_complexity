#include <wx/wx.h>
#include <iostream>
#include <set>
#include <string>
#include <map>
#include <random> 
#include "greedy.cpp"
#include <map>
#include <chrono>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <wx/filefn.h> 

//Values at files separated by ' '
struct rgb
{
    int red;
    int green;
    int blue;
};

std::map<int,rgb> get_colors(int max_color)
{
    std::map <int,rgb> colors;
   std::mt19937 eng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
   std::uniform_int_distribution <int> dist(0,255);
   for(int i=0;i<=max_color;i++)
   {
      colors.insert(std::make_pair(i,rgb{dist(eng),dist(eng),dist(eng)}));
   }
   return colors;
}


class Frame : public wxFrame
{
private:
    wxBoxSizer *bx;
    graph *g;
    std::string filename;
    std::vector<std::set<int>> occs;
    wxTextCtrl *results;
    bool exists_on_list(std::string data)
    {
        if (this->occs.size() - 1 >= std::stoi(data))
        {
            return true;
        }
        return false;
    }
    std::set<std::string> exams;
    void comboPanel()
    {
        wxPanel *panel = new wxPanel(this, wxID_ANY);
        wxGridSizer *gs = new wxGridSizer(0, 4, 3, 3);
        wxStaticText *st = new wxStaticText(panel, wxID_ANY, "SELECT ACTION", wxDefaultPosition, wxSize(100, 20));
        wxButton *sel = new wxButton(panel, wxID_ANY, "FILE", wxDefaultPosition, wxSize(60, 20));
        wxButton *ff = new wxButton(panel, wxID_ANY, "FIRST FIT", wxDefaultPosition, wxSize(70, 20));
        wxButton *exp = new wxButton(panel, wxID_ANY, "EXPORT", wxDefaultPosition, wxSize(60, 20));
        sel->Bind(wxEVT_BUTTON, &Frame::openfile, this);
        ff->Bind(wxEVT_BUTTON, &Frame::first_fit, this);
        exp->Bind(wxEVT_BUTTON, &Frame::Export, this);
        gs->Add(st);
        gs->Add(sel);
        gs->Add(ff);
        gs->Add(exp);
        panel->SetSizer(gs);
        this->bx->Add(panel);
    }

public:
    Frame() : wxFrame(nullptr, wxID_ANY, "FIRST FIT", wxDefaultPosition, wxSize(400, 300))
    {
        bx = new wxBoxSizer(wxVERTICAL);
        wxStaticText *st = new wxStaticText(this, wxID_ANY, "FIRST FIT ALGORITHM FOR GRAPH COLORING", wxDefaultPosition, wxSize(290, 20));
        st->SetForegroundColour(wxColor("#803c1d"));
        bx->Add(st, 0, wxCENTRE);
        this->comboPanel();
        this->results = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(394, 200), wxTE_MULTILINE);
        bx->Add(this->results);
        this->SetSizer(bx);
    }
    ~Frame()
    {
        delete this->g;
        exit(EXIT_SUCCESS);
    }
    void openfile(wxCommandEvent &ev)
    {
        this->occs.clear();
        this->exams.clear();
        std::vector<std::string> files;
        std::string line, word;
        std::ifstream ifs;
        ifs.open("input.txt");
        int counter = 1;
        std::string message = "Select file\n";
        message += "--------------------------------------------\n";
        while (std::getline(ifs, line))
        {
            files.push_back(line);
            message += std::to_string(counter) + "." + line + "\n";
            counter++;
        }
        ifs.close();
        wxTextEntryDialog *dial = new wxTextEntryDialog(nullptr, message);
        if (dial->ShowModal() == wxID_CANCEL)
            return;
        int indeex = std::stoi(dial->GetValue().ToStdString());
        std::ifstream is;
        this->filename=files.at(indeex - 1);
        is.open("datasheets/" + files.at(indeex - 1));
        if (!is.is_open())
        {
            wxMessageBox("File did not open");
            return;
        }
        this->occs.push_back(std::set<int>());
        int studentid = 1;
        while (std::getline(is, line))
        {
            std::istringstream iss(line);
            while (iss >> word)
            {
                if (word.empty())
                {
                    std::cout << "Empty" << std::endl;
                    continue;
                }
                this->exams.insert(word);
                if (this->exists_on_list(word))
                {
                    this->occs[std::stoi(word)].insert(studentid);
                }
                else
                {
                    int j = this->exams.size();
                    while (j <= std::stoi(word))
                    {
                        this->occs.push_back(std::set<int>());
                        j++;
                    }
                    this->occs[std::stoi(word)].insert(studentid);
                }
            }
            studentid++;
        }
        is.close();
        g = new graph(this->exams.size(), this->occs);
    }
    void first_fit(wxCommandEvent &ev)
    {
        if (g == nullptr)
        {
            wxMessageBox("No Graph designed!Please select a file in order to get a graph");
            return;
        }
        std::vector<int> res = g->coloring();
        std::string ff = "";
        std::set<std::string>::iterator cnt = this->exams.begin();
        int counter = 0;
        for (auto &x : res)
        {
            counter++;
            if (counter == 1)
                continue;
            ff += "\t" + *cnt + "-->" + std::to_string(x) + "\n";
            cnt++;
        }
        this->results->SetValue(ff);
    }
    void Export(wxCommandEvent &ev)
    {
        if (g == nullptr)
            return;
        std::vector<int> coloring = g->coloring();
        std::string message = "Choose format\n";
        message += "---------------------------------------------\n";
        message += "1.Csv\n2.Html\n";
        wxTextEntryDialog *dial = new wxTextEntryDialog(nullptr, message);
        if (dial->ShowModal() == wxID_CANCEL)
        {
            return;
        }
        int option = std::stoi(dial->GetValue().ToStdString());
        std::map <int,rgb> cls;
        std::set<std::string>::iterator cnt = this->exams.begin();
        std::string htmlview="";
        std::string fn;
        std::ofstream ofs;
        wxFileDialog *savedialog;
        switch (option)
        {
         case 1:
            savedialog=new wxFileDialog(nullptr, "Export coloring results", "", "", "csv file *,csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
            if (savedialog->ShowModal() == wxID_CANCEL)
            {
                return;
            }
            fn= savedialog->GetPath().ToStdString();
            ofs.open(filename);
            ofs << "\t\tFile Created at:" << __TIME__ << "--" << __DATE__ << std::endl;
            ofs << "\t\tTotal exams enroll:" << this->exams.size() << std::endl;
            ofs << "\t\tColoring results" << std::endl;
            ofs << "------------------------------------------------" << std::endl;
            ofs << ";;;Exam;Color" << std::endl;
            for (int i = 1; i < coloring.size(); i++)
            {
                ofs << ";;;" << *cnt << ";" << std::endl;
                cnt++;
            }
            ofs.close();
            wxMessageBox("File Created as:" + filename);
            break;
         case 2:
         
           htmlview="<html><body><center><h2>Results for file:"+this->filename+"</h2><hr style=\"border-top:2px solid red;\"><table border=\"3\" style=\"background-color:white; width:60%; height:auto; color:blue; text-align:center; font-weight:bold; font-size:17px;\"><tr";
           htmlview+=" style=\"background-color:green; color:black; font-size:21px;\"><td>Vertex</td><td>Color Number</td><td>Color</td></tr>";
           cls=get_colors(*std::max_element(coloring.begin(),coloring.end()));
           cnt=this->exams.begin();
           for(int i=1,t=coloring.size();i<t;i++)
           {
               htmlview+="<tr><td>"+*cnt+"</td><td>"+std::to_string(coloring.at(i))+"</td><td style=\"background-color:rgb("+std::to_string(cls[coloring.at(i)].red)+","+std::to_string(cls[coloring.at(i)].green)+","+std::to_string(cls[coloring.at(i)].blue)+")\">";
               if(cnt==this->exams.end()) continue;
               cnt++;
           }
           htmlview+="</table></center></body></html>";
           std::cout<<htmlview<<std::endl;
           fn=wxGetCwd().ToStdString()+"//"+"overview.html";
           ofs.open(fn);
           ofs<<htmlview<<std::endl;
           ofs.close();
           wxMessageBox("File created as:"+fn);
           ShellExecuteA(NULL,"open",fn.c_str(),"","",SW_SHOWNORMAL);
           break;
        }
       
    }
};

class app : public wxApp
{
public:
    bool OnInit() wxOVERRIDE
    {
        Frame *f = new Frame;
        f->Show();
        return true;
    }
};

wxIMPLEMENT_APP(app);