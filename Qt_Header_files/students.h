#ifndef STUDENTS_H
#define STUDENTS_H
#include <QString>
#include <QStringList>

class student
{
private:
    QString id;
    QStringList exams;
public:
    static QString fileid;
    static int id_maker;
    student();
    void add_exam(const QString &exm);
    int totalexams()const;
    QString get(int i)const;
    QString get_id()const;
    QStringList getExams()const;
    bool is_in(const QString &lesson)const;
    QString info()const;

};

#endif // STUDENTS_H
