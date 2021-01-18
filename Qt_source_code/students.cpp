#include "students.h"

int student::id_maker=0;

student::student()
{
    id_maker++;
    this->id="student_"+QString::number(student::id_maker);
}
QString student::get_id()const {return this->id;}
void student::add_exam(const QString &exm)
{
    this->exams.append(exm);
}
int student::totalexams()const
{
   return this->exams.size();
}
QString student::get(int i)const
{
 return this->exams.at(i);
}
bool student::is_in(const QString &lesson)const {return this->exams.indexOf(lesson)!=-1;}
QStringList student::getExams()const {return this->exams;}
QString student::info()const
{
   QString message="<center><h3>Id:"+this->id+"</h3></center><ul>";
   for(auto &y:this->exams)
   {
      message+="<li>"+y+"</li>";
   }
   message+="</ul>";
   return message;
}
