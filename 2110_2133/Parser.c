#include "Parser.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "Client.h"
#include <stdlib.h>
bool ignoreEmptyLines(char buffer[], FILE *input_file)
{
    while (buffer[0] == '\n' || buffer[0] == ' ' || buffer[0] == '\t' || buffer[0] == '\0')
    {
        if(fgets(buffer, 2048, input_file) == NULL){
            return false;
        }
    }
    return true;
}
void add_Student(char buffer[], FILE *input_file)
{
    while (fgets(buffer, 2048, input_file) != NULL && ignoreEmptyLines(buffer, input_file) && buffer[0] != '#')
    {
        int roll_no, no_of_subjects;
        char name[100];
        float cgpa;
        int k = sscanf(buffer, "%d,%100[^,],%f,%d", &roll_no, name, &cgpa, &no_of_subjects);
        if (k == 4)
        {
            addStudent(roll_no, name, cgpa);
        }
        else
        {
            printf("issue found while parsing this line '%s' in input file in # add student\n", buffer);
        }
    }
}
void modify_Student(char buffer[], FILE *input_file)
{
    while (fgets(buffer, 2048, input_file) != NULL && ignoreEmptyLines(buffer, input_file) && buffer[0] != '#')
    {
        int roll_no;
        float cgpa;
        int k = sscanf(buffer, "%d,%f", &roll_no, &cgpa);
        if (k == 2)
        {
            modifyStudent(roll_no, cgpa);
        }
        else
        {
            printf("issue found while parsing this line '%s' in input file in # modify student\n", buffer);
        }
    }
}
void delete_Student(char buffer[], FILE *input_file)
{
    while (fgets(buffer, 2048, input_file) != NULL && ignoreEmptyLines(buffer, input_file) && buffer[0] != '#')
    {
        int roll_no;

        int k = sscanf(buffer, "%d", &roll_no);
        if (k == 1)
        {
            deleteStudent(roll_no);
        }
        else
        {
            printf("issue found while parsing this line '%s' in input file in # delete student\n", buffer);
        }
    }
}
void add_Course(char buffer[], FILE *input_file)
{
    while (fgets(buffer, 2048, input_file) != NULL && ignoreEmptyLines(buffer, input_file) && buffer[0] != '#')
    {
        int roll_no, courseCode, marks;
        int k = sscanf(buffer, "%d,%d,%d", &roll_no, &courseCode, &marks);
        if (k == 3)
        {
            addCourse(roll_no, courseCode, marks);
        }
        else
        {
            printf("issue found while parsing this line '%s' in input file in # add course\n", buffer);
        }
    }
}
void modify_Course(char buffer[], FILE *input_file)
{
    while (fgets(buffer, 2048, input_file) != NULL && ignoreEmptyLines(buffer, input_file) && buffer[0] != '#')
    {
        int roll_no, courseCode, marks;
        int k = sscanf(buffer, "%d,%d,%d", &roll_no, &courseCode, &marks);
        if (k == 3)
        {
            modifyCourse(roll_no, courseCode, marks);
        }
        else
        {
            printf("issue found while parsing this line '%s' in input file in # modify course\n", buffer);
        }
    }
}
void delete_Course(char buffer[], FILE *input_file)
{
    while (fgets(buffer, 2048, input_file) != NULL && ignoreEmptyLines(buffer, input_file) && buffer[0] != '#')
    {
        int roll_no, courseCode;
        int k = sscanf(buffer, "%d,%d", &roll_no, &courseCode);
        if (k == 2)
        {
            deleteCourse(roll_no, courseCode);
        }
        else
        {
            printf("issue found while parsing this line '%s' in input file in # delete course\n", buffer);
        }
    }
}
void parsingFile(FILE *input_file)
{
    char buffer[2048];
    
    if (fgets(buffer, 2048, input_file) != NULL)
    {  
        ignoreEmptyLines(buffer, input_file);
        while (buffer[0] == '#')
        {   
            if (strcmp(buffer, "# add student\n") == 0)
            {
                add_Student(buffer, input_file);
            }
            else if (strcmp(buffer, "# modify student\n") == 0)
            {
                modify_Student(buffer, input_file);
            }
            else if (strcmp(buffer, "# delete student\n") == 0)
            {
                delete_Student(buffer, input_file);
            }
            else if (strcmp(buffer, "# add course\n") == 0)
            {
                add_Course(buffer, input_file);
            }
            else if (strcmp(buffer, "# modify course\n") == 0)
            {
                modify_Course(buffer, input_file);
            }
            else if (strcmp(buffer, "# delete course\n") == 0)
            {
                delete_Course(buffer, input_file);
            }
            else{
                fprintf(stderr,"Incorrect input\t %s",buffer);
                exit(1);
            }
        }
    }
}