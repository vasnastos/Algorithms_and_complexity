#include <wx/wx.h>
#include <iostream>
#include <set>
#include <string>
#include "greedy.cpp"
#include <map>
#include <sstream>
#include <fstream>

//Values at files separated by ' '

class Frame : public wxFrame
{
private:
    wxBoxSizer *bx;
    graph *g;
    std::vector <std::set <int>> occs;
    wxTextCtrl *results;
    bool exists_on_list(std::string data)
    {
        if(this->occs.size()-1>=std::stoi(data))
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
        is.open("datasheets/" + files.at(indeex - 1));
        if (!is.is_open())
        {
            wxMessageBox("File did not open");
            return;
        }
        this->occs.push_back(std::set <int>());
        int studentid=1;
        while (std::getline(is, line))
        {
            std::istringstream iss(line);
            while (iss >> word)
            {
                if (word.empty())
                    continue;
                this->exams.insert(word);
                if(this->exists_on_list(word))
                {
                    this->occs[std::stoi(word)].insert(studentid);
                }
                else{
                    int j=this->exams.size();
                    while(j<=std::stoi(word))
                    {
                      this->occs.push_back(std::set<int>());
                      j++;
                    }
                    this->occs[std::stoi(word)].insert(studentid);
                }
                 studentid++;
            }
        }
        is.close();
        g = new graph(this->exams.size());
        g->fill_graph(this->occs);
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
        int counter=0;
        for (auto &x : res)
        {
            counter++;
            if(counter==1) continue;
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
        wxFileDialog savedialog(nullptr, "Export coloring results", "", "", "csv file *,csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (savedialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }
        std::string filename = savedialog.GetPath().ToStdString();
        std::ofstream ofs;
        ofs.open(filename);
        ofs << "\t\tFile Created at:" << __TIME__ << "--" << __DATE__ << std::endl;
        ofs << "\t\tTotal exams enroll:" << this->exams.size() << std::endl;
        ofs << "\t\tColoring results" << std::endl;
        ofs << "------------------------------------------------" << std::endl;
        ofs << ";;;Exam;Color" << std::endl;
        std::set<std::string>::iterator cnt = this->exams.begin();
        for (int i = 1; i < coloring.size(); i++)
        {
            ofs << ";;;" << *cnt << ";" << std::endl;
            cnt++;
        }
        ofs.close();
        wxMessageBox("File Created as:" + filename);
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