#include "Course.h"
typedef struct Student
{
    int Roll_no;
    char Name[100];
    float CGPA;
    int No_of_subjects;
    Course *CourseHead;
    struct Student *next;
    struct Student *previous;
} Student;
extern Student *head;
extern Student *end;
extern char * outputFileName ;
int addStudent(int roll_No, char name[100], float cgpa);
int modifyStudent(int roll_No, float cgpa);
int deleteStudent(int roll_No);
Student *searchStudent(int roll_No);
void fileParse();
void writer();