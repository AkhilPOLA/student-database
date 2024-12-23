#include <stdio.h>
#include "Student.h"
#include <stdlib.h>
Course *searchCourse(Student *search, int courseCode)
{
    Course *last = search->CourseHead;
    while (last != NULL && last->Course_Code != courseCode)
    {
        last = last->next;
    }
    if (last != NULL && last->Course_Code == courseCode)
    {
        return last;
    }
    return NULL;
}
int addCourse(int roll_No, int courseCode, int marks)
{
    Student *search;
    if ((search = searchStudent(roll_No)) != NULL)
    {
        if (searchCourse(search, courseCode) == NULL)
        {
            Course *newCourse = (Course *)malloc(sizeof(Course));
            newCourse->Course_Code = courseCode;
            newCourse->Marks = marks;
            newCourse->next = NULL;
            newCourse->previous = NULL;
            if (search->CourseHead == NULL)
            {
                search->CourseHead = newCourse;
            }
            else
            {
                Course *last = search->CourseHead;
                while (last->next != NULL)
                {
                    last = last->next;
                }
                last->next = newCourse;
                newCourse->previous = last;
            }
            search->No_of_subjects += 1;
            return 0;
        }
        else
        {
            // printf("can't add course for student with %d roll number as course with code %d already exists \n", roll_No, courseCode);
            return -1;
        }
    }
    else
    {
        // printf("can't add course as student with %d roll number does not exist \n", roll_No);
        return -2;
    }
}
int modifyCourse(int roll_No, int courseCode, int marks)
{
    Student *search;
    if ((search = searchStudent(roll_No)) != NULL)
    {
        Course *last;
        if ((last = searchCourse(search, courseCode)) != NULL)
        {
            last->Marks = marks;
            return 0;
        }
        else
        {
            // printf("can't modify course for student with %d roll number as course with code %d doesnot exist \n", roll_No, courseCode);
            return -1;
        }
    }
    else
    {
        // printf("can't modify course as student with %d roll number does not exist\n", roll_No);
        return -2;
    }
}
int deleteCourse(int roll_No, int courseCode)
{
    Student *search;
    if ((search = searchStudent(roll_No)) != NULL)
    {
        Course *last;
        if ((last = searchCourse(search, courseCode)) != NULL)
        {  
            if (last->previous == NULL)
            {
                last = last->next;
                if(last!=NULL){
                last->previous = NULL;
                }
                search->CourseHead = last;
            }
            else if (last->next == NULL)
            {
                (last->previous)->next = NULL;
            }
            else
            {
                (last->next)->previous = last->previous;
                (last->previous)->next = last->next;
            }
            search->No_of_subjects -= 1;
        }
        else
        {
            // printf("can't delete course for student with %d roll number as course with code %d doesnot exist\n", roll_No, courseCode);
            return -1;
        }
    }
    else
    {
        // printf("can't delete course as student with %d roll number does not exist\n", roll_No);
        return -2;
    }
return 0;
}
