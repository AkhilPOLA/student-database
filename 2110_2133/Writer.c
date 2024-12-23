#include <stdio.h>
#include <string.h>
#include "Student.h"
#include <stdlib.h>
#define MAX 2048
FILE *input_file;
void openFile(char* mode){
    input_file = fopen(outputFileName,mode);
    if (!input_file)
    {
        perror("Error opening input file");
        exit(1);
    }
}
void closeFile(){
    fclose(input_file);
}
void fileParse(){
    openFile("r");
    // FILE *input_file = fopen("2110_2133.out", "r");
    // if (!input_file)
    // {
    //     perror("Error opening input file");
    //     exit(1);
    //     // return -1;
    // }
    char buffer[MAX];
    while (fgets(buffer, MAX, input_file) != NULL)
    {
        int roll_no, no_of_subjects;
        char name[100];
        float cgpa;
        int k = sscanf(buffer, "%d,%100[^,],%f,%d", &roll_no, name, &cgpa, &no_of_subjects);
        if (k == 4)
        {   
            // printf( "%d,%s,%f,%d\n", roll_no, name, cgpa, no_of_subjects);
            addStudent(roll_no, name, cgpa);
            for (int i = 0; i < no_of_subjects; i++)
            {
                if (fgets(buffer, MAX, input_file) != NULL)
                {
                    int courseCode, marks;
                    int a = sscanf(buffer, "%d,%d", &courseCode, &marks);
                    if (a == 2)
                    {
                        addCourse(roll_no, courseCode, marks);
                        // printf( "%d,%d\n", courseCode,marks);
                    }
                    else
                    {
                        printf("issue found while parsing this line '%s' in output file \n", buffer);
                    }
                }
                else
                {
                    printf("cannot read line for adding courses in output file for student with roll number %d\n", roll_no);
                }
            }
        }
        else
        {
            printf("issue found while parsing this line '%s' in output file \n", buffer);
        }
    }
    closeFile();
    // fclose(input_file);
}
void writer()
{
    // FILE *input_file = fopen("2110_2133.out", "w");
    // if (!input_file)
    // {
    //     perror("Error opening input file");
    //     return -1;
    // }
    openFile("w");
    Student *newStudent = head;
    while (newStudent != NULL)
    {
      
         fprintf(input_file, "%d,%s,%f,%d\n", newStudent->Roll_no, newStudent->Name, newStudent->CGPA, newStudent->No_of_subjects);
         Course *course = newStudent->CourseHead;
         for (int i = 0; i < newStudent->No_of_subjects; i++)
         {
            fprintf(input_file, "%d,%d\n", course->Course_Code, course->Marks);
            course = course->next;
         }
        newStudent = newStudent->next;
    
    }
    closeFile();
    // fclose(input_file);
    // return 0;
}
